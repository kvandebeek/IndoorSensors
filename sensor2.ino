#include <Adafruit_MPL3115A2.h>
#include <HttpClient.h>
#include "math.h"
#include "adafruit-sht31.h"

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
Adafruit_SHT31 sht31 = Adafruit_SHT31();

HttpClient http;
http_request_t request;
http_response_t response;

http_header_t headers[] = {
     { "Accept" , "*/*"},
     { NULL, NULL } 
 };
 
void setup() {
      Serial.begin(9600);
  baro.begin();
  delay(4000);
  
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    }
    
    RGB.control(true); 
    RGB.color(0, 0, 0);
}

void postResult(String value, String sensor) 
{
  request.hostname = "8.8.8.8"; // // Change IP to the correct IP address
  request.path = "/url.php?val="; 
  request.path += value;  
  request.path += "&sen=";
  request.path += sensor;
  request.port = 80;
  http.get(request, response, headers);
  delay(200);
}

void loop() {
    

    
  sht31.readTemperature();
  sht31.readHumidity();
  baro.getTemperature();
  baro.getPressure();
    
    
  float t = sht31.readTemperature();
  postResult(String(t), "tSht31Keu");
  delay(1000);
  
  float h = sht31.readHumidity();
  postResult(String(h), "hKeuken");
  delay(1000);
    
  float pho_t2 = baro.getTemperature() + 0.2;
  postResult(String(pho_t2), "tKeuken");
  delay(1000);
  
  float pho_k2 = sht31.readTemperature();
  postResult(String(pho_k2), "tKeuken2");
  delay(1000);
  
  float barKeuken = (baro.getPressure() / 100)+5;
  postResult(String(barKeuken), "barKeuken");
  delay(1000);

    Particle.publish("wifi", "going into wifi off mode", PUBLIC);
    delay(5000);
    WiFi.off();
    delay(5000);
    WiFi.on();
    delay(5000);
    Particle.publish("wifi", "going into wifi on mode", PUBLIC);
    delay(2500);
    System.reset();
    
}
