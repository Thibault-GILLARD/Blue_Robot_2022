#include <Arduino.h>
#ifndef _ROBOT_DEFINES_BR_HPP_
#define _ROBOT_DEFINES_BR_HPP_

#define Robot 1 // 1 is Left-handed, 2 is Right-handed.
/*
 We need to precise the type of motor interface we are using to drive the motors whith accelstepper lib.
 FUNCTION = 0 , DRIVER = 1 , FULL2WIRE = 2 , FULL3WIRE = 3 ,FULL4WIRE = 4 , HALF3WIRE = 6 , HALF4WIRE = 8
*/ 
#define motorInterfaceType 1 

#define Pi 3.14159265358979323846f

#define L1 375.0f		// arm 1 length
#define L2 350.0f		// arm 2 length
#define Delta 263125.0f // L1^2 + L2^2
#define Gamma 262500.0f // 2 * L1 * L2

#define ratio_speed_1 1 // This is the ratio speed for gear 1
#define ratio_speed_2 0.5f // This is the ratio speed for gear 2
#define ratio_speed_3 0.17777f // This is the ratio speed for gear 3

#define ratio_reduction_1 14.0625f // This is the ratio reduction for gear 1
#define ratio_reduction_2 7.03125f // This is the ratio reduction for gear 2
#define ratio_reduction_3 2.5f // This is the ratio reduction for gear 3


/* 
// These are the number of steps required to rotate the 'arms' 360 degrees when the microstepping is set to maximum.
#define step_rev_1 360000
#define step_rev_2 180000
#define step_rev_3 64000
#define step_rev_max 360000
 */

#if (Robot == 1)// The arm is Left-handed
	#define dir_Forward 1 
	#define dir_Reverse -1 
	#define elbowType 1 
#else          // is Left-handed
	#define dir_Forward -1
	#define dir_Reverse 1
	#define elbowType -1 
#endif

#endif