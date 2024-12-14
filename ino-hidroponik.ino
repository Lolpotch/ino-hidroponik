#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

// Define pins for SoftwareSerial
SoftwareSerial esp8266(0, 1); // RX, TX

const int IN_1 = 8;

const int analogInPin = A0;
int sensorValue = 0;
float pHVol, pHValue;
float PH9Vol = 3.11;
float PH4Vol = 3.75;
float PH_step;

const int SOLENOID_PIN = 2; // Relay pada pin 2

DateTime now;

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4); // Alamat I2C LCD (0x27), dengan ukuran 20x4

void setup() {
  // Mulai komunikasi serial dengan komputer dan ESP8266
  Serial.begin(9600);         // Komunikasi dengan Serial Monitor
  esp8266.begin(9600);      // For communication with ESP8266

  pinMode(SOLENOID_PIN, OUTPUT);

  digitalWrite(SOLENOID_PIN, LOW);

  lcd.init();       // Inisialisasi LCD
  lcd.backlight();    // Aktifkan backlight LCD

  PH_step = (PH4Vol - PH9Vol) / 5;

  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // //Inisialisasi RTC
  // if (!) {
  //   Serial.println("Tidak bisa menemukan RTC");
  //   while (1);
  // }

  //Cek jika RTC perlu diatur ulang
  if (rtc.lostPower()) {
    Serial.println("RTC kehilangan daya, atur ulang waktu!");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  lcd.setCursor(0, 3);
  lcd.print("SMART HIDROPONIK");
}

void loop() {
  ReadPhSensor();
  DisplayPh();
  DisplayTime();
  DCSet();
  HandleRelay();
  SendToEsp();
  delay(3000);
}

void ReadPhSensor() {
  sensorValue = analogRead(analogInPin);
  pHVol = (float)sensorValue * 5.0 / 1024; // Konversi nilai analog ke tegangan (0-5V)

  // Hitung nilai pH berdasarkan kalibrasi
  pHValue = 4.0 + (pHVol - PH4Vol) / (PH9Vol - PH4Vol) * (9.18 - 4.01);
}

void DisplayPh() {
  lcd.setCursor(0, 0);
  lcd.print("Volt: ");
  lcd.print(pHVol, 2);
  lcd.setCursor(0, 1);
  lcd.print("pH  : ");
  lcd.print(pHValue, 2);
}

void DisplayTime() {
  now = rtc.now();

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

void DCSet() {
  if (pHValue > 6.00) {
    digitalWrite(IN_1, HIGH);
 
  } else if (pHValue < 5.00) {
    digitalWrite(IN_1, LOW);
  }
}


void HandleRelay() {
  now = rtc.now();              // Ambil waktu saat ini dari RTC
  int currentHour = now.hour();          // Ambil jam saat ini

  if (currentHour > 10 && currentHour < 14) {
    // Pagi/Siang: Relay menyala terus
      digitalWrite(SOLENOID_PIN, HIGH);
      //Serial.println("Relay menyala terus (Siang)");
  } else {
      digitalWrite(SOLENOID_PIN, LOW);
      //Serial.println("Relay mati (not-siang)");
  }
}

void SendToEsp()
{
  Serial.println(pHValue);
}
