
//*****************************************************************************
//
// drifter2.ino
//
// This is the code for the main processor of the drifter2 device.  This code
// sleeps for an hour in low power mode and then wakes up and takes a GPS
// reading getting the date, time, and location.  The temperature is retrieved
// from the temp sensor.  The yawn, roll, and Z axis acceleration is gathered
// from the IMU.  All his data is collected in the drifter data structure.
// At certain hours during the day the drifter data structure
// is transmitted back to the user using the Iridium system.  The system then
// goes back to sleep for an hour.
//
//*****************************************************************************

#include <Snooze.h>
#include <SnoozeBlock.h>

#include <Time.h>
#include <TimeAlarms.h>

#include <i2c_t3.h>

//#include "atlastemp.h"
#include "brtemp.h"
#include "gps.h"
#include "imu.h"
#include "rockblock.h"
#include "drifter2.h"

int noFixFoundCount;

//unsigned long loopStartTime;

static TimeElements teensyTimeElements;

uint16_t seqNbr;

drifterData0 ddData0;
#ifdef SEND_2_RECORDS
drifterData1 ddData1;
#endif

SnoozeAlarm	alarm;

SnoozeBlock config_teensy35(alarm);

bool firstTimeAfterReset;

#ifdef SERIAL_DEBUG
void debugClockDisplay() {

  char debugBuff[50];

  sprintf(debugBuff, "%d/%02d/%02d %02d:%02d:%02d\r\n",
          year(), month(), day(), hour(), minute(), second());
  DEBUG_SERIAL.print(debugBuff);
}
#endif

time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

void setup() {

#ifdef ALWAYS_ALWAYS_TRANSMIT
  firstTimeAfterReset = false;
#else
  firstTimeAfterReset = true;
#endif

  seqNbr = 0;

  pinMode(GPS_POWER_PIN, OUTPUT);
  digitalWrite(GPS_POWER_PIN, LOW);

  pinMode(ROCKBLOCK_POWER_PIN, OUTPUT);
  digitalWrite(ROCKBLOCK_POWER_PIN, LOW);

  pinMode(IMU_POWER_PIN, OUTPUT);
  digitalWrite(IMU_POWER_PIN, LOW);

  pinMode(TEMP_POWER_PIN, OUTPUT);
  digitalWrite(TEMP_POWER_PIN, LOW);

  // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time);

  Wire.begin();

#ifdef SERIAL_DEBUG
  // Start the serial ports
  DEBUG_SERIAL.begin(DEBUG_BAUD);
  delay(1000);
  DEBUG_SERIAL.print("\r\n\r\nDebugging start.\r\n");
#endif

}

