#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

#define SensorPin A0 // Connect the pH sensor to analog pin A0
#define Offset 0.00  // Calibration offset (adjust as needed)

RTC_DS3231 rtc;

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Alamat I2C LCD (0x27), dengan ukuran 16x2

const int RELAY_PIN = 4;

float voltage, pHValue;

// Function for animating text movement from left to right


void setup() {
  Serial.begin(9600);

  pinMode(SensorPin, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  

  // Inisialisasi LCD
  lcd.init();         // Gunakan lcd.init() untuk menginisialisasi LCD
  lcd.backlight();    // Mengaktifkan backlight LCD

  digitalWrite(RELAY_PIN, HIGH);

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

  // Call the animation for "SMART HIDROPONIK"
  //animateText("SMART HIDROPONIK - [PwZ]");

  if (phValue > 6) {digitalWrite(RELAY_PIN, HIGH);}
  else if (phValue < 5) {digitalWrite(RELAY_PIN, LOW);}

  digitalWrite(RELAY_PIN, LOW);

  delay(1000);
}

void ReadPhSensor()
{
  int sensorValue = analogRead(SensorPin);
  voltage = sensorValue * (5.0 / 1024.0); // Convert ADC value to voltage
  pHValue = 3.5 * voltage + Offset;       // Convert voltage to pH level (adjust multiplier if needed)
}

void DisplayPh()
{
  lcd.setCursor(0, 0);
  lcd.print("Volt: ");
  lcd.print(voltage, 2);
  lcd.setCursor(0, 1);
  lcd.print("pH  : ");
  lcd.print(pHValue, 2);
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
  if (now.minute() < 10) {
    lcd.print('0');  // Add a leading zero if minutes are less than 10
  }
  lcd.print(now.minute(), DEC);

  lcd.setCursor(11, 0);
  lcd.print('|');
  lcd.setCursor(11, 1);
  lcd.print('|');
  lcd.setCursor(0, 2);
  lcd.print("-----------+--------");
}

void animateText(const char *text) {
  int textLen = strlen(text);  // Get the length of the text
  int displayWidth = 20;      // Number of columns on the LCD
  int scrollLength = displayWidth + textLen;

  for (int position = -textLen; position <= displayWidth; position++) {
    lcd.setCursor(0, 3);
    for (int i = 0; i < displayWidth; i++) {
      if (position + i >= 0 && position + i < textLen) {
        lcd.print(text[position + i]);
      } else {
        lcd.print(' '); // Fill with spaces
      }
    }
    delay(200); // Adjust speed of animation
  }
}