#include <Arduino.h>
#ifndef _PIN_DEFINES_BR_HPP_
#define _PIN_DEFINES_BR_HPP_


// Define the pins used for the motors, the proximity sensors and the pneumatics valves.

#define Robot_Prox1_Pin 39 //Proximity Sensor 1
#define Robot_Prox2_Pin 41 //Proximity Sensor 2
#define Robot_Prox3_Pin 46 //Proximity Sensor 3

#define Robot_Cyl_Up_Pin 37 //Cylinder Up
#define Robot_Cyl_Down_Pin 42 //Cylinder Down
#define Robot_Vacuum_Pin 45 //Vacuum
#define Robot_Blast_Pin 40 //Blast

#define Robot_Step1_Pin 35 //Stepper Motor 1 Step
#define Robot_Direction1_Pin 34 //Stepper Motor 1 Direction
#define Robot_Enable1_Pin 33 //Stepper Motor 1 Enable

#define Robot_Step2_Pin 31 //Stepper Motor 2 Step
#define Robot_Direction2_Pin 30 //Stepper Motor 2 Direction
#define Robot_Enable2_Pin 29 //Stepper Motor 2 Enable

#define Robot_Step3_Pin 27 //Stepper Motor 3 Step
#define Robot_Direction3_Pin 26 //Stepper Motor 3 Direction
#define Robot_Enable3_Pin 25 //Stepper Motor 3 Enable


#define setIO(pin) digitalWrite(pin, HIGH) // set pin to high
#define clearIO(pin) digitalWrite(pin, LOW) // set pin to low
#define enableIO(pin) pinMode(pin, OUTPUT) // set pin to output

#define airFactor 13.0f

#define cyl_Stop 		(clearIO(Robot_Cyl_Up_Pin), clearIO(Robot_Cyl_Down_Pin)) /* Stop the cylinder */
#define cyl_Up 			(setIO(Robot_Cyl_Up_Pin), clearIO(Robot_Cyl_Down_Pin)) /* Move the cylinder up */
#define cyl_Down 	(clearIO(Robot_Cyl_Up_Pin), setIO(Robot_Cyl_Down_Pin)) /* Move the cylinder down */

#define vac_On 			(setIO(Robot_Vacuum_Pin), clearIO(Robot_Blast_Pin)) /* Turn on the vacuum */
#define vac_Off 		(clearIO(Robot_Vacuum_Pin)) /* Turn off the vacuum */
#define blast_On 		(setIO(Robot_Blast_Pin), clearIO(Robot_Vacuum_Pin)) /* Turn on the blast */
#define blast_Off 		(clearIO(Robot_Blast_Pin)) /* Turn off the blast */


#endif
