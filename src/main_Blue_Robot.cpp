
#include <Arduino.h>
#include "main_BR.hpp"
#include "Steppers_BR.hpp"

//
// Variables---
// Recived Data
String data[30];

// Sensor values
int Prox_Sensor[4] = {0, 0, 0, 0};// Proximity sensor state (0 = not triggered, 1 = triggered)

int myTimeout = 2;// milliseconds

String Serial_input[90][30];  
int Serial_counter = 0;
int Serial_counter_use = 0;


// Functions---
void setup()
{
  Timer_activate = false;

  // to manipulate manualy the axes befor the first homing we disables the steppmotors
  pinMode(Robot_Enable1_Pin, OUTPUT); 
  digitalWrite(Robot_Enable1_Pin, HIGH); // HIGH Level Disable
  pinMode(Robot_Enable2_Pin, OUTPUT);
  digitalWrite(Robot_Enable2_Pin, HIGH);
  pinMode(Robot_Enable3_Pin, OUTPUT);
  digitalWrite(Robot_Enable3_Pin, HIGH);

  pinMode(Robot_Cyl_Up_Pin, OUTPUT); // Cylinder Up Pin as output 
  pinMode(Robot_Cyl_Down_Pin, OUTPUT);// Cylinder Down Pin as output
  pinMode(Robot_Vacuum_Pin, OUTPUT);// Vacuum Pin as output
  pinMode(Robot_Blast_Pin, OUTPUT);// Blast Pin as output

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
  SerialUSB.setTimeout(myTimeout); // set shorter timeout for serial read, default is 1000ms but we don't/want need that long for this application
}

/*
  void loop():
  1. The main loop is checking if there is something to read from the serial port.
  2. If there is something to read, the code reads the serial port and stores it in a string variable.
  3. The code checks if the string variable is not empty.
  4. If the string is not empty, the code processes the string and the counter is increased by one.
  5. After the code processes the string, the code checks if the timer is activated.
  6. If the timer is not activated, the code checks if the steppers are in motion.
  7. If the steppers are not in motion, the code checks if the string variable is not empty.
  8. If the string variable is not empty, the code processes the string.
  9. If the steppers are in motion, the code checks if the steppers are still in motion.
  10. If the steppers are not in motion, the code sets the inMove variable to false.
  11. If the steppers are in motion, the code sets the inMove variable to true.
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

void st_run()// run the steppers...
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

void Move(XYWA Target) // Move the robot to the target position
{
  F32 angle1;// angle of the first/upper arm
  F32 angle2;// angle of the second/lower arm
  F32 angle3;// angle of the 'wrist'

  // We chose to put the origin of the repere at the homing position of the robot 
  // So we need to compensate with the real origin (on which the equations are based)
  Target.X = Target.X - 16;
  Target.Y = Target.Y + 555;

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
      If the first axis has the most steps, I call the function speed_adapt, which will be explain later.
    4. This is done for each possible combination of the three axes.



    8. If all three axes have the same amount of steps, I set the speed of all three axes to the General_speed and General_acc.
  */

  Axis[1].Steps = abs(Axis[1].curent_position - angle1);
  Axis[2].Steps = abs(Axis[2].curent_position - angle2);
  Axis[3].Steps = abs(Axis[3].curent_position - angle3);

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
  steppers[1]->moveTo(((angle1)*Driver[Curent_Offset].MICROSTEP*Axis[1].ratio_reduc) / 360);// move the first axis to the target position

  steppers[2]->moveTo(((angle2)*Driver[Curent_Offset].MICROSTEP*Axis[2].ratio_reduc) / 360);// move the second axis to the target position

  steppers[3]->moveTo(((angle3)*Driver[Curent_Offset].MICROSTEP*Axis[3].ratio_reduc) / 360);// move the third axis to the target position

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
  F32 ratio1 = Axis[to_adapte_1].Steps / Axis[max_mvmnt].Steps; //
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

void Home()
{
  digitalWrite(Robot_Enable1_Pin, LOW); // LOW Level = Enable
  digitalWrite(Robot_Enable2_Pin, LOW);
  digitalWrite(Robot_Enable3_Pin, LOW);
  U8 delay_homing[2] = {0, 10};

  for (U8 l = 0; l < 2; l++)
  {
    for (U8 k = 1; k < stepperAmount; k++)
    {
      long initial_homing = -1;                  // Used to Home Stepper at startup
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
      initial_homing = 1;

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
  else if (realString == "O") // Valve & Blast Off
  {
    vac_Off;
    blast_Off;
    Msg_Air = "AIR O";
  }
  else
  {
    Msg_Air = "NO" + realString;
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
  AIR: Manually controls the pneumatic solenoids. The next element of the data array is the desired state of the solenoids, and the element after that is the time (in seconds) for which the solenoids should remain in that state.
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
    // Target.A = 0; // for the moment
    // Target.H = 0; // for the moment
    Move(Target);

    SerialUSB.println("AT" + String(data[1]) + String(data[2]) + String(data[3]));
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
    SerialUSB.println("OFFSET" + String(Driver[Curent_Offset].MICROSTEP)); // Return the offset
  }
  else if (data[0] == "ROFFSET") // Reads the current offsets from Flash Memory
  {
    SerialUSB.println("OFFSET," + String(Axis[1].Offset) + "," + String(Axis[2].Offset) + "," + String(Axis[3].Offset));
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
