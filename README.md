# TeensyRobotComputer
Firmware for controlling the Land Maverick soil-sampling robotic drive base

# LADriver2
# Dual Linear Actuator Motor Driver Software
This program is designed to run on an Arduino Uno microcontroller, however it _should_ be portable to any other Arduino IDE compatible microcontroller.

**Note**: If an Arduino Uno is not used, the output and input pin numbers in the program will need to be updated to the appropriate input and output pins on the microcontroller board in use.

# Hardware Setup
This section will explain how to set up the Linear Actuators

## Required Hardware
The following hardware and devices are required:
1. 2x   2-Channel Relay Module
2. 2x   Pololu cs01a 0K1214 Current Sensors (+/-20A)
    * These current sensors are **NOT** sensitive enough for the linear actuators. Barely enough difference is generated for the current drawn by the actuators.
3. 2x   Linear Actuators, Model XTL: 200mm throw, 24mm/s rate, 12VDC, 500N
4. 1x   5V 3A+ Power Source (for digital logic)
5. 1x   12V 10A Power Source (for Linear Actuators)
6. 1x   Arduino IDE compatible microcontroller (Uno, Teensy, ESP32, etc...)

## Wiring Schematic
![Wiring Schematic](Linear-Actuator-Schematic.png?raw=true)
**IMPORTANT!!** The current sensors have a label marked "i-->" on the back, pointing from one of the large holes to the other. The hole the arrow points towards is the "-" (i- negative) pin, where the hole the arrow comes from is the "+" (i+ positive) pin. These must be installed correctly for proper operation!

# Program Overview
## Software Quickstart
1. Power on microcontroller and connect to Arduino IDE
2. Upload "LADriver2.ino"
3. Wire all hardware components as specified in the wiring diagram above
4. Via Arduino IDE Serial Monitor (or any device connected to the microcontroller via serial) at 115200baud
    * Command "1" (digit one) to request extending motors
    * Command "2" (digit two) to request retracting motors
    * Command "3" (digit three) or any char except "1" or "2" to halt both motors
5. Send one of the commands.

## Full Program Description
Brief description of the program file "LADriver2.ino".
'''cpp
// Actuator 1 Relay Output Pins
const int R1_1 = 8;
const int R1_2 = 9;

// Actuator 2 Relay Output Pins
const int R2_1 = 10;
const int R2_2 = 11;
'''
Set up relay hardware pinout on microcontroller.

'''cpp
// Number of voltage values to ignore while "debouncing" each actuator
const int ignore_count = 200;
'''
Helps ignore naturally occuring, harmless current spikes at the beginning of motor movements.

'''cpp
// Current Sensor Input Pin For Actuator 1
const int CS1 = A0;
const int CS1_MP = 510; // Midpoint ((2.5v), 0A reading) for CS1
int CS1_Val = CS1_MP;   // Measured voltage for CS1
'''
Set the "midpoint" or zero current reading.

'''cpp
// Averaging Variables for CS1
const int CS1_nv = 30;         // number of values to use for smoothing
int CS1_vals[CS1_nv];          // Array of integers to hold values to average
int CS1_sum;                   // Sum of all values in the array
int CS1_avg;                   // Average of all values in array
int CS1_idx;                   // Index within value array
int CS1_ignore = ignore_count; // number of values to ignore before acting on values
bool CS1_ignoring = false;     // boolean for controlling "debounce" ignoring
int CS1_OT = 10;               // difference threshold for "Over current!" (hit object)
int CS1_NC = 3;                // threshold for "no current" (motors are stopped, possibly at endstops)
'''
Variables and Properties for smoothing averages, high current thresholds, and endstop current thresholds.

'''cpp
// Handle incoming serial byte for control
int incomingByte = 0;
'''
Variable for handling incoming serial control bytes.

'''cpp
// Extend Linear Actuator Arms
void extend()
{
  // Actuator 1
  digitalWrite(R1_1, HIGH);
  digitalWrite(R1_2, LOW);

  // Actuator 2
  digitalWrite(R2_1, HIGH);
  digitalWrite(R2_2, LOW);
}
'''
Function to extend both linear actuators.

'''cpp
// Halt Linear Actuator Arms
void halt()
{
  // Actuator 1
  digitalWrite(R1_1, HIGH);
  digitalWrite(R1_2, HIGH);

  // Actuator 2
  digitalWrite(R2_1, HIGH);
  digitalWrite(R2_2, HIGH);
}
'''
Function to stop both actuators from moving.

'''cpp
// Retract Linear Actuator Arms
void retract()
{
  // Actuator 1
  digitalWrite(R1_1, LOW);
  digitalWrite(R1_2, HIGH);

  // Actuator 2
  digitalWrite(R2_1, LOW);
  digitalWrite(R2_2, HIGH);
}
'''
Function to retract both linear actuators.

'''cpp
void readVoltage()
{
  // Read sensor 1's voltage and perform smoothing
  CS1_Val = analogRead(CS1); // Read voltage for CS1
  if (CS1_idx == CS1_nv)
  {
    CS1_idx = 0;
  }
  CS1_sum = CS1_sum - CS1_vals[CS1_idx];
  CS1_vals[CS1_idx] = CS1_Val;
  CS1_sum = CS1_sum + CS1_vals[CS1_idx];
  CS1_avg = CS1_sum / CS1_nv;
  CS1_idx++;

  // Read sensor 2's voltage and perform smoothing
  CS2_Val = analogRead(CS2); // Read voltage for CS2
  if (CS2_idx == CS2_nv)
  {
    CS2_idx = 0;
  }
  CS2_sum = CS2_sum - CS2_vals[CS2_idx];
  CS2_vals[CS2_idx] = CS2_Val;
  CS2_sum = CS2_sum + CS2_vals[CS2_idx];
  CS2_avg = CS2_sum / CS2_nv;
  CS2_idx++;

  Serial.print("1: ");
  Serial.print(CS1_avg);
  Serial.print("\t2: ");
  Serial.println(CS2_avg);

  // test if the value should be ignored (actuator just switched on = noisy readings)
  if (CS1_ignoring)
  {
    // if so, decrement counter until ignore count is 0
    if (CS1_ignore > 0)
    {
      CS1_ignore--;
    }
    // reset ignore amount and ignoring flag
    else
    {
      CS1_ignore = ignore_count;
      CS1_ignoring = false;
    }
  }
  // If not ignore, test if the reading is within certain (appropriate) bounds
  else
  {
    // Check if current is OVER the threshold!
    if (abs(CS1_avg - CS1_MP) > CS1_OT)
    {
      // if so, halt the motors immediately!
      halt();
    }
    // check if current is within "no current" threshold! (motors not moving, probably at endstop)
    else if (abs(CS1_avg - CS1_MP) < CS1_NC)
    {
      // reset the current movement status to "stopped"
      halt();
    }
  }

  // test if the value should be ignored (actuator just switched on = noisy readings)
  if (CS2_ignoring)
  {
    // if so, decrement counter until ignore count is 0
    if (CS2_ignore > 0)
    {
      CS2_ignore--;
    }
    // reset ignore amount and ignoring flag
    else
    {
      CS2_ignore = ignore_count;
      CS2_ignoring = false;
    }
  }
  // If not ignore, test if the reading is within certain (appropriate) bounds
  else
  {
    // Check if current is OVER the threshold!
    if (abs(CS2_avg - CS2_MP) > CS2_OT)
    {
      // if so, halt the motors immediately!
      halt();
    }
    // check if current is within "no current" threshold! (motors not moving, probably at endstop)
    else if (abs(CS2_avg - CS2_MP) < CS2_NC)
    {
      // reset the current movement status to "stopped"
      halt();
    }
  }
}
'''
Perform smoothing and averaging, and current "debouncing".

'''cpp
// Set up required GPIO and Serial comms
void setup()
{
  // Relay 1
  pinMode(R1_1, OUTPUT);
  pinMode(R1_2, OUTPUT);

  // Relay 2
  pinMode(R2_1, OUTPUT);
  pinMode(R2_2, OUTPUT);

  // Halt both motors
  halt();

  // Current Sensor 1
  pinMode(CS1, INPUT);

  // Current Sensor 2
  pinMode(CS2, INPUT);

  // Begin Serial comms
  Serial.begin(115200);
}
'''
Set up pinmodes and begin serial at 115200 baud.

'''cpp
void loop()
{
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    // ignore Line Feed (new line) character
    if (incomingByte == 10)
    {
      // do nothing
    }
    else if (incomingByte == 49)
    {
      CS1_ignoring = true;
      CS2_ignoring = true;
      extend();
    }
    else if (incomingByte == 50)
    {
      CS1_ignoring = true;
      CS2_ignoring = true;
      retract();
    }
    else if (incomingByte == 51)
    {
      CS1_ignoring = true;
      CS2_ignoring = true;
      halt();
    }
    else
    {
      CS1_ignoring = true;
      CS2_ignoring = true;
      halt();
    }
  }
  readVoltage();
}
'''
Receive serial input, command motor movement, and check current measurement.