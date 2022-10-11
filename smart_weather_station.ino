#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>

char buffer[100];

 String m;

void setup() {
  // Initialize Serial port
  Serial.begin(38400);
 while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(434E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

//String msg = serializeJson(doc, Serial);
 char buffer[100];
} 

void loop(){

  const int capacity = JSON_OBJECT_SIZE(3);
StaticJsonDocument<capacity> doc;

   doc["id"] = "9080";
doc["lat"] = 48.748010;
doc["lon"] = 2.293491;
//unsigned char m = serializeJsonPretty(doc, buffer);
serializeJson(doc, Serial);
  LoRa.beginPacket();
  LoRa.print(buffer); 

  LoRa.endPacket();
  
  Serial.print(buffer);
  Serial.println();
  delay(50);
  }
