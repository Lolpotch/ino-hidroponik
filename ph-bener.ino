#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <L298N.h>

const int EN_A = 10;
const int IN_1 = 8;
const int IN_2 = 9;

L298N driver(EN_A,IN_1,IN_2);

const int analogInPin = A0;
int sensorValue = 0;
float pHVol, pHValue;
float PH9Vol = 3.20;
float PH4Vol = 3.82;
float PH_step;

const int DC_PIN = 4;
const int RELAY_PIN = 2; // Relay pada pin 2

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4); // Alamat I2C LCD (0x27), dengan ukuran 20x4

void setup() {
  // Mulai komunikasi serial dengan komputer dan ESP8266
  Serial.begin(9600);         // Komunikasi dengan Serial Monitor

  pinMode(DC_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(DC_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  lcd.init();       // Inisialisasi LCD
  lcd.backlight();    // Aktifkan backlight LCD

  PH_step = (PH4Vol - PH9Vol) / 5;

  //Inisialisasi RTC
  if (!rtc.begin()) {
    Serial.println("Tidak bisa menemukan RTC");
    while (1);
  }

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
  delay(3000);
}

// void ReadPhSensor() {
//   sensorValue = analogRead(analogInPin);
//   pHVol = (float)sensorValue*5.0/1024;

//   pHValue = 7.00 + ((PH9Vol - PH4Vol) / PH_step);

// }

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

void DCSet() {
  if (pHValue > 6.00) {
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
  } else if (pHValue < 5.00) {
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, LOW);
  }
}

unsigned long previousMillis = 0;    // Untuk menyimpan waktu sebelumnya
unsigned long interval = 5000;      // Interval nyala atau mati (5 detik)
bool relayState = false;            // Status relay (true = nyala, false = mati)
unsigned long relayStartMillis = 0; // Waktu mulai relay aktif atau nonaktif

void HandleRelay() {
  DateTime now = rtc.now();              // Ambil waktu saat ini dari RTC
  int currentHour = now.hour();          // Ambil jam saat ini
  unsigned long currentMillis = millis(); // Ambil waktu saat ini dalam millis

  if (currentHour > 10 && currentHour < 14) {
    // Pagi/Siang: Relay menyala terus
    if (!relayState) {
      relayState = true;
      digitalWrite(RELAY_PIN, HIGH);
      relayStartMillis = currentMillis; // Catat waktu mulai menyala
      Serial.println("Relay menyala terus (Siang)");
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
