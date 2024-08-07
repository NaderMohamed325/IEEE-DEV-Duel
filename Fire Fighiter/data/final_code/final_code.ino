/*------ Arduino Fire Fighting Robot Code with OLED Screen by hobby project---- */

#include <Servo.h>              // Include the Servo library
#include <Wire.h>               // Include the Wire library for I2C communication
#include <Adafruit_GFX.h>       // Include the Adafruit GFX library
#include <Adafruit_SSD1306.h>   // Include the Adafruit SSD1306 library
#include <EEPROM.h>             // Include the EEPROM library

#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels

// Create an SSD1306 display object connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Servo myservo;  // Create a Servo object to control the servo

int pos = 0;  // Variable to store the servo position
boolean fire = false;  // Flag to indicate if a fire is detected

// Define sensor pins
#define Left 8     // Left sensor
#define Right 9    // Right sensor
#define Forward 10 // Front sensor

// Define motor control pins
#define LM1 2      // Left motor forward
#define LM2 7      // Left motor backward
#define RM1 4      // Right motor forward
#define RM2 12     // Right motor backward

#define pump 6     // Water pump control pin

// Define enable pins for PWM speed control
int ena1 = 3;   
int ena2 = 5;

// RTC (Real-Time Clock) I2C address
#define RTC_address 0x68

byte bcdToDec(byte val) {
  // Convert BCD (Binary Coded Decimal) to normal decimal numbers
  return ((val / 16 * 10) + (val % 16));
}

void readtime(byte *seconds, byte *minutes, byte *hours, byte *day, byte *date, byte *month, byte *year) {
  Wire.beginTransmission(RTC_address);
  Wire.write(0);  // Start at register 0x00 (seconds)
  Wire.endTransmission();

  Wire.requestFrom(RTC_address, 7);

  if (Wire.available() == 7) {
    *seconds = bcdToDec(Wire.read() & 0x7F);  // Mask to discard the CH (Clock Halt) bit
    *minutes = bcdToDec(Wire.read());
    *hours   = bcdToDec(Wire.read() & 0x3F);  // 24-hour format, mask to discard the 12/24 hour bit
    *day     = bcdToDec(Wire.read());
    *date    = bcdToDec(Wire.read());
    *month   = bcdToDec(Wire.read());
    *year    = bcdToDec(Wire.read());
  }
}

void storeTime(byte seconds, byte minutes, byte hours, byte day, byte date, byte month, byte year) {
  EEPROM.write(0, seconds);
  EEPROM.write(1, minutes);
  EEPROM.write(2, hours);
  EEPROM.write(3, day);
  EEPROM.write(4, date);
  EEPROM.write(5, month);
  EEPROM.write(6, year);
}

void readStoredTime(byte *seconds, byte *minutes, byte *hours, byte *day, byte *date, byte *month, byte *year) {
  *seconds = EEPROM.read(0);
  *minutes = EEPROM.read(1);
  *hours   = EEPROM.read(2);
  *day     = EEPROM.read(3);
  *date    = EEPROM.read(4);
  *month   = EEPROM.read(5);
  *year    = EEPROM.read(6);
}

void setup() {
  Wire.begin();
  Serial.begin(9600);  // Initialize serial communication for debugging
  
  // Initialize OLED display with error check
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for most 128x64 OLED displays
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextSize(1);       // Set text size
  display.setTextColor(SSD1306_WHITE);  // Set text color
  display.setCursor(0, 0);
  display.println(F("Fire Fighting Robot"));
  display.display();
  delay(2000);  // Display welcome message for 2 seconds
  
  // Set sensor pins as inputs
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Forward, INPUT);
  
  // Set motor and pump pins as outputs
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(ena1, OUTPUT);
  pinMode(ena2, OUTPUT);

  myservo.attach(11);  // Attach the servo to pin 11
  myservo.write(90);   // Set the initial servo position to 90 degrees (center)
  
  byte seconds, minutes, hours, day, date, month, year;
  readStoredTime(&seconds, &minutes, &hours, &day, &date, &month, &year);

  Serial.print("Stored Time: ");
  Serial.print(hours);
  Serial.print(":");
  Serial.print(minutes);
  Serial.print(":");
  Serial.print(seconds);
  Serial.print(" Stored Date: ");
  Serial.print(date);
  Serial.print("/");
  Serial.print(month);
  Serial.print("/");
  Serial.print(2000 + year);  // Assuming year is 20xx
  Serial.print(" Stored Day: ");
  Serial.println(day);
}

