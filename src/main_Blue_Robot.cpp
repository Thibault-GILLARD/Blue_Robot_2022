
#include <Arduino.h>
#include "main_BR.hpp"
#include "Steppers_BR.hpp"

void setup()
{
  Timer_activate = false;

  // to manipulate manualy the axes befor the first homing we disables the steppmotors
  pinMode(Robot_Enable1_Pin, OUTPUT); // set pin to output
  digitalWrite(Robot_Enable1_Pin, HIGH); // HIGH Level Disable
  pinMode(Robot_Enable2_Pin, OUTPUT);
  digitalWrite(Robot_Enable2_Pin, HIGH);
  pinMode(Robot_Enable3_Pin, OUTPUT);
  digitalWrite(Robot_Enable3_Pin, HIGH);

  pinMode(Robot_Cyl_Up_Pin, OUTPUT);   // Cylinder Up Pin as output
  pinMode(Robot_Cyl_Down_Pin, OUTPUT); // Cylinder Down Pin as output
  pinMode(Robot_Vacuum_Pin, OUTPUT);   // Vacuum Pin as output
  pinMode(Robot_Blast_Pin, OUTPUT);    // Blast Pin as output

  vac_Off;   // Vacuum OFF
  blast_Off; // Blast OFF
  cyl_Up;    // Cylinder UP

  for (U8 k = 0; k < stepperAmount; k++)
  {
    Axis[k].Homed = false;
  }

  SerialUSB.begin(115200); // The baud rate with "SerialUSB" (native port) communication don't realy mean something, via USB, the speed will always be as fast as the device can go
  while (!SerialUSB)       // Wait for the serial connection to be establised.
  {
  }
  int myTimeout = 2;               // milliseconds
  SerialUSB.setTimeout(myTimeout); // set shorter timeout for serial read, default is 1000ms but we don't/want need that long for this application
}

/*
void loop():
1. First the Arduino reads the serial USB port for any input. If there is input it will store it in a 2D array called Serial_input.
The Arduino will wait for a new line character ('\n') before storing the line in the array. The Arduino can only store 90 lines in the array.
Once the array is full it will overwrite the first line in the array if at least one command have been processed.

2. The Arduino will check if the timer is active (if we have to wait before processed any other command), and if the steppers are done moving.
If the timer is not active and the steppers are done moving, the Arduino will check if there are any lines in the Serial_input array that haven't
been processed yet. If there are lines in the Serial_input array that haven't been used yet, the Arduino will process them. The Arduino will process
a line by calling the process_orders function.

3. process_orders will parse the line and call the function that it needs to call.

4. The timer is used to delay the Arduino for a certain number of milliseconds. The timer is started by calling the start_timer function.
The start_timer function sets the Timer_activate variable to true and sets the pTime variable to the current time.
In the main loop the Arduino will check if the timer is active and if the current time is greater than the pTime + myTime.
If the current time is greater than the pTime + myTime, the Arduino will set the Timer_activate variable to false.

5. We run the stepper st_run(); function to move the steppers with the accelstepper library)

6. The Arduino will check if the steppers are done moving. If the steppers are done moving, the Arduino will set the inMove variable to false.
*/
void loop()
{
  if (SerialUSB.available())
  {
    if (Serial_counter > 90 && Serial_counter_use > 0) //
    {
      Serial_counter = 0;
      Serial_counter_use = 0;
    }
    Serial_input[Serial_counter][0] = SerialUSB.readStringUntil('\n');
    Serial_counter += 1;
  }

  if (!inMove && !Timer_activate)
  {
    if (Serial_input[Serial_counter_use][0] != "")
    {
      process_orders(Serial_input[Serial_counter_use][0]);
      Serial_counter_use += 1;
    }
  }

  if (Timer_activate)
  {
    if (millis() > pTime + myTime)
    {
      Timer_activate = false;
    }
  }

  st_run(); // run the steppers

  if (steppers[1]->distanceToGo() == 0 && steppers[2]->distanceToGo() == 0 && steppers[3]->distanceToGo() == 0)
  {
    inMove = false;
  }
  else
  {
    inMove = true;
  }
}

