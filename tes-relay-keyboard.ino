#include <L298N.h>

const int EN_A = 10;
const int IN_1 = 8;
const int IN_2 = 9;
L298N driver(EN_A,IN_1,IN_2);

const int RELAY_PIN = 2; // Relay pada pin 2

void setup()
{
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);

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
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      
      Serial.println("Relay is ON.");
    }
    else if (input.equalsIgnoreCase("0")) 
    {
      digitalWrite(RELAY_PIN, HIGH); // Relay inactive
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      Serial.println("Relay is OFF.");
    }
    else 
    {
      Serial.println("Invalid input. Please enter 'ON' or 'OFF'.");
    }
  }
}
