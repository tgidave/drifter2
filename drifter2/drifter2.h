#ifndef DRIFTER2_H
  #define DRIFTER2_H

#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
  #define SERIAL_DEBUG_TIME
  #define SERIAL_DEBUG_GPS
  #define SERIAL_DEBUG_TEMP
  #define SERIAL_DEBUG_IMU
  #define SERIAL_DEBUG_ROCKBLOCK

  #define DEBUG_SERIAL Serial1
  #define DEBUG_BAUD 115200
#endif

#define VECT_COUNT  25

#define OUTBUFFER_SIZE  340

//#define ALWAYS_TRANSMIT
//#define NEVER_TRANSMIT

#define TRANSMIT_HOUR_1 0
#define TRANSMIT_HOUR_2 6
#define TRANSMIT_HOUR_3 12
#define TRANSMIT_HOUR_4 18

typedef struct motionVect{
  float heading;
  float pitch;
  float roll;
} motionVect;


typedef struct drifterData{
  uint16_t ddYear;
  uint16_t ddMonth;
  uint16_t ddDay;
  uint16_t ddHour;
  uint16_t ddMinute;
  uint16_t ddSecond;
  float ddLatitude;
  float ddLongitude;
  float ddAltitude;
  float ddSpeed;
  float ddCourse;
  float ddTemperature;
  motionVect ddVect[VECT_COUNT];
} drifterData;

#endif