void loop() {

  int i;
  int fixFound = false;
  time_t GPSTime;
  time_t nextTime;
  imuVect* vectPtr;
  noFixFoundCount = 0;

#ifdef SERIAL_DEBUG_TIME
  debugClockDisplay();
#endif
  ddData0.ddSeqNbr = seqNbr;
  ddData0.ddRecordType = 0;

  fixFound = getGPSFix();

  if (fixFound == true) {
    teensyTimeElements.Year =   ddData0.ddYear =   gpsGetYear();
    teensyTimeElements.Year -= 1970;
    teensyTimeElements.Month =  ddData0.ddMonth =  gpsGetMonth();
    teensyTimeElements.Day =    ddData0.ddDay =    gpsGetDay();
    teensyTimeElements.Hour =   ddData0.ddHour =   gpsGetHour();
    teensyTimeElements.Minute = ddData0.ddMinute = gpsGetMinute();
    teensyTimeElements.Second = ddData0.ddSecond = gpsGetSecond();

    GPSTime = makeTime(teensyTimeElements);
    Teensy3Clock.set(GPSTime);
    setTime(GPSTime);
#ifdef SERIAL_DEBUG_TIME
    debugClockDisplay();
#endif
    ddData0.ddLatitude = gpsGetLatitude();
    ddData0.ddLongitude = gpsGetLongitude();
    ddData0.ddAltitude = gpsGetAltitude();
    ddData0.ddSpeed = gpsGetSpeed();
    ddData0.ddCourse = gpsGetCourse();

    noFixFoundCount = 0;
  } else {
    ++noFixFoundCount;
  }

#ifdef DISABLE_TEMP_SENSOR
  ddData0.ddTemperature = 0;
#else
  brIinitializeTemp();
  ddData0.ddTemperature = brGetCurrentTemp();
  brShutdownTemp();
#endif

#ifdef SERIAL_DEBUG_IMU
  DEBUG_SERIAL.print("Temperature = ");
  DEBUG_SERIAL.print(ddData0.ddTemperature);
  DEBUG_SERIAL.print("\r\n");
#endif

  if (imuPowerUp() == 0) {

    vectPtr = getIMUPosition(); //The first entry is always zeros so throw it away.
    delay(IMU_SAMPLERATE_DELAY_MS * 10);

#ifdef SERIAL_DEBUG_IMU
    DEBUG_SERIAL.print("Record 0\r\n");
#endif
    for (i = 0; i < VECT_COUNT_0; ++i) {
      vectPtr = getIMUPosition();
      ddData0.ddVect[i].pitch = vectPtr->pitch;
      ddData0.ddVect[i].roll = vectPtr->roll;
      ddData0.ddVect[i].accelZ = vectPtr->accelZ;

      if (IMU_AVG_SAMPLERATE_DELAY_MS > 0) {
        delay(IMU_AVG_SAMPLERATE_DELAY_MS);
      }
    }
#ifdef SEND_2_RECORDS
    ddData1.ddSeqNbr = seqNbr;
    ddData1.ddRecordType = 1;

#ifdef SERIAL_DEBUG_IMU
    DEBUG_SERIAL.print("Record 1\r\n");
#endif

    for (i = 0; i < VECT_COUNT_1; ++i) {
      vectPtr = getIMUPosition();
      ddData1.ddVect[i].pitch = vectPtr->pitch;
      ddData1.ddVect[i].roll = vectPtr->roll;
      ddData1.ddVect[i].accelZ = vectPtr->accelZ;

      if (IMU_SAMPLERATE_DELAY_MS > 0) {
        delay(IMU_AVG_SAMPLERATE_DELAY_MS);
      }
    }
#endif

#ifdef SERIAL_DEBUG_IMU
  } else {
    DEBUG_SERIAL.print("No BNO055 detected ... Check your wiring or I2C ADDR!\r\n");
#endif
  }

  imuPowerDown();

#ifdef SERIAL_DEBUG
  debugClockDisplay();
  DEBUG_SERIAL.print("sizeof(ddData0) = ");
  DEBUG_SERIAL.print(sizeof(ddData0));
  DEBUG_SERIAL.print("\r\n");

#ifdef SEND_2_RECORDS
  DEBUG_SERIAL.print("sizeof(ddData1) = ");
  DEBUG_SERIAL.print(sizeof(ddData1));
  DEBUG_SERIAL.print("\r\n");
#endif
  DEBUG_SERIAL.flush();
#endif

  if (firstTimeAfterReset) {
    firstTimeAfterReset = false;
  } else {
#ifdef ALWAYS_TRANSMIT
#ifdef SEND_2_RECORDS
    transmitGPSFix(&ddData0, sizeof(ddData0), &ddData1, sizeof(ddData1));
#else
    transmitGPSFix(&ddData0, sizeof(ddData0));
#endif
    ++seqNbr;
#else
#ifndef NEVER_TRANSMIT
    // If the time is right, send the data back home using the Iritium system.
    if ((hour() == TRANSMIT_HOUR_1) ||
        (hour() == TRANSMIT_HOUR_2) ||
        (hour() == TRANSMIT_HOUR_3) ||
        (hour() == TRANSMIT_HOUR_4)) {
#ifdef SEND_2_RECORDS
      transmitGPSFix(&ddData0, sizeof(ddData0), &ddData1, sizeof(ddData1));
#else
      transmitGPSFix(&ddData0, sizeof(ddData0));
#endif
      ++seqNbr;
    }
#endif
#endif
  }
  
  if ((nextTime = 30 - minute()) <= 0) {
    nextTime += 60;
  }

  alarm.setAlarm(0, nextTime, 0); // hour, min, sec

#ifdef SERIAL_DEBUG
  DEBUG_SERIAL.print("Will sleep for ");
  DEBUG_SERIAL.print(nextTime);
  DEBUG_SERIAL.print(" minutes.\r\n");
  DEBUG_SERIAL.flush();
#endif

  Snooze.hibernate(config_teensy35); // Sleep for an hour
}



