const int DC_PIN = 4;
const int RELAY_PIN = 2; // Relay pada pin 2

void setup()
{
  pinMode(DC_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(DC_PIN, LOW);
  digitalWrite(RELAY_PIN, HIGH);
}

void loop()
{
  digitalWrite(DC_PIN, HIGH);
  digitalWrite(RELAY_PIN, HIGH);
  delay(5000);
  digitalWrite(DC_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
  delay(5000);
}
