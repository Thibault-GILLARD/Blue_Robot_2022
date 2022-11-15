#define _MAIN_R_HPP_

#include <AccelStepper.h> // AccelStepper library
#include <Arduino.h>	  // Arduino library

#include "Type.hpp"
#include "USB_device_R.h"
#include "Pin_Define_BR.hpp"
#include "Robot_Define_BR.hpp"

/*
The volatile keyword is used to tell the compiler that the variable can change at any time,
so the compiler should not optimize the code to take advantage of the known value.
*/
Bool inMove = false; // Flag to indicate if the robot is moving or not.
S32 volatile airCount = 0;
S32 volatile airStopCount = 0;

S32 volatile General_speed = 10000; // 10000
S32 volatile General_acc = General_speed / 2;

String Msg_Air;

U32 myTime;			 // Time sends by the command "AIR" corresponding to the time to wait before any other actions.
U32 pTime;			 // Exact time when the "AIR" command is received.
Bool Timer_activate; // True while we have not wait the time specified by the command "AIR"(myTime).

//--------------------------------------------------------------------------------------------

struct AxisDescription
{
	U8 stepPin;		 // Step pin
	U8 directionPin; // Direction pin
	U8 enablePin;	 // enable pin
	U8 proxPin;		 // proximity sensor pin
	S8 Direction;
	F32 ratio_speed; // arm reduction ratio
	F32 volatile curent_position;
	F32 volatile Steps; // number of steps to do
	F32 ratio_reduc;
	S32 volatile Offset;
	Bool volatile Homed; // True if the arm is homed
};
struct XYWA
{
	F32 X;
	F32 Y;
	F32 W;
	S32 Steps[4];
	String A;
	S32 H;
};

AxisDescription Axis[4] =
	{
		{

			0,			 // stepPin
			0,			 // directionPin
			0,			 // enablePin
			0,			 // proxPin
			dir_Forward, // Direction
			0,			 // ratio_speed
			0,			 // curent_position
			0,			 // Steps
			0,			 // ratio_reduc
			false,		 // Homed
		},
		{

			Robot_Step1_Pin,	  // stepPin
			Robot_Direction1_Pin, // directionPin
			Robot_Enable1_Pin,	  // enablePin
			Robot_Prox1_Pin,	  // proxPin
			dir_Reverse,		  // Direction
			ratio_speed_1,		  // ratio_speed
			0,					  // curent_position
			0,					  // Steps
			ratio_reduction_1,	  // ratio_reduc
			0,					  // Offset
			false,				  // Homed
		},
		{

			Robot_Step2_Pin,	  // stepPin
			Robot_Direction2_Pin, // directionPin
			Robot_Enable2_Pin,	  // enablePin
			Robot_Prox2_Pin,	  // proxPin
			dir_Reverse,		  // Direction
			ratio_speed_2,		  // ratio_speed
			0,					  // curent_position
			0,					  // Steps
			ratio_reduction_2,	  // ratio_reduc
			0,					  // Offset
			false,				  // Homed
		},
		{

			Robot_Step3_Pin,	  // stepPin
			Robot_Direction3_Pin, // directionPin
			Robot_Enable3_Pin,	  // enablePin
			Robot_Prox3_Pin,	  // proxPin
			dir_Forward,		  // Direction
			ratio_speed_3,		  // ratio_speed
			0,					  // curent_position
			0,					  // Steps
			ratio_reduction_3,	  // ratio_reduc
			0,					  // Offset
			false,				  // Homed
		}};

const U8 stepperAmount = 4; // 3 + 1, explanation right after
/*
  1. The first stepper is a null stepper, just to make the array start at 1, so that the axis can be referenced by the axis number.
  2. The second, third, fourth steppers are the steppers for axis 1,2 3, with the step pin and direction pin defined.
  3. The array "steppers" is then defined, with each stepper in the array in the order that they are defined above.
*/
AccelStepper stepper0 = AccelStepper(1, 0, 0);													 // null stepper
AccelStepper stepper1 = AccelStepper(motorInterfaceType, Axis[1].stepPin, Axis[1].directionPin); // stepper for axis 1
AccelStepper stepper2 = AccelStepper(motorInterfaceType, Axis[2].stepPin, Axis[2].directionPin); // stepper for axis 2
AccelStepper stepper3 = AccelStepper(motorInterfaceType, Axis[3].stepPin, Axis[3].directionPin); // stepper for axis 3

AccelStepper *steppers[stepperAmount] = {
	// Array of pointers to AccelStepper instances
	&stepper0,
	&stepper1,
	&stepper2,
	&stepper3,
};

//--------------------------------------------------------------------------------------------

void Move(XYWA Target);											   // Move the robot to the target position
void speed_adapt(int max_mvmnt, int to_adapte_1, int to_adapte_2); // Adapt the speed of the robot to the max speed of the axis
void process_orders(String content);							   // Process the orders received by the USB
void Home(void);												   // Home the robot
void read_and_extract(String content);							   // Read the orders received by the USB and extract the data
void st_run();													   // Run the stepper motors
void air_process(String realString);							   // Process the command "AIR"
