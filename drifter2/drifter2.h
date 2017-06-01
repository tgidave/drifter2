#ifndef DRIFTER2_H
#define DRIFTER2_H

//#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
  #define SERIAL_DEBUG_TIME
  #define SERIAL_DEBUG_GPS
  #define SERIAL_DEBUG_TEMP
  #define SERIAL_DEBUG_IMU
  #define SERIAL_DEBUG_ROCKBLOCK

  #define DEBUG_SERIAL Serial1
  #define DEBUG_BAUD 115200
#endif

#define AVERAGE_ACCELZ

#ifdef AVERAGE_ACCELZ
  #define ACCELZ_AVG_SAMPLE_COUNT 10
  #define ACCELZ_AVG_SAMPLERATE_DELAY_MS 10
  #define IMU_AVG_SAMPLERATE_DELAY_MS (IMU_SAMPLERATE_DELAY_MS - (ACCELZ_AVG_SAMPLERATE_DELAY_MS * ACCELZ_AVG_SAMPLE_COUNT))
#else
  #define IMU_AVG_SAMPLERATE_DELAY_MS (IMU_SAMPLERATE_DELAY_MS)
#endif

#define SEND_2_RECORDS

#define VECT_COUNT_0  25
#define VECT_COUNT_1  28

#define OUTBUFFER_SIZE  340

//#define ALWAYS_ALWAYS_TRANSMIT    
//#define ALWAYS_TRANSMIT
//#define NEVER_TRANSMIT

#ifdef ALWAYS_ALWAYS_TRANSMIT
  #define ALWAYS_TRANSMIT
#endif

//#define DISABLE_TEMP_SENSOR

#define TRANSMIT_HOUR_1 0
#define TRANSMIT_HOUR_2 6
#define TRANSMIT_HOUR_3 12
#define TRANSMIT_HOUR_4 18

typedef struct motionVect{
    float pitch;
    float roll;
    float accelZ; 
} motionVect;

typedef struct drifterData0 {
  uint16_t ddSeqNbr;
  uint16_t ddRecordType;
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
  motionVect ddVect[VECT_COUNT_0];
} drifterData0;

typedef struct drifterData1 {
    uint16_t ddSeqNbr;
    uint16_t ddRecordType;
    motionVect ddVect[VECT_COUNT_1];
} drifterData1;
#endif



