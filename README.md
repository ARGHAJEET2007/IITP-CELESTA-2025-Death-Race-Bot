# IITP CELESTA 2025 - Death Race Bot

RC off-road robot developed for the IIT Patna CELESTA 2025 Death Race competition.

## Overview

This project is a manually controlled off-road robot built using an Arduino Uno, multiple motor drivers, six DC motors, and an FS-i6S radio control system.

The robot uses differential drive control, where the throttle and steering inputs from the RC transmitter are processed by the Arduino to independently control the left and right sides of the drivetrain.

## Hardware

- Arduino Uno
- 2 × MDD10A Motor Driver
- 2 × MD10C Motor Driver
- 6 × DC Motors
- FS-i6S Transmitter
- FS-i6S Receiver
- Buck Converter
- Battery

## Control System

### RC Channels

| Channel | Arduino Pin | Function |
|---|---|---|
| CH2 | A1 | Throttle |
| CH4 | A2 | Steering |

### Motor Control

| Function | Arduino Pin |
|---|---|
| Left Direction | D6 |
| Left PWM | D5 |
| Right Direction | D10 |
| Right PWM | D9 |

## Drive Logic

The robot uses arcade-style differential drive.

```text
Left Motor Command  = Throttle + Steering
Right Motor Command = Throttle - Steering
