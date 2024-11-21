#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SimpleTimer.h>

SimpleTimer timer;

float calibration_value = 15.00 - 0.31;

int pHsensor = A1;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

float ph_act;

RTC_DS3231 rtc;

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Alamat I2C LCD (0x27), dengan ukuran 16x2

const int RELAY_PIN = 4;

float voltage, pHValue;

// Function for animating text movement from left to right


void setup() {
  Serial.begin(9600);

  pinMode(pHsensor, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  timer.setInterval(500L, display_pHValue);
  

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

  if (pHValue > 6) {digitalWrite(RELAY_PIN, HIGH);}
  else if (pHValue < 5) {digitalWrite(RELAY_PIN, LOW);}x

  digitalWrite(RELAY_PIN, LOW);

  delay(1000);
}

void ReadPhSensor()
{
  timer.run(); // Initiates SimpleTimer
 for(int i=0;i<10;i++) 
 { 
  buffer_arr[i]=analogRead(pHsensor);
  delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5/1024.0/6;  
 //Serial.print("Voltage: ");
 //Serial.println(volt);
  ph_act = -4.90 * volt + calibration_value;

 Serial.print("pH Val: ");
 Serial.println(ph_act);

 delay(1000);
}

void DisplayPh()
{
  lcd.setCursor(0, 0);
  lcd.print("Volt: ");
  lcd.print(voltage, 2);
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