#define _OFFSET_BR_HPP_

#include <AccelStepper.h> // AccelStepper library
#include "USB_device_R.h"

U8 volatile Curent_Offset = 7; // Driver[7] - initilised to the max microstepping value (25600)


struct OFFSET_DRIVER
{
    String ON_OFF;  // Switch position of the driver 0 = ON, 1 =OFF
    U32 MICROSTEP; // Microstepping value
};

OFFSET_DRIVER Driver[14] = {
    // 14 driver microstep position possible
    {"0000", 200},   // 0
    {"0100", 400},   // 1
    {"0010", 800},   // 2
    {"0110", 1600},  // 3
    {"0001", 3200},  // 4
    {"0101", 6400},  // 5
    {"0011", 12800}, // 6
    {"0111", 25600}, // 7
    {"1000", 500},   // 8
    {"1100", 1000},  // 9
    {"1010", 2500},  // 10
    {"1110", 5000},  // 11
    {"1001", 12500}, // 12
    {"1101", 25000}  // 13
};