// This #include statement was automatically added by the Particle IDE.
#include <SparkFun_MPL3115A2.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_DHT_Particle.h>

// This #include statement was automatically added by the Particle IDE.

#include <HttpClient.h>
#include <Adafruit_MPL3115A2.h>
#include <HttpClient.h>

#define DHTPINA D3     // what pin we're connected to
#define DHTPINB D2     // what pin we're connected to
#define DHTTYPE DHT22		// DHT 22 (AM2302)
#define DHTTYPEB DHT11

DHT dht(DHTPINA, DHTTYPE);
DHT dhtb(DHTPINB, DHTTYPEB);

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
MPL3115A2 sparkbaro = MPL3115A2();

HttpClient http;
http_request_t request;
http_response_t response;

http_header_t headers[] = {
     { "Accept" , "*/*"},
     { NULL, NULL } 
 };
 
String pho_hum_SerreText = "pho_serre_hum";
String pho_bar_SerreText = "pho_serre_bar";
String pho_tmp_SerreText = "pho_serre_tmp";

void setup() {
  baro.begin();
  delay(2000);
  dht.begin();
  delay(2000);
  sparkbaro.begin();
  delay(2000);

  dhtb.getHumidity();
  delay(2000);
  baro.getPressure()/100;
  delay(2000);
  dht.getTempCelcius();
  delay(2000);
  baro.getTemperature();
  delay(2000);
  sparkbaro.readTemp();
  delay(2000);
}

void postResult(String value, String sensor) 
{
  request.hostname = "8.8.8.8"; //replace with correct IP
  request.path = "/url.php?val="; 
  request.path += value;  
  request.path += "&sen=";
  request.path += sensor;
  request.port = 80;
  http.get(request, response, headers);
}

void loop() {

    float pho_serre_hum = dhtb.getHumidity();
    delay(2000);
    float pho_serre_bar1 = (baro.getPressure()/100)+8;
    delay(2000);
    float pho_serre_tmp1 = dht.getTempCelcius() ;
    delay(2000);
    float pho_serre_tmp2a = baro.getTemperature() ;
    delay(2000);
    float pho_serre_tmp2b = sparkbaro.readTemp();
    delay(2000);
    float pho_serre_tmp2 = 0;
    
  
    postResult(String(pho_serre_bar1), pho_bar_SerreText);
    delay(250);
    
    postResult(String(pho_serre_hum), pho_hum_SerreText);
    delay(250);
    
    if (pho_serre_tmp2a > 70)
    {
        pho_serre_tmp2 = pho_serre_tmp2b;
    }
    else
    {
        pho_serre_tmp2 = pho_serre_tmp2a;
    }

    
    if (pho_serre_tmp1 > -200 && pho_serre_tmp1 < 200)
    {
        if (pho_serre_tmp2 > -200 && pho_serre_tmp2 < 200)
        {
            postResult((String(((pho_serre_tmp1) + pho_serre_tmp2)/2)), pho_tmp_SerreText);
            delay(250);
            
            postResult(String((pho_serre_bar1)),"pho_serre_bar1");
            delay(250);
            postResult(String((pho_serre_tmp1)),"pho_serre_tmp1");
            delay(250);
            postResult(String((pho_serre_tmp2)),"pho_serre_tmp2");
            delay(250);
            postResult(String((pho_serre_hum)),"pho_serre_hum");
            delay(250);
        }
    }
    
    Particle.publish("pho_serre_bar1", String(pho_serre_bar1), PUBLIC);
    Particle.publish("pho_serre_tmp1", String(pho_serre_tmp1), PUBLIC);
    Particle.publish("pho_serre_tmp2", String(pho_serre_tmp2), PUBLIC);
    Particle.publish("pho_serre_tmp2a", String(pho_serre_tmp2a), PUBLIC);
    Particle.publish("pho_serre_tmp2b", String(pho_serre_tmp2b), PUBLIC);
        Particle.publish("pho_serre_hum", String(pho_serre_hum), PUBLIC);

    Particle.publish("wifi", "going into wifi off mode", PUBLIC);
    delay(1500);
    WiFi.off();
    delay(10000);
    WiFi.on();
    delay(10000);
    Particle.publish("wifi", "going into wifi on mode", PUBLIC);
    delay(1500);
    System.reset();
}
