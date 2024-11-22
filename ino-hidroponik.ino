#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
//#include <SimpleTimer.h>

//SimpleTimer timer;

float calibration_value = 15.00 - 0.31;

int pHsensor = A1;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

int soilMoisturePin = A0; // Sensor kelembaban tanah terhubung ke pin A2
int soilMoistureValue = 0; // Variabel untuk menyimpan nilai kelembaban
int dryThreshold = 800;   // Threshold untuk kondisi kering (adjust sesuai sensor)
int wetThreshold = 400;   // Threshold untuk kondisi basah (adjust sesuai sensor)


float ph_act, volt;

RTC_DS3231 rtc;

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Alamat I2C LCD (0x27), dengan ukuran 16x2

const int DC_PIN = 4;
const int RELAY_PIN = 2; // Relay pada pin 2

float voltage, pHValue;

// Function for animating text movement from left to right


void setup() {
  Serial.begin(9600);

  pinMode(pHsensor, INPUT);
  pinMode(DC_PIN, OUTPUT);
  pinMode(soilMoisturePin, INPUT);
    
    lcd.init();         // Inisialisasi LCD
    lcd.backlight();    // Aktifkan backlight LCD

  //timer.setInterval(500L, display_pHValue);
  

  // Inisialisasi LCD
  lcd.init();         // Gunakan lcd.init() untuk menginisialisasi LCD
  lcd.backlight();    // Mengaktifkan backlight LCD

  digitalWrite(DC_PIN, LOW);

  // // Inisialisasi RTC
  if (! rtc.begin()) {
    Serial.println("Tidak bisa menemukan RTC");
    while (1);
  }

  // Cek jika RTC perlu diatur ulang
  if (rtc.lostPower()) {
    Serial.println("RTC kehilangan daya, atur ulang waktu!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() { 

  ReadPhSensor();

  DisplayPh();
  
  DisplayTime();

  DCSet();

  nyobaRelay();

  ReadSoilMoisture();  

  DisplaySoilMoisture(); 

  delay(1000);
}

void ReadPhSensor()
{
 for (int i = 0; i < 10; i++) { 
        buffer_arr[i] = analogRead(pHsensor);
        delay(30);
    }

    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 10; j++) {
            if (buffer_arr[i] > buffer_arr[j]) {
                temp = buffer_arr[i];
                buffer_arr[i] = buffer_arr[j];
                buffer_arr[j] = temp;
            }
        }
    }

    avgval = 0;
    for (int i = 2; i < 8; i++) {
        avgval += buffer_arr[i];
    }

    float volt = (float)avgval * 5 / 1024.0 / 6;  
    ph_act = -4.90 * volt + calibration_value;

    Serial.print("pH Val: ");
    Serial.println(ph_act);
}

void DisplayPh()
{
  lcd.setCursor(0, 0);
  lcd.print("Volt: ");
  lcd.print(volt, 2);
  lcd.setCursor(0, 1);
  lcd.print("pH  : ");
  //lcd.print(pHValue, 2);
  lcd.print(ph_act, 2);
}

void DisplayTime()
{
  DateTime now = rtc.now();

  lcd.setCursor(12, 0);
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year() % 100, DEC);
  
  lcd.setCursor(15, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);

}

void DCSet()
{
  if (pHValue > 6) {digitalWrite(DC_PIN, HIGH);}
  else if (pHValue < 5) {digitalWrite(DC_PIN, LOW);}
}

unsigned long previousMillis = 0;    // Untuk menyimpan waktu sebelumnya
unsigned long interval = 5000;      // Interval nyala atau mati (5 detik)
bool relayState = false;            // Status relay (true = nyala, false = mati)
unsigned long relayStartMillis = 0; // Waktu mulai relay aktif atau nonaktif

void nyobaRelay() {
    DateTime now = rtc.now();              // Ambil waktu saat ini dari RTC
    int currentHour = now.hour();          // Ambil jam saat ini
    unsigned long currentMillis = millis(); // Ambil waktu saat ini dalam millis

    if (currentHour >= 6 && currentHour < 11) {
        // Pagi/Siang: Relay menyala terus
        if (!relayState) {
            relayState = true;
            digitalWrite(RELAY_PIN, HIGH);
            relayStartMillis = currentMillis; // Catat waktu mulai menyala
            Serial.println("Relay menyala terus (Pagi/Siang)");
        }
    } else {
        // Malam hari: Relay nyala dan mati bergantian
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis; // Reset waktu sebelumnya

            // Toggle status relay
            relayState = !relayState; 
            digitalWrite(RELAY_PIN, relayState ? HIGH : LOW);

            // Hitung durasi nyala/mati
            unsigned long duration = currentMillis - relayStartMillis;

            // Cetak status dinamis ke Serial Monitor
            if (relayState) {
                Serial.print("Relay nyala selama ");
            } else {
                Serial.print("Relay mati selama ");
            }
            Serial.print(duration / 1000); // Konversi milidetik ke detik
            Serial.println(" detik.");

            // Reset waktu mulai
            relayStartMillis = currentMillis;
        }
    }
}

void ReadSoilMoisture() {
    soilMoistureValue = analogRead(soilMoisturePin);
    Serial.print("Soil Moisture Value: ");
    Serial.println(soilMoistureValue);
}

// Fungsi untuk menampilkan status kelembaban tanah pada LCD
void DisplaySoilMoisture() {
    lcd.setCursor(0, 2); // Baris ketiga LCD
    lcd.print("Soil: ");
    
    if (soilMoistureValue > dryThreshold) {
        Serial.println("dry");
        lcd.print("Dry  ");
    } else if (soilMoistureValue < wetThreshold) {
        Serial.println("wet");
        lcd.print("Wet  ");
    } else {
        Serial.println("moist");
        lcd.print("Moist");
    }

    lcd.setCursor(0, 3); // Baris keempat untuk nilai sensor
    lcd.print("Value: ");
    lcd.print(soilMoistureValue);
}
