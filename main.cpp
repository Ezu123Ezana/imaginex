#include <servio.h>

int output1Value = 0;
int sen1Value = 0;
int sen2Value = 0;
int const gas_sensor = A1;
int const LDR = A0;
int limit = 400;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

Servo servo_7;

void setup()
{
  Serial.begin(9600);      // Initialize serial communication
  pinMode(A0, INPUT);      // LDR (Light Dependent Resistor) pin as input
  pinMode(A1, INPUT);      // Gas sensor pin as input
  pinMode(13, OUTPUT);     // Relay control for light bulb
  servo_7.attach(7, 500, 2500); // Attach servo motor to pin 7, with min and max pulse width

  pinMode(8, OUTPUT);      // Piezo buzzer control
  pinMode(9, INPUT);       // PIR (Passive Infrared) motion sensor input
  pinMode(10, OUTPUT);     // NPN transistor switch control
  pinMode(4, OUTPUT);      // Red LED control
  pinMode(3, OUTPUT);      // Green LED control
}

void loop()
{
  // Light intensity control
  int val1 = analogRead(LDR);
  if (val1 > 500) {
    digitalWrite(13, LOW); // Turn bulb ON
    Serial.print("Bulb ON | Light intensity: ");
    Serial.println(val1);
  } else {
    digitalWrite(13, HIGH); // Turn bulb OFF
    Serial.print("Bulb OFF | Light intensity: ");
    Serial.println(val1);
  }

  // Motion detection
  sen2Value = digitalRead(9);
  if (sen2Value == 0) {
    digitalWrite(10, LOW); // NPN transistor switch OFF
    digitalWrite(4, HIGH); // Red LED ON
    digitalWrite(3, LOW);  // Green LED OFF
    Serial.println("NO Motion Detected");
  } else {
    digitalWrite(10, HIGH); // NPN transistor switch ON
    digitalWrite(4, LOW);   // Red LED OFF
    digitalWrite(3, HIGH);  // Green LED ON
    Serial.println("Motion Detected!");
  }

  // Gas sensor
  int gasValue = analogRead(gas_sensor);
  Serial.print("Gas Sensor Value: ");
  Serial.println(gasValue);
  if (gasValue > limit) {
    tone(8, 400); // Activate piezo buzzer
  }
  delay(300);
  noTone(8);

  // Servo motor control
  sen1Value = 0.01723 * readUltrasonicDistance(6, 6);
  if (sen1Value < 100) {
    servo_7.write(90); // Open door (servo angle 90°)
    Serial.print("Door Open! | Distance: ");
    Serial.print(sen1Value);
    Serial.println(" cm");
  } else {
    servo_7.write(0); // Close door (servo angle 0°)
    Serial.print("Door Closed | Distance: ");
    Serial.print(sen1Value);
    Serial.println(" cm");
  }

  delay(10); // Delay to stabilize performance
}
