float calibration = 0.00; //change this value to calibrate
const int analogInPin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  float anValue = analogRead(analogInPin);
  
  float pHVol = anValue*(5.0/1023.0);
  
  float phValue = ((pHVol / 5.0) * 14.0) - calibration;
  
  Serial.println(anValue);
  Serial.println(pHVol);
  Serial.println(phValue);
  
  delay(500);
}
