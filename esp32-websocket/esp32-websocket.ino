#include "defines.h"
#include "credentials.h"

#include <WiFi.h>
#include <WebSockets2_Generic.h>
#include <ArduinoJson.h>

using namespace websockets2_generic;

void onMessageCallback(WebsocketsMessage message) 
{
  Serial.print("Received: ");
  StaticJsonDocument<200> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, message.data());
  if (error)
  {
    return;
  }
  
  const char* type_txt = jsonDoc["type"];
  const char* content = jsonDoc["content"];
  String type_txt_str = String(type_txt);
  String content_str = String(content);

  Serial.print(type_txt_str);
  Serial.print(": ");
  Serial.println(content_str);



  if (type_txt_str == "LED" && content_str == "AAN")
  {
    digitalWrite(ONBOARD_LED,HIGH);
  }
  else if (type_txt_str == "LED" && content_str == "UIT")
  {
    digitalWrite(ONBOARD_LED,LOW);
  }
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

void setup() 
{
  Serial.begin(115200);

  Serial.println("\nStarting esp32-websocket on " + String(ARDUINO_BOARD));
  Serial.println(WEBSOCKETS2_GENERIC_VERSION);

  pinMode(ONBOARD_LED,OUTPUT);
  
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
  }
}

void loop() 
{
  client.poll();
}
