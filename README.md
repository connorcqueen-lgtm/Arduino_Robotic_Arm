# Arduino Robotic Arm — Final Project

## Project Overview

This project is an Arduino-controlled robotic arm developed for my robotics and embedded systems coursework at the University of Advancing Technology.

The goal is to design and build a functional robotic arm capable of moving in multiple directions and operating a gripper using a joystick.

The robotic arm uses an Arduino Uno, servo motors, sensors, and a custom 3D-printed structure. The project demonstrates motor control, sensor integration, electrical wiring, programming, and mechanical design.

## Hardware Components

- Arduino Uno R3
- Three servo motors (SG90 and SM-S2309S)
- Analog joystick module with push button
- HC-SR04 ultrasonic distance sensor (planned)
- Tilt sensor (planned)
- LED indicators (planned)
- Breadboard and jumper wires
- External 5V USB power bank
- 3D-printed robotic arm components
- FlashForge Adventurer 5M 3D printer

## Robotic Arm Controls

The robotic arm is controlled using an analog joystick.

| Joystick Input | Function |
|---|---|
| Left / Right | Rotate the base |
| Up / Down | Raise or lower the shoulder |
| Press joystick button | Open or close the gripper |
| Release joystick | Maintain the current servo positions |

The Arduino reads the joystick's analog inputs and adjusts the servo positions accordingly.

## Software

The project is programmed using the Arduino IDE and C++.

The Arduino Servo library controls the motors.

The current working program includes:

- Independent control of two servos using the joystick
- Third servo control using the joystick button
- Servo position limits
- Joystick dead-zone detection
- Serial Monitor debugging
- External servo power supply

## 3D-Printed Components

The robotic arm structure is being printed using a FlashForge Adventurer 5M.

Print settings:

- Material: PLA
- Layer height: 0.20 mm
- Infill: 20%
- Shells: 3

The design uses existing robotic arm STL files. The original design source and attribution will be added to the project documentation.

## Project Progress

- [x] Set up Arduino Uno
- [x] Connect and test joystick
- [x] Test SG90 servo
- [x] Test SM-S2309S servo
- [x] Connect external 5V power supply
- [x] Control two servos with joystick
- [x] Control third servo with joystick button
- [x] Print robotic arm base
- [x] Print base rotator
- [x] Create GitHub repository
- [ ] Complete 3D printing
- [ ] Assemble robotic arm
- [ ] Integrate ultrasonic distance sensor
- [ ] Integrate tilt sensor
- [ ] Add LED indicators
- [ ] Complete final testing
- [ ] Record demonstration video
- [ ] Complete final lab report

## Project Goals

1. Build a functional robotic arm using 3D-printed components.
2. Control multiple servo motors using an Arduino Uno.
3. Implement joystick-based movement and gripper operation.
4. Integrate two sensors and LED indicators.
5. Demonstrate a complete working robotic system.
6. Document the hardware, software, testing, and challenges so the project can be reproduced.

## Current Status

Three servos have been successfully tested using joystick controls. The external power supply is working, and the robotic arm components are being 3D printed.

The next stage involves sensor integration and mechanical assembly.

## Progress Videos

### Three-Servo Joystick Control

This video documents the initial testing of the robotic arm's control system.

The demonstration shows the Arduino Uno controlling three servo motors using an analog joystick.

- Horizontal joystick movement controls the base servo.
- Vertical joystick movement controls the shoulder servo.
- Pressing the joystick button operates the gripper servo.

[Watch the servo control demonstration](Progress_Videos/Three_Servo_Joystick_Test.mp4)

Progress Video 2 – Complete Electronics Integration
This video demonstrates the completed Arduino robotic arm control system, including three servo motors controlled by a joystick, an HC-SR04 ultrasonic sensor, a two-pin tilt switch, and an LED warning system. Both sensors activate the warning LED without interrupting manual joystick control.