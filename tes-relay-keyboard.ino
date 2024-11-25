const int DC_PIN = 4;
const int RELAY_PIN = 2; // Relay pada pin 2

void setup()
{
  pinMode(DC_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(DC_PIN, LOW);
  digitalWrite(RELAY_PIN, HIGH);

  // Initialize Serial Communication
  Serial.begin(9600);
  Serial.println("Enter 'ON' to turn the relay on, and 'OFF' to turn it off.");
}

void loop()
{
  if (Serial.available() > 0) // Check if data is available from Serial
  {
    String input = Serial.readStringUntil('\n'); // Read input until newline character
    input.trim(); // Remove any leading or trailing whitespace

    if (input.equalsIgnoreCase("1")) 
    {
      digitalWrite(RELAY_PIN, LOW); // Relay active
      digitalWrite(DC_PIN, LOW);
      Serial.println("Relay is ON.");
    }
    else if (input.equalsIgnoreCase("0")) 
    {
      digitalWrite(RELAY_PIN, HIGH); // Relay inactive
      digitalWrite(DC_PIN, HIGH);
      Serial.println("Relay is OFF.");
    }
    else 
    {
      Serial.println("Invalid input. Please enter 'ON' or 'OFF'.");
    }
  }
}
