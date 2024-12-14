#include <ESP8266WiFi.h>

// Data SSID dan password 
const char* ssid     = "FTP - Nalaar";
const char* password = "1011010011";
const char* host = "192.168.80.115";

bool ipAddressDisplayed = false; // Status untuk memastikan IP hanya tampil sekali

void setup() {
  Serial.begin(9600);
  delay(200);

  // Menampilkan pesan koneksi dengan WiFi
  Serial.println();
  Serial.print("Terkoneksi dengan Wifi : ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Tampilkan IP Address sekali
  if (!ipAddressDisplayed) {
    Serial.println();
    Serial.print("IP address ESP8266: ");
    Serial.println(WiFi.localIP());
    ipAddressDisplayed = true;
  }
}

void loop() {
  // Cek status WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi tidak terkoneksi.");
  }

  String data ="";
  while(Serial.available()>0)
  {
    data += char(Serial.read());
  }

  data.trim();
  Serial.println(data);
   
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // This will send the request to the server
 client.print(String("GET http://localhost/nodemcuHidroponik/index.php?") + 
                          ("data_ph=") + data +
                          " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 1000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");

  delay(3000);
}
