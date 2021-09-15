// Actuator 1 Relay Output Pins
const int R1_1 = 8;
const int R1_2 = 9;

// Actuator 2 Relay Output Pins
const int R2_1 = 10;
const int R2_2 = 11;

// Number of voltage values to ignore while "debouncing" each actuator
const int ignore_count = 200;

// Current Sensor Input Pin For Actuator 1
const int CS1 = A0;
const int CS1_MP = 510; // Midpoint ((2.5v), 0A reading) for CS1
int CS1_Val = CS1_MP;   // Measured voltage for CS1

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

// Current Sensor Input Pin For Actuator 2
const int CS2 = A1;
const int CS2_MP = 510; // Midpoint ((2.5v), 0A reading) for CS2
int CS2_Val = CS2_MP;   // Measured votlage for CS2

// Averaging Variables for CS2
const int CS2_nv = 30;         // number of values to use for smoothing
int CS2_vals[CS2_nv];          // Array of integers to hold values to average
int CS2_sum;                   // Sum of all values in the array
int CS2_avg;                   // Average of all values in array
int CS2_idx;                   // Index within value array
int CS2_ignore = ignore_count; // number of values to ignore before acting on values
bool CS2_ignoring = false;     // boolean for controlling "debounce" ignoring
int CS2_OT = 10;               // difference threshold for "Over current!" (hit object)
int CS2_NC = 3;                // threshold for "no current" (motors are stopped, possibly at endstops)

// Handle incoming serial byte for control
int incomingByte = 0;

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
