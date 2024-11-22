#include <LiquidCrystal_I2C.h>

int soilMoisturePin = A0; // Sensor kelembaban tanah terhubung ke pin A2
int soilMoistureValue = 0; // Variabel untuk menyimpan nilai kelembaban
int dryThreshold = 800;   // Threshold untuk kondisi kering (adjust sesuai sensor)
int wetThreshold = 400;   // Threshold untuk kondisi basah (adjust sesuai sensor)

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Alamat I2C LCD (0x27), dengan ukuran 16x2

void setup() {
    // Kode setup utama tetap
    Serial.begin(9600);
    pinMode(soilMoisturePin, INPUT);
    
    lcd.init();         // Inisialisasi LCD
    lcd.backlight();    // Aktifkan backlight LCD

}

void loop() {
    ReadSoilMoisture();  // Tambahkan fungsi ini
    DisplaySoilMoisture(); // Tambahkan fungsi ini
    delay(1000);
}

// Fungsi untuk membaca nilai kelembaban tanah
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
