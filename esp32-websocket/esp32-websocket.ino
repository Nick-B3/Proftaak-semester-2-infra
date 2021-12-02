#include <FastLED.h>

#include "defines.h"
#include "credentials.h"

#include <WiFi.h>
#include <WebSockets2_Generic.h>
#include <ArduinoJson.h>

#define NUM_LEDS 260
#define LED_PIN 27
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define LED_BRIGHTNESS 255

CRGB leds[NUM_LEDS];

using namespace websockets2_generic;

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
    ESP.restart();
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
  
  Serial.println("\nStarting esp32-websocket on " + String(ARDUINO_BOARD));
  Serial.println(WEBSOCKETS2_GENERIC_VERSION);

  pinMode(ONBOARD_LED,OUTPUT);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  
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
    return;
  }

  Serial.print("\nConnected to Wifi, Connecting to WebSockets Server @");
  Serial.println(websockets_connection_string);

  // run callback when messages are received
  client.onMessage(onMessageCallback);

  // run callback when events are occurring
  client.onEvent(onEventsCallback);

  #if USING_INSECURE_MODE
    client.setInsecure();
  #else
    // Before connecting, set the ssl fingerprint of the server
    client.setCACert(echo_org_ssl_ca_cert);
  #endif

  // Connect to server
  bool connected = client.connect(websockets_connection_string);

  if (connected) 
  {
    Serial.println("Connected!");
  } 
  else 
  {
    Serial.println("Not Connected!");
    ESP.restart();
  }
}

void loop() 
{
  client.poll();
}
