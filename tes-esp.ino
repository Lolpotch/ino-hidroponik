#include <ESP8266WiFi.h>
//data ssid dan password 
const char* ssid     = "FTP - Nalaar";
const char* password = "1011010011";     

// membuat variable wifiStatus utk menyimpan data status wifi
int wifiStatus; 
     
void setup() {
      
Serial.begin(9600);
delay(200);
    
// Membual label status terkneksi dengan nama Wifi     
Serial.println();
Serial.println();
Serial.print("Terkoneksi dengan Wifi : ");
Serial.println(ssid);
    
WiFi.begin(ssid, password);
      
//ketika WiFI.status nilainya TDK sama dg WL_CONNECTED
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }     
}   
     
void loop() {
  wifiStatus = WiFi.status();

  if(wifiStatus == WL_CONNECTED){
    Serial.println("");
    Serial.println("ESP8266 sudah terkonesi dg Wifi!");  
    Serial.println("IP address esp8266 : ");
    Serial.println(WiFi.localIP());  
    }
    else{
      Serial.println("");
      Serial.println("WiFi tdk terkoneksi");
      }
      
    delay(1000); // check for connection every once a second
}
