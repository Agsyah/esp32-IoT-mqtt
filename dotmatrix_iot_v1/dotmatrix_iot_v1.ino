/*
    IOT DotMatrix - Agus Syahputra

    PLAN:
      * Receive text data from MIT
      * Store data for dotmatrix
      * 
    
    HOW TO USE:
      * Configure your wifi ssid and password
      * Use `dotmatrix_text` as string output to dotmatrix
*/


#include <WiFi.h>                                 // ESP32 Library
#include <PubSubClient.h>                         // MQTT Library

// WiFi Configuration
const char* ssid          = "ssid";              // SSID WiFi
const char* password      = "password";         // Password WiFi

// MQTT Configuration
const char* mqttServer    = "broker.hivemq.com";  // MQTT URL
const char* mqttUserName  = "test_username";      // MQTT username
const char* mqttPwd       = "test_password";      // MQTT password
const char* clientID      = "client-micro";       // MQTT client id 
const int mqttPort        = 1883;                 // MQTT port

const char* text_topic      = "dotmatrix/teks";

// Storing data
char dotmatrix_store[16] = {};
String dotmatrix_text;


// Setting wifi and mqtt client
WiFiClient espClient;
PubSubClient client(espClient);

void WiFi_Setup()
{
  delay(10);

  Serial.println("Connecting to WiFi ...");
  WiFi.begin(ssid, password);
  delay(1000);
  WiFi.disconnect();
  delay(1000);
  WiFi.begin(ssid, password);
  delay(1000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}

void MQTT_Setup()
{
  Serial.println("Connecting to MQTT ...");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    if (client.connect(clientID, mqttUserName, mqttPwd))
    {
      Serial.println("MQTT connected!");

      client.subscribe(text_topic);
    }
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message from topic: ");
  Serial.println(topic);
  Serial.print("Message: ");

  // Reset array data
  memset(dotmatrix_store, 0, sizeof(dotmatrix_store));

  // Character collecting
  for (int i = 0; i < length; i++) {
      dotmatrix_store[i] += payload[i];
  }

  // String compare, 0: Equal | 1: Not-Equal
  if(!strcmp(text_topic, topic)) {
    dotmatrix_text = dotmatrix_store;
    Serial.println(dotmatrix_text);

    // Add your dotmatrix update code here...
    
  }
  
  Serial.println("-----------------------");
}

void reconnect() {
  Serial.println("Reconnecting to MQTT ...");
  
  while (!client.connected()) {  
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("MQTT connected");

      // List subscribe
      client.subscribe(text_topic);
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  WiFi_Setup();
  delay(1000);
  
  MQTT_Setup();
  delay(1000);
}

void loop() {
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
