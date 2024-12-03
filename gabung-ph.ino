#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <L298N.h>

const int EN_A = 10;
const int IN_1 = 8;
const int IN_2 = 9;
L298N driver(EN_A,IN_1,IN_2);

float calibration = 0.00; //change this value to calibrate
const int analogInPin = A0;
int sensorValue = 0;
unsigned long int avgValue;
float b;
float pHVol, pHValue;
int buf[10],temp;

const int DC_PIN = 4;
const int RELAY_PIN = 2; // Relay pada pin 2

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4); // Alamat I2C LCD (0x27), dengan ukuran 20x4

void setup() {
  // Mulai komunikasi serial dengan komputer dan ESP8266
  Serial.begin(9600);         // Komunikasi dengan Serial Monitor

  pinMode(DC_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  lcd.init();         // Inisialisasi LCD
  lcd.backlight();    // Aktifkan backlight LCD

  digitalWrite(DC_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  // Inisialisasi RTC
  // if (!rtc.begin()) {
  //   Serial.println("Tidak bisa menemukan RTC");
  //   while (1);
  // }

  // Cek jika RTC perlu diatur ulang
  // if (rtc.lostPower()) {
  //   Serial.println("RTC kehilangan daya, atur ulang waktu!");
  //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // }
}

void loop() {
  ReadPhSensor();
  DisplayPh();
  DisplayTime();
  DCSet();
  HandleRelay();
}

void ReadPhSensor() {
  for(int i=0;i<10;i++)
  {
    buf[i]=analogRead(analogInPin);
    delay(30);
  }
  for(int i=0;i<9;i++)
    {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  
  avgValue=0;
  
  for(int i=2;i<8;i++)
  
  avgValue+=buf[i];
  
  pHVol=(float)avgValue*5.0/1024/6;
  
  pHValue = pHVol/5.0 * 14.0 + calibration;
  
  Serial.println(pHVol);
  Serial.println(pHValue);
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
  if (pHValue > 6) {
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
  } else if (pHValue < 5) {
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

  if (false) {
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