// Function to put out the fire
void put_off_fire() {
  delay(100);  // Wait for half a second

  // Stop the robot's movement by setting all motor pins high
  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, HIGH);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, HIGH);
  
  delay(100);  // Wait for another half a second
  
  // Sweep the servo to spray water over a wide area
  for (pos = 50; pos <= 130; pos += 1) { 
    digitalWrite(pump, HIGH);  // Turn on the water pump
    myservo.write(pos);  // Move the servo to the next position
    
    // Update OLED with current angle
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Putting out fire\nAngle: ");
    display.println(pos);
    display.display();

    delay(10);  // Small delay for smooth servo movement
  }
  
  for (pos = 130; pos >= 50; pos -= 1) { 
    digitalWrite(pump, LOW);  // Keep the pump on
    myservo.write(pos);  // Move the servo back
    
    // Update OLED with current angle
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Putting out fire\nAngle: ");
    display.println(pos);
    display.display();

    delay(10);  // Small delay for smooth servo movement
  }
  
  myservo.write(90);  // Reset the servo to center position
  
  fire = false;  // Reset the fire flag after putting out the fire
}

void loop() {
  myservo.write(90);  // Keep the servo centered when idle
  Serial.println("angle : 90 ");  // Print the current servo angle for debugging
  
  // Read sensor values and print them for debugging
  int leftSensor = digitalRead(Left);
  int rightSensor = digitalRead(Right);
  int forwardSensor = digitalRead(Forward);

  Serial.print("sL : ");
  Serial.println(leftSensor);
  Serial.print("sR : ");
  Serial.println(rightSensor);
  Serial.print("sF : ");
  Serial.println(forwardSensor);

  // Update OLED with sensor status
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Sensors:\nL: ");
  display.print(leftSensor);
  display.print(" R: ");
  display.print(rightSensor);
  display.print(" F: ");
  display.println(forwardSensor);
  display.display();

  // If all sensors detect a fire, stop the robot and turn on the pump
  if (leftSensor == 1 && rightSensor == 1 && forwardSensor == 1) {
    Serial.println("stop");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Fire Detected!\nStopping...");
    display.display();

    digitalWrite(pump, HIGH);  // Turn on the water pump
    digitalWrite(LM1, HIGH);   // Stop left motor
    digitalWrite(LM2, HIGH);   // Stop left motor
    digitalWrite(RM1, HIGH);   // Stop right motor
    digitalWrite(RM2, HIGH);   // Stop right motor
    
    // Read current time from RTC
    byte seconds, minutes, hours, day, date, month, year;
    readtime(&seconds, &minutes, &hours, &day, &date, &month, &year);

    // Store the current time in EEPROM
    storeTime(seconds, minutes, hours, day, date, month, year);
    
  } 
  // If the front sensor detects a fire, move the robot forward
  else if (forwardSensor == 0) {
    Serial.println("forward");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Moving Forward");
    display.display();

      analogWrite(ena1,255);
     analogWrite(ena2,255);
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM2, HIGH);
    digitalWrite(RM1, LOW);
    fire = true;  // Set the fire flag
    delay(100);  // Short delay to allow the robot to move forward
  } 
  // If the left sensor detects a fire, turn left
  else if (leftSensor == 0) {
    Serial.println("left");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Turning Left");
    display.display();
      analogWrite(ena1,255);
     analogWrite(ena2,255);
    digitalWrite(LM2, HIGH);
    digitalWrite(LM1, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH);
    delay(100);  // Short delay for the robot to complete the turn
  } 
  // If the right sensor detects a fire, turn right
  else if (rightSensor == 0) {
    Serial.println("Right");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Turning Right");
    display.display();
      analogWrite(ena1,255);
     analogWrite(ena2,255);
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    delay(100);  // Short delay for the robot to complete the turn
  }
  
  delay(100);  // Slow down the robot's movement by adding a delay
  
  // If a fire is detected, activate the fire extinguishing function
  while (fire == true) {
    Serial.println("put off fire");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Extinguishing Fire...");
    display.display();

    put_off_fire();  // Call the function to put out the fire
  }
}
