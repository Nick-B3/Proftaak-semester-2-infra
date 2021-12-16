#include "defines.h"
#include "credentials.h"

#include <WebSockets2_Generic.h>
#include <ArduinoJson.h>
#include <FastLED.h>

#define NUM_LEDS 64
#define LED_PIN 6
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

using namespace websockets2_generic;

void(* resetFunc) (void) = 0;

void onMessageCallback(WebsocketsMessage message)
{
  Serial.println("Received: ");
  // const char* input;
  // size_t inputLength; (optional)

  StaticJsonDocument<384> doc;
  DeserializationError error = deserializeJson(doc, message.data());

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char* Location = doc["Location"]; // "right/left"
  int Duration = doc["Duration"]; // nullptr

  JsonObject Type = doc["Type"];

  JsonObject Type_Light = Type["Light"];
  const char* Type_Light_Onoff = Type_Light["Onoff"]; // nullptr

  JsonObject Type_Light_RGB = Type_Light["RGB"];
  int Type_Light_RGB_Red = Type_Light_RGB["Red"]; // "0"
  int Type_Light_RGB_Green = Type_Light_RGB["Green"]; // "255"
  int Type_Light_RGB_Blue = Type_Light_RGB["Blue"]; // "0"

  const char* Type_Light_HEX = Type_Light["HEX"]; // nullptr

  int Type_Sound_Frequency = Type["Sound"]["Frequency"]; // nullptr

  bool Type_Movement = Type["Movement"]; // nullptr

  int Intensity_Light = doc["Intensity"]["LightIntensity"]; // "100"
  int Intensity_Sound = doc["Intensity"]["Sound"]; // "100"

  Serial.print("Red: ");
  Serial.println(Type_Light_RGB_Red);
  Serial.print("Green: ");
  Serial.println(Type_Light_RGB_Green);
  Serial.print("Blue: ");
  Serial.println(Type_Light_RGB_Blue);
  Serial.print("Intensity_Light: ");
  Serial.println(Intensity_Light);

//  if value this, do this
  if (Type_Light_RGB_Red == 255) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Red;
    }
  } else if (Type_Light_RGB_Green == 255) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Green;
    }
  } else if (Type_Light_RGB_Blue == 255){
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Blue;
    }
  }

  FastLED.setBrightness(Intensity_Light);
  FastLED.show();
}

void onEventsCallback(WebsocketsEvent event, String data)
{
  (void) data;

  if (event == WebsocketsEvent::ConnectionOpened)
  {
    Serial.println("Connection Opened");
  }
  else if (event == WebsocketsEvent::ConnectionClosed)
  {
    Serial.println("Connection Closed");
    #if defined(ARDUINO)
      #if defined(ARDUINO_SAMD_MKR1000)
        NVIC_SystemReset();
      #else
        resetFunc();
      #endif
    #else
      ESP.restart();
    #endif
  }
  else if (event == WebsocketsEvent::GotPing)
  {
    Serial.println("Got a Ping!");
  }
  else if (event == WebsocketsEvent::GotPong)
  {
    Serial.println("Got a Pong!");
  }
}

WebsocketsClient client;

void setup() {
  Serial.begin(115200);

  Serial.println("\nStarting Websocket Connection on " + String(BOARD_NAME));
  Serial.println(WEBSOCKETS2_GENERIC_VERSION);

  pinMode(ONBOARD_LED,OUTPUT);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  
  // FastLED 2.1 Power management set at 5V, 500mA
  //set_max_power_in_volts_and_milliamps(5, 500);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  // Wait some time to connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  // Check if connected to Wi-Fi
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("No Wifi!");
    #if defined(ARDUINO)
      #if defined(ARDUINO_SAMD_MKR1000)
        NVIC_SystemReset();
      #else
        resetFunc();
      #endif
    #else
      ESP.restart();
    #endif
  }

  Serial.print("\nConnected to Wifi, Connecting to WebSockets Server @");
  Serial.println(websockets_connection_string);

  // run callback when messages are received
  client.onMessage(onMessageCallback);

  // run callback when events are occurring
  client.onEvent(onEventsCallback);

  // Connect to server
  bool connected = client.connect(websockets_connection_string);

  if (connected)
  {
    Serial.println("Connected!");
  }
  else
  {
    Serial.println("Not Connected!");
    #if defined(ARDUINO)
      #if defined(ARDUINO_SAMD_MKR1000)
        NVIC_SystemReset();
      #else
        resetFunc();
      #endif
    #else
      ESP.restart();
    #endif
  }
}

void loop()
{
  client.poll();
}
