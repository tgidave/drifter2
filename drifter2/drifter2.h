#ifndef DRIFTER2_H
#define DRIFTER2_H

//To turn off the debugging messages, comment out the next line.

//#define SERIAL_DEBUG

//The following defines are used to control what data is transmitted during debugging.
//If "SERIAL_DEBUG" is not defined they have no effect.

#ifdef SERIAL_DEBUG
  #define SERIAL_DEBUG_TIME
  #define SERIAL_DEBUG_GPS
  #define SERIAL_DEBUG_TEMP
  #define SERIAL_DEBUG_IMU
  #define SERIAL_DEBUG_ROCKBLOCK

  #define DEBUG_SERIAL Serial1
  #define DEBUG_BAUD 115200
#endif

//The following define is used to determine if the rockblock board is connected through 
//a relay where the code must turn the power on and off.  Comment out this line if
//the rockblock is directly connected to power.

//#define MANAGE_ROCKBLOCK_POWER

//The next line turns on and off the code that averages the ACCELZ value.

#define AVERAGE_ACCELZ

//The following lines how the ACCELZ value is averaged.  
//If "AVERAGE_ACCELZ" is not defined they have no effect.

#ifdef AVERAGE_ACCELZ
  #define ACCELZ_AVG_SAMPLE_COUNT 10
  #define ACCELZ_AVG_SAMPLERATE_DELAY_MS (IMU_SAMPLERATE_DELAY_MS / ACCELZ_AVG_SAMPLE_COUNT)
  #define IMU_AVG_SAMPLERATE_DELAY_MS (IMU_SAMPLERATE_DELAY_MS - (ACCELZ_AVG_SAMPLERATE_DELAY_MS * ACCELZ_AVG_SAMPLE_COUNT))
#else
  #define IMU_AVG_SAMPLERATE_DELAY_MS (IMU_SAMPLERATE_DELAY_MS)
#endif

//The next line determines weather one or two records are sent during each transmit event.
//If two records are sent, the second record contains only motion vector data.
#define SEND_2_RECORDS

//These defines determine how many motion vectors in each record.
#define VECT_COUNT_0  25
#define VECT_COUNT_1  28

//Maximum size of the output vectors.
#define OUTBUFFER_SIZE  340

//These defines determine when transmit happens.  "ALWAYS_TRANSMIT" will transmit every hour on the half our.
//"ALWAYS_ALWAYS_TRANSMIT" will transmit as soon as the device is powered up in addition to every hour on the half hour.
//"NEVER_TRANSMIT" will never transmit.
//These defines are used mostly during the debugging process.

//#define ALWAYS_TRANSMIT
//#define ALWAYS_ALWAYS_TRANSMIT
//#define NEVER_TRANSMIT

#ifdef ALWAYS_ALWAYS_TRANSMIT
  #define ALWAYS_TRANSMIT
#endif

//Uncomment The next line to disable the code that manages the temperature sensor.

//#define DISABLE_TEMP_SENSOR

//The next lines determine when the code attemps to transmit data back through the Iridium network.  The timezone used for
//the hour values is the UTC timezone.  Currently, the number of times to transmit is limited to four times per 24 hour day.  
//
//***All four TRANSMIT_HOUR_n values must be specified***.
//
//If less than four transmit events per day are requires, set the extra transmit times equal to one of the other required transmit times.
//I.E. if three transmit events are required and the last transmit occurs at hour 18, set both the TRANSMIT_HOUR_3 and 
//TRANSMIT_HOUR_4 values to he 18.  If only one transmit time is required at noon UTC time, set all four values to 12. 
 
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



