#include <SPI.h>
#include <RH_RF95.h>
#include "DHT.h"

#define DHTPIN A0     // what pin we're connected to
#define DHTTYPE DHT11   // DHT type
DHT dht(DHTPIN, DHTTYPE);
float hum;  //Stores humidity value
float temp; //Stores temperature value

RH_RF95 rf95;

void setup() 
{
  Serial.begin(9600);
  dht.begin();
  if (!rf95.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

}

void loop()
{
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  String humidity = String(hum); //int to String
  String temperature = String(temp);
  String data = temperature + humidity;
  Serial.print(data);
  char d[5];
  data.toCharArray(d, 5); //String to char array
  Serial.println("Sending to rf95_server");
  rf95.send(d, sizeof(d));
  rf95.waitPacketSent();
  
  delay(400);
}
