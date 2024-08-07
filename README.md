# Fire Fighting Robot with OLED Screen - Arduino Project

## Introduction

This project involves creating a fire-fighting robot equipped with an OLED screen for status display. The robot uses sensors to detect fire and a servo motor to aim a water spray at the detected fire. The robot is capable of navigating towards the fire and extinguishing it autonomously.

## Components

1. **Arduino Uno** - The microcontroller used to control the robot.
2. **Servo Motor** - Used to control the direction of the water spray.
3. **OLED Screen** - Displays the status of the robot.
4. **Water Pump** - Used to spray water on the fire.
5. **Fire Sensors** - Detects the presence of fire.
6. **Motors and Motor Driver** - Controls the movement of the robot.
7. **Power Supply** - To power the components.

## Pin Configuration

- **Sensors**:
  - Left Sensor: Pin 8
  - Right Sensor: Pin 9
  - Front Sensor: Pin 10

- **Motor Control**:
  - Left Motor Forward: Pin 2
  - Left Motor Backward: Pin 7
  - Right Motor Forward: Pin 4
  - Right Motor Backward: Pin 12

- **PWM Speed Control**:
  - Left Motor Enable: Pin 3
  - Right Motor Enable: Pin 5

- **Servo Motor**: Pin 11

- **Water Pump**: Pin 6

- **OLED Screen**: I2C communication (SDA, SCL)

## Libraries Used

- `Servo.h`: For controlling the servo motor.
- `Wire.h`: For I2C communication.
- `Adafruit_GFX.h`: Graphics library for the OLED.
- `Adafruit_SSD1306.h`: Library for the SSD1306 OLED display.

## Setup

1. **Install Libraries**: Ensure you have the necessary libraries installed in your Arduino IDE. You can install them from the Library Manager.
   - Servo
   - Adafruit GFX Library
   - Adafruit SSD1306

2. **Connect Components**: Connect the components to the Arduino as per the pin configuration mentioned above.

3. **Upload Code**: Upload the provided Arduino code to your Arduino board.

## Code Explanation

The code is divided into several parts:

1. **Initialization**:
   - Initializes the OLED display.
   - Sets up the sensor and motor pins.
   - Displays a welcome message on the OLED screen.

2. **Main Loop**:
   - Continuously reads sensor values.
   - Displays sensor status on the OLED screen.
   - Moves the robot based on sensor input to locate the fire.
   - Stops and activates the water pump when a fire is detected.

3. **Fire Extinguishing**:
   - Sweeps the servo motor to spray water over a wide area.
   - Displays the current status on the OLED screen during the fire extinguishing process.

## Function Definitions

- `put_off_fire()`: A function that handles the fire extinguishing process by controlling the servo motor and water pump.

## Usage

1. **Power On**: Power on the robot. The OLED screen will display a welcome message.
2. **Detection**: The robot will start moving and detecting fire.
3. **Extinguishing Fire**: When a fire is detected, the robot will stop and start extinguishing the fire using the water pump and servo motor.

## Troubleshooting

- **OLED Display Issues**: If the OLED screen does not display anything, check the connections and ensure the correct I2C address (0x3C) is being used.
- **Sensor Issues**: If the sensors are not detecting fire, check the connections and ensure they are working correctly.
- **Motor Issues**: If the motors are not working, check the motor driver connections and ensure the PWM pins are correctly configured.

## Conclusion

This project demonstrates how to build a simple fire-fighting robot using an Arduino and an OLED display. The robot can autonomously navigate towards a fire and extinguish it using a water spray. The OLED screen provides real-time status updates, making it easier to monitor the robot's actions.

