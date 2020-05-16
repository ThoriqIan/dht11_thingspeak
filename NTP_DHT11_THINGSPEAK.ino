#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "DHT.h"
#include "ThingSpeak.h"

#define DHTPIN D1

#define DHTTYPE DHT11
#define SECRET_CH_ID 806400

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Alfatih";  
const char* pass = "16042016";

const long utcOffsetInSeconds = 3600*7;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = "605YLGWBOGDDL7IT";

float h;
float t;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);
  
  WiFi.begin(ssid, pass);  

  dht.begin();
}

int ngirimmenit = 0;

void loop() {
  sensorSuhu();
  
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.begin();
    timeClient.update();
    int menit = timeClient.getMinutes();
    
    if(ngirimmenit==0){
      if(menit%5==0){
        thingspeak();
        ngirimmenit=1;
      }
    }
    if(ngirimmenit==1){
      if(menit%5!=0){
        ngirimmenit=0;
      }
    }
  }
}

void thingspeak(){
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}
  
void sensorSuhu() {
  delay(2000);

  h = dht.readHumidity();
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
}
