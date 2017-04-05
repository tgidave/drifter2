#ifndef _ROCKBLOCK_H
  #define _ROCKBLOCK_H

#include "drifter2.h"

#define ROCKBLOCK_SERIAL Serial3
#define ROCKBLOCK_RX_PIN 7 // Pin marked RX on RockBlock
#define ROCKBLOCK_TX_PIN 8 // Pin marked TX on RockBlock
#define ROCKBLOCK_SLEEP_PIN 6
#define ROCKBLOCK_BAUD 19200
#define ROCKBLOCK_POWER_PIN 5

int transmitGPSFix(drifterData *, int);

#endif