// Functions---
void st_run() // run the steppers...
{
  for (U8 k = 1; k < stepperAmount; k++)
  {
    steppers[k]->run();
  }
}

/*
 Reads the collected data in command and separates the data by commas and stores it in the array "data"
 Ex : "MOVE, 30, -40, 0" will be stored in the array "data" as : data[0] = "MOVE", data[1] = "30", data[2] = "-40", data[3] = "0"
*/
void read_and_extract(String content)
{
  // Remove carriage return and line feed
  content.replace("\r", "");
  content.replace("\n", "");
  for (U8 i = 0; i < 4; i++) // 4 is the maximum number of argument that can be sent in single command separated by commas
  {
    int index = content.indexOf(",");              // locate the first ","
    data[i] = content.substring(0, index).c_str(); // Extract the string from start to the ","
    content = content.substring(index + 1);        // Remove what was before the "," from the string
  }
}

/*
void process_orders(String content):
  1. The code is a function that takes a string as an argument.
  2. The function uses the read_and_extract function to take the string and separate it into individual strings, each of which is stored in the data array.
  3. The code checks the first element of the data array, and depending on the value, it executes a different part of the code. The possible values are:

  PING: Pings the controller to test communication.
  MOVE: Moves the robot arm from its current position to that indicated by the next three elements of the data array. The three elements are the X, Y, and W coordinates of the desired position.
  ID: Asks if the robot arm is right- or left-handed. Returns either ROBOTR or ROBOTL.
  HOME: Homes the robot.
  AIR: Controls the pneumatic system. The next element of the data array is the command to be executed, and the element after that is the duration of the command.
  OFFSET: Sets the offsets for the three stepper axes.
  ROFFSET: Reads the current offsets from Flash Memory.
  PROX: Returns the current values of the proximity sensors.
  SPEEDSET: Sets the maximum speed and acceleration of the robot arm to a percentage of the default values.

  4. If the value of the first element of the data array is not one of the above, the robot arm returns NACK and the value of the first element of the data array.
*/
void process_orders(String content)
{

  read_and_extract(content);

  if (data[0] == "PING")
  { // Pings the controller to test communication
    SerialUSB.println("PONG");
  }
  else if (data[0] == "MOVE") // Moves the robot arm from its current position to that indicated
  {
    char char_array[3][30];

    for (U8 k = 1; k < stepperAmount; k++)
    {
      strcpy(char_array[k], data[k].c_str());
    }

    XYWA Target;
    Target.X = atof(char_array[1]);
    Target.Y = atof(char_array[2]);
    Target.W = atof(char_array[3]);
    Move(Target);

    SerialUSB.println("AT" + String(data[1]) + ',' + String(data[2]) + ',' + String(data[3]));
  }
  else if (data[0] == "ID") // Asks if the robot arm is right- or left-handed
  {
    if (Robot == 1)
    {
      SerialUSB.println("ID, ROBOTL");
    }
    else
    {
      SerialUSB.println("ID, ROBOTR");
    }
  }
  else if (data[0] == "HOME") // Homes the robot.
  {
    SerialUSB.println("HOME");
    Home();
  }
  else if (data[0] == "AIR") // Manually controls the pneumatic solenoids
  {
    String realString = data[1];
    air_process(realString);

    char char_array_time[30];
    strcpy(char_array_time, data[2].c_str());

    if (data[2] == data[1])
    {
      SerialUSB.println(Msg_Air);
    }
    else
    {
      myTime = atof(char_array_time); // Time sends, corresponding to the time to wait before any other actions.
      pTime = millis();               // Actual time.
      Timer_activate = true;          // In the main loop Timer_activate is in this state while we he have not waited the 'myTime' duration.
      SerialUSB.println(Msg_Air + "," + String(myTime, 2));
    }
  }
  else if (data[0] == "OFFSET") // Sets the offsets for the three stepper axes
  {
    // read the data from data[1] and compare to the MICROSTEP of OFFSET_DRIVER objects Driver[14]
    for (U8 k = 0; k < 15; k++)
    {
      if (data[1] == Driver[k].ON_OFF)
      {
        Curent_Offset = k;
      }
    }
    SerialUSB.println("OFFSET," + String(Driver[Curent_Offset].MICROSTEP)); // Return the offset
  }
  else if (data[0] == "ROFFSET") // Reads the current offsets from Flash Memory
  {
    SerialUSB.println("OFFSET," + String(Driver[Curent_Offset].MICROSTEP));
  }
  else if (data[0] == "PROX") // Returns the current values of the proximity sensors
  {
    for (U8 k = 1; k < stepperAmount; k++)
    {
      pinMode(Axis[k].proxPin, INPUT_PULLUP);        // Set the pin to input ("INPUT_PULLUP" is the mode of the pin, which is to pull the pin high when it is not connected to ground.)
      Prox_Sensor[k] = digitalRead(Axis[k].proxPin); // Read the pin
    }
    SerialUSB.println("PROX," + String(Prox_Sensor[1]) + "," + String(Prox_Sensor[2]) + "," + String(Prox_Sensor[3])); // Send the proximity sensor values(state)
  }
  else if ((data[0] == "SPEEDSET")) // Sets the maximum speed and acceleration of the robot arm to a percentage of the default values
  {
    char char_array_s[3][30];

    for (U8 k = 1; k < 3; k++)
    {
      strcpy(char_array_s[k], data[k].c_str());
    }
    General_speed = General_speed + (General_speed * atof(char_array_s[2])) / 100;
    General_acc = General_acc + (General_acc * atof(char_array_s[2])) / 100;
    SerialUSB.println("SPEEDSET" + data[1] + data[2]);
  }
  else
  {
    SerialUSB.println("NACK," + String(data[0]));
  }
}

void Home() // Homes the robot
{
  digitalWrite(Robot_Enable1_Pin, LOW); // LOW Level = Enable
  digitalWrite(Robot_Enable2_Pin, LOW);
  digitalWrite(Robot_Enable3_Pin, LOW);

  /*
  In order to make the homing accurate and fast enough, the code does it in two steps.
  Once quickly, then a second time more slowly, with a delay between each homing step.
  */
  U8 delay_homing[2] = {0, 10}; // delay between each homing step in ms

  for (U8 l = 0; l < 2; l++)
  {
    for (U8 k = 1; k < stepperAmount; k++)
    {
      long initial_homing = 0;                   // Used to Home Stepper at startup
      pinMode(Axis[k].proxPin, INPUT_PULLUP);    // Set the pin to input ("INPUT_PULLUP" is the mode of the pin, which is to pull the pin high when it is not connected to ground.)
      steppers[k]->setMaxSpeed(General_speed);   // Set Max Speed of Stepper (Slower to get better accuracy)
      steppers[k]->setAcceleration(General_acc); // Set Acceleration of Stepper

      while (digitalRead(Axis[k].proxPin))
      {                                      // Make the Stepper move CCW until the switch is activated
        steppers[k]->moveTo(initial_homing); // Set the position to move to
        if (Axis[k].Direction == dir_Forward)
        {
          initial_homing = initial_homing - 1; // Decrease by 1 for next move if needed
        }
        else
        {
          initial_homing = initial_homing + 1;
        }
        steppers[k]->run(); // Start moving the stepper
        delay(delay_homing[l]);
      }
      steppers[k]->setCurrentPosition(0);        // Set the current position as zero for now
      steppers[k]->setMaxSpeed(General_speed);   // Set Max Speed of Stepper (Slower to get better accuracy)
      steppers[k]->setAcceleration(General_acc); // Set Acceleration of Stepper
      initial_homing = 0;

      while (!digitalRead(Axis[k].proxPin))
      { // Make the Stepper move CW until the switch is deactivated
        steppers[k]->moveTo(initial_homing);
        steppers[k]->run();

        if (Axis[k].Direction == dir_Forward)
        {
          initial_homing = initial_homing + 1; // Decrease by 1 for next move if needed
        }
        else
        {
          initial_homing = initial_homing - 1;
        }
        delay(delay_homing[l]);
      }
      steppers[k]->setCurrentPosition(0);
      Axis[k].Homed = true;
    }
  }
}

void Move(XYWA Target) // Move the robot to the target position
{
  F32 angle1; // angle of the first/upper arm
  F32 angle2; // angle of the second/lower arm
  F32 angle3; // angle of the 'wrist'

  // We chose to put the origin of the repere at the homing position of the robot
  // So we need to compensate with the real origin (on which the equations are based)
  Target.X = Target.X - 16;
  Target.Y = Target.Y + 555;

  // We need to calculate the angles of the three stepper motors
  F32 Q = acos((Target.X * Target.X + Target.Y * Target.Y + L1 * L1 - L2 * L2) / (2 * L1 * sqrt(Target.X * Target.X + Target.Y * Target.Y)));
  F32 S = atan2(Target.Y, Target.X) - Q;
  F32 E = acos((Target.X * Target.X + Target.Y * Target.Y - L1 * L1 - L2 * L2) / (Gamma));
  if (elbowType == -1)
  {
    angle1 = S * (180 / Pi);
    angle2 = E * (180 / Pi);
  }
  else
  {
    angle1 = -((S + 2 * Q) * 180 / Pi - 130.03844822836373);
    angle2 = (E * (180 / Pi)) - 80.09454688220181 + angle1;
  }
  angle3 = Target.W;

  /* Here is the explanation for the code above:
    For the next line the code is adapting the speed and acceleration of the steppers for each axis in order to finish the movement at the same time.
    The code must adapt the speed and acceleration of the steppers taking into consideration :
      -1 the reduction ratio of each axis.
      -2 the distance to be covered by each axis in relation to the others.
    1. The code calculate the absolute values of the distance between the current position and the target position.
    2. The current position is update  to the target position.
    3. The code compare the steps of the first axis with the steps of the second and third axis.
      If the first axis has the most steps, the function speed_adapt is called, which will be explain later.
    4. This is done for each possible combination of the three axes.
    5. If all three axes have the same amount of steps, the speed of all three axes is set to the General_speed and General_acc.
  */

  // absolute values of the distance between the current position and the target position.
  Axis[1].Steps = abs(Axis[1].curent_position - angle1);
  Axis[2].Steps = abs(Axis[2].curent_position - angle2);
  Axis[3].Steps = abs(Axis[3].curent_position - angle3);

  // update the current position to the target position
  Axis[1].curent_position = angle1;
  Axis[2].curent_position = angle2;
  Axis[3].curent_position = angle3;

  if (Axis[1].Steps > Axis[2].Steps && Axis[1].Steps > Axis[3].Steps)
  {
    speed_adapt(1, 2, 3);
  }
  else if (Axis[2].Steps > Axis[1].Steps && Axis[2].Steps > Axis[3].Steps)
  {
    speed_adapt(2, 1, 3);
  }
  else if (Axis[3].Steps > Axis[1].Steps && Axis[3].Steps > Axis[2].Steps)
  {
    speed_adapt(3, 2, 1);
    if (angle1 < 1 && angle2 < 1)
    {
      steppers[3]->setMaxSpeed(General_speed);
      steppers[3]->setAcceleration(General_acc);
    }
  }
  else
  {
    steppers[1]->setMaxSpeed(General_speed);
    steppers[1]->setAcceleration(General_acc);
    steppers[2]->setMaxSpeed(General_speed);
    steppers[2]->setAcceleration(General_acc);
    steppers[3]->setMaxSpeed(General_speed);
    steppers[3]->setAcceleration(General_acc);
  }

  // The code below is used to move the robot to the target position
  // The final target position must be adapted to the reduction ratio of each axis et the offset(microstepping)
  // So to Driver[Curent_Offset].MICROSTEP and Axis[Curent_Offset].ReductionRatio

  steppers[1]->moveTo(((angle1)*Driver[Curent_Offset].MICROSTEP * Axis[1].ratio_reduc) / 360); // move the first axis to the target position

  steppers[2]->moveTo(((angle2)*Driver[Curent_Offset].MICROSTEP * Axis[2].ratio_reduc) / 360); // move the second axis to the target position

  steppers[3]->moveTo(((angle3)*Driver[Curent_Offset].MICROSTEP * Axis[3].ratio_reduc) / 360); // move the third axis to the target position

  /* // Just for informations
  for (int k = 1; k < stepperAmount; k++)
  {
    if (Axis[k].curent_position == 0)
    {
      Axis[k].Homed = true;
    }
    else
    {
      Axis[k].Homed = false; //
    }
  } */
}

