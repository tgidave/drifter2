
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

unsigned long loopStartTime;

static TimeElements teensyTimeElements;

drifterData ddData;

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

  firstTimeAfterReset = true;
 
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
  DEBUG_SERIAL.print("Debugging start.\r\n");
#endif

}

void loop() {

  int i;
  int fixFound;
  time_t GPSTime;
  time_t nextTime;
  imuVect* vectPtr;
  noFixFoundCount = 0;

#ifdef SERIAL_DEBUG_TIME
  debugClockDisplay();
#endif
  
  fixFound = getGPSFix();

  if (fixFound == true) {
    teensyTimeElements.Year =   ddData.ddYear =   gpsGetYear();
    teensyTimeElements.Year -= 1970;
    teensyTimeElements.Month =  ddData.ddMonth =  gpsGetMonth();
    teensyTimeElements.Day =    ddData.ddDay =    gpsGetDay();
    teensyTimeElements.Hour =   ddData.ddHour =   gpsGetHour();
    teensyTimeElements.Minute = ddData.ddMinute = gpsGetMinute();
    teensyTimeElements.Second = ddData.ddSecond = gpsGetSecond();

    GPSTime = makeTime(teensyTimeElements);
    Teensy3Clock.set(GPSTime);
    setTime(GPSTime); 
#ifdef SERIAL_DEBUG_TIME
    debugClockDisplay();
#endif

    ddData.ddLatitude = gpsGetLatitude();
    ddData.ddLongitude = gpsGetLongitude();
    ddData.ddAltitude = gpsGetAltitude();
    ddData.ddSpeed = gpsGetSpeed();
    ddData.ddCourse = gpsGetCourse();

    noFixFoundCount = 0;
  } else {
    ++noFixFoundCount;
  }

  brIinitializeTemp();
  ddData.ddTemperature = brGetCurrentTemp();
  brShutdownTemp();

#ifdef SERIAL_DEBUG_IMU
  DEBUG_SERIAL.print("Temperature = ");
  DEBUG_SERIAL.print(ddData.ddTemperature);
  DEBUG_SERIAL.print("\r\n");
#endif

  if (imuPowerUp() == 0) {

    vectPtr = getIMUPosition(); //The first entry is always zeros so throw it away.
    delay(BNO055_SAMPLERATE_DELAY_MS);

    for (i = 0; i < VECT_COUNT; ++i) {
      vectPtr = getIMUPosition();
      ddData.ddVect[i].heading = vectPtr->heading;
      ddData.ddVect[i].pitch = vectPtr->pitch;
      ddData.ddVect[i].roll = vectPtr->roll;
      delay(BNO055_SAMPLERATE_DELAY_MS);
    }
  }

#ifdef SERIAL_DEBUG_IMU
  else {
    DEBUG_SERIAL.print("No BNO055 detected ... Check your wiring or I2C ADDR!\r\n");
  }
#endif

  imuPowerDown();

#ifdef SERIAL_DEBUG
  debugClockDisplay();
  DEBUG_SERIAL.print("sizeof(ddData) = ");
  DEBUG_SERIAL.print(sizeof(ddData));
  DEBUG_SERIAL.print("\r\n");
#endif

  if (firstTimeAfterReset) {
    firstTimeAfterReset = false;
  } else {
#ifdef ALWAYS_TRANSMIT
    transmitGPSFix(&ddData, sizeof(ddData));
#else
#ifndef NEVER_TRANSMIT
    // If the time is right, send the data back home using the Iritium system.
    if ((hour() == TRANSMIT_HOUR_1) ||
        (hour() == TRANSMIT_HOUR_2) ||
        (hour() == TRANSMIT_HOUR_3) ||
        (hour() == TRANSMIT_HOUR_4)) {
      transmitGPSFix(&ddData, sizeof(ddData));
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



