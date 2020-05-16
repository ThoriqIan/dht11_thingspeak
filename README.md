Cara mengkoneksikan dht11 ke thingspeak secara berkala

**Library yang dibutuhkan**

- Library DHT by Adafruit
- Library Thingspeak by MathWork
- Library NTP

**Cara Download Library**

> Buka aplikasi arduino.
> Klik menu sketch
> Include library
> Manage Library

![lib](https://user-images.githubusercontent.com/65123734/82112041-b01dd600-9773-11ea-8f4d-4326dd360534.png)

- Library DHT

> Cari dengan keyword DHT
> Install library DHT by Adafruit

![dht](https://user-images.githubusercontent.com/65123734/82112037-abf1b880-9773-11ea-95ee-83c5f7ef39df.PNG)

- Library Thingspeak

> Cari dengan keyword Thingspeak
> Install library DHT by MathWork


![thingspeak](https://user-images.githubusercontent.com/65123734/82112046-b4e28a00-9773-11ea-9f8d-4b4d0f54cb2b.PNG)

**Penjelasan Code**

- Library yang dipakai

> Library WiFi
```
#include <ESP8266WiFi.h>
```

> Library NTP
```
#include <NTPClient.h>
#include <WiFiUdp.h>
```

> Library DHT
```
#include "DHT.h"
```

> Library Thingspeak
```
#include "ThingSpeak.h"
```

> Mendefinisikan pin dan jenis DHT
```
#define DHTPIN D1
#define DHTTYPE DHT11
```

> ID channel yang digunakan
```
#define SECRET_CH_ID 806400
```
![web](https://user-images.githubusercontent.com/65123734/82112328-0f7ce580-9776-11ea-8b34-4a7d98fc659d.png)

> Mendefinisikan objek dht
```
DHT dht(DHTPIN, DHTTYPE);
```

> Mengkoneksikan dengan jaringan
```
const char* ssid = "BEDABISA";  
const char* pass = "I23A5678g";
```

> Mendifinikan waktu dalam GMT
```
const long utcOffsetInSeconds = 3600*7;
```

> Mendefinisikan NTP
```
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
```

> Mendifinisikan WiFi client
```
WiFiClient  client;
```

> Mendefinisikan Channel dan API key
```
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = "605YLGWBOGDDL7IT";
```

![api](https://user-images.githubusercontent.com/65123734/82112393-a8136580-9776-11ea-8592-9ea71e339624.PNG)

> Mendefinisikan variabel temperatur dan kelembapan dengan tipe data float
```
float h;
float t;
```


void setup() {
  // put your setup code here, to run once:
> Mendefinisikan Serial Monitor dengan Baud Rate 9600
  ```
  Serial.begin(9600);
  ```
  
> Mendefinisikan WiFi mode
  ```
  WiFi.mode(WIFI_STA); 
  ```
  
> Mendefinisikan jaringan yang dipakai ke thingspeak
  ```
  ThingSpeak.begin(client);
  ```
  
> Mengkoneksikan mikrokontroller ke WiFi
  ```
  WiFi.begin(ssid, pass);  
  ```

> Menjalankan DHT
  ```
  dht.begin();
  ```
}

> Membuat variabel agar hanya mengirim 1 kali saja dalam menit yang ditentukan
```
int ngirimmenit = 0;
```

void loop() {
> Memanggil Prosedur sensor Suhu
  ```
  sensorSuhu();
  ```

> Cek apakah WiFi sudah terkoneksi.
  ```  
  if (WiFi.status() == WL_CONNECTED) {
  ```

> Mendapatkan menit dari NTP
    ```
    timeClient.begin();
    timeClient.update();
    int menit = timeClient.getMinutes();
    ```
    
> Logika mengirim 5 menit sekali
    ```
    if(ngirimmenit==0){
      if(menit%5==0){
    ```
> Memanggil Prosedut thingspeak
        ```
        thingspeak();
        ```

> Proses agar tetap mengirim data ke thingspeak 5 menit sekali
    ```
        ngirimmenit=1;
      }
    }
    if(ngirimmenit==1){
      if(menit%5!=0){
        ngirimmenit=0;
      }
    }
    ```
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
