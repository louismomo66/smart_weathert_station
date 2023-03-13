#include <SPI.h> //Import SPI librarey 

#include <RH_RF95.h> // RF95 from RadioHead Librarey 

#define RFM95_CS 10 //CS if Lora connected to pin 10

#define RFM95_RST 9 //RST of Lora connected to pin 9

#define RFM95_INT 2 //INT of Lora connected to pin 2
#include <Wire.h>
#include <Adafruit_Sensor.h>

#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; // I2C Interface

//#include <Adafruit_BME280.h>
//#define BME_SCK 13
//#define BME_MISO 12
//#define BME_MOSI 11
//#define BME_CS 10
//#define SEALEVELPRESSURE_HPA (1013.25)
//Adafruit_BME280 bme; // I2C

// Change to 434.0 or other frequency, must match RX's freq!

#define RF95_FREQ 434.0
#include "DHT.h"
#define DHTPIN A0     // what pin we're connected to
#define DHTTYPE DHT11   // DHT type
DHT dht(DHTPIN, DHTTYPE);
float hum;  //Stores humidity value
float temp; //Stores temperature value
// Singleton instance of the radio driver

int msensor = A3; // moisture sensor is connected with the analog pin A1 of the Arduino
float msvalue = 0; 
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 

{
//Initialize Serial Monitor
Serial.begin(9600);
dht.begin();
  
Serial.println(F("BMP280 test"));
if (!bmp.begin(0x76,0x58)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
// Reset LoRa Module 
  pinMode(msensor, INPUT);
  pinMode(RFM95_RST, OUTPUT); 

  digitalWrite(RFM95_RST, LOW);

  delay(10);

  digitalWrite(RFM95_RST, HIGH);

  delay(10);
  
//Initialize LoRa Module
while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }

  

 //Set the default frequency 434.0MHz

  if (!rf95.setFrequency(RF95_FREQ)) {

    Serial.println("setFrequency failed");

    while (1);

  }

  //rf95.setTxPower(50); //Transmission power of the Lora Module

}

//char value = 48;

void loop()

{
  float bmeT= bmp.readTemperature(); 
  float bmeP =bmp.readPressure() / 100;
  float bmeA = bmp.readAltitude(1019.66);
  //float bmeH = bme.readHumidity();
  String BmeT = String(bmeT);
  String BmeP = String(bmeP);
  String BmeA = String(bmeA);
  String BmeH = String(00.00);
  
  msvalue = analogRead(msensor);
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  String soilm = String(msvalue);
  String humidity = String(hum); //int to String
  
  String temperature = String(temp);
 // Serial.println(BmeT);
   BmeH+=' '+humidity+' '+BmeA+' '+temperature+' '+BmeT+' '+BmeP+' '+soilm;
  
  //uint8_t data[6] = data1;
  //data.concat(humidity);
  //Serial.println(sizeof(data));
  char d[55];
//  /
  BmeH.toCharArray(d, 55);
 
  //char radiopacket[5] = {char(d)};
  
  Serial.println(BmeH);
  
  rf95.send(d, sizeof(d));
  rf95.waitPacketSent();
  //rf95.send((uint8_t *)data, 5);
  //Serial.println(data);
    

  delay(1000);

//  value++;

//  if (value > '9')
//
//  value = 48;

}