/*
void speed_adapt(int max_mvmnt, int to_adapte_1, int to_adapte_2):
  1. First, we calculate the ratio between the axis to adapt and the axis of maximum movement (max_mvmnt).
  2. Next, we calculate the maximum and acceleration speeds for the axis of maximum movement (max_mvmnt),
   and for the other two axes (to_adapte_1 and to_adapte_2).
  3. We multiply the speed and acceleration ratios by the ratio calculated in step 1, and we set the new
   speed and acceleration for the two axes to adapt.
*/
void speed_adapt(int max_mvmnt, int to_adapte_1, int to_adapte_2)
{
  F32 ratio1 = Axis[to_adapte_1].Steps / Axis[max_mvmnt].Steps;
  F32 ratio2 = Axis[to_adapte_2].Steps / Axis[max_mvmnt].Steps;

  steppers[max_mvmnt]->setMaxSpeed(Axis[max_mvmnt].ratio_speed * General_speed);   // Set the speed of the axis of maximum movement
  steppers[max_mvmnt]->setAcceleration(Axis[max_mvmnt].ratio_speed * General_acc); // Set the acceleration of the axis of maximum movement

  // Set the acceleration of the axis to adapt
  steppers[to_adapte_1]->setAcceleration(Axis[to_adapte_1].ratio_speed * General_acc * ratio1);
  steppers[to_adapte_2]->setAcceleration(Axis[to_adapte_2].ratio_speed * General_acc * ratio2);
  // Set the speed of the axis to adapt
  steppers[to_adapte_1]->setMaxSpeed(Axis[to_adapte_1].ratio_speed * General_speed * ratio1);
  steppers[to_adapte_2]->setMaxSpeed(Axis[to_adapte_2].ratio_speed * General_speed * ratio2);
}

/*
void air_process(String realString):
  1. The string realString is the string that has been received from the serial port
  2. If the string is equal to "S", then the function will call the function cyl_Stop, which is defined in the header file
  3. If the string is equal to "U", then the function will call the function cyl_Up, ...
  4. If the string is equal to "D", then the function will call the function cyl_Down, ...
  5. If the string is equal to "V", then the function will call the function vac_On, ...
  6. If the string is equal to "B", then the function will call the function blast_On, ...
  7. If the string is equal to "O", then the function will call the function vac_Off, ...
  8. If the string is equal to "O", then the function will call the function blast_Off, ...
  9. If the string is not equal to any of the above, then the function will set the string Msg_Air to "NO" + realString.
    The string "NO" will be displayed if the string received is not equal to any of the above mentioned strings.
*/
void air_process(String realString)
{
  if (realString == "S")
  {
    // Stop
    cyl_Stop;
    Msg_Air = "AIR S";
  }
  else if (realString == "U")
  {
    // Up
    cyl_Up;
    Msg_Air = "AIR U";
  }
  else if (realString == "D")
  {
    // Down
    cyl_Down;
    Msg_Air = "AIR D";
  }
  else if (realString == "V")
  {
    // Valve On
    vac_On;
    Msg_Air = "AIR V";
  }
  else if (realString == "B")
  {
    // Blast On
    blast_On;
    Msg_Air = "AIR B";
  }
  else if (realString == "O")
  {
    // Valve & Blast Off
    vac_Off;
    blast_Off;
    Msg_Air = "AIR O";
  }
  else
  {
    Msg_Air = "NO" + realString;
  }
}
