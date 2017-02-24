
#include <TinyGPS++.h>  // NMEA parsing: http://arduiniana.org
#include <PString.h>    // String buffer formatting: http://arduiniana.org

#include "drifter2.h"
#include "gps.h"

TinyGPSPlus tinygps;

int getGPSFix(void) {

//  int i;
  int fixfnd = false;
  unsigned long now;
//  time_t GPSTime;
  int notAvailableCount;
  char outBuffer[OUTBUFFER_SIZE];

  loopStartTime = millis();

  digitalWrite(GPS_POWER_PIN, HIGH);
  GPS_SERIAL.begin(GPS_BAUD);

  // Step 1: Reset TinyGPS++ and begin listening to the GPS
#ifdef SERIAL_DEBUG_GPS
  DEBUG_SERIAL.println("Beginning GPS");
#endif
  tinygps = TinyGPSPlus();

  // Step 2: Look for GPS signal for up to 7 minutes
  for (now = millis(); !fixfnd && ((millis() - now) < (5UL * 60UL * 1000UL));) {
    if (GPS_SERIAL.available()) {
//      DEBUG_SERIAL.println("GPS available!");
      tinygps.encode(GPS_SERIAL.read());
      fixfnd = tinygps.location.isValid() && tinygps.date.isValid() &&
          tinygps.time.isValid() && tinygps.altitude.isValid();
//    } else {
//        DEBUG_SERIAL.println("GPS not available!");
//        delay(1000);
    }
  }

//#ifdef SERIAL_DEBUG_GPS
//  if (fixfnd) {
//    DEBUG_SERIAL.println("fix found");
//  } else {
//    DEBUG_SERIAL.println("fix not found");
//  }
//#endif
  if (fixfnd) {
/*
    teensyTimeElements.Year =   dateYear =   tinygps.date.year();
    teensyTimeElements.Year -= 1970;
    teensyTimeElements.Month =  dateMonth =  tinygps.date.month();
    teensyTimeElements.Day =    dateDay =    tinygps.date.day();
    teensyTimeElements.Hour =   timeHour =   tinygps.time.hour();
    teensyTimeElements.Minute = timeMinute = tinygps.time.minute();
    teensyTimeElements.Second = timeSecond = tinygps.time.second();

    GPSTime = makeTime(teensyTimeElements);
    Teensy3Clock.set(GPSTime);
    setTime(GPSTime);
#ifdef SERIAL_DEBUG_TIME
    debugClockDisplay();
#endif
    latitude = tinygps.location.lat();
    longitude = tinygps.location.lng();

#ifndef ALTITUDE_ARRAY
    altitude = tinygps.altitude.meters();
#endif

    speed = tinygps.speed.knots();
    course = tinygps.course.value() / 100;
    notAvailableCount = 0;

#ifdef ALTITUDE_ARRAY
    for (i = 0; i < WAVE_COUNT;) {
      if (GPS_SERIAL.available()) {
        tinygps.encode(GPS_SERIAL.read());
        waveData[i] = tinygps.altitude.meters();
        ++i;
        notAvailableCount = 0;
      } else {
        ++notAvailableCount;
        if (notAvailableCount >= 3) {
          waveData[i] = 0;
          notAvailableCount = 0;
          ++i;
#ifdef SERIAL_DEBUG_GPS
          DEBUG_SERIAL.print("notAvailableCount = 3\r\n");
#endif
        }
      }

#ifdef SERIAL_DEBUG_GPS
      sprintf(outBuffer, "i = %d %d\r\n", i, notAvailableCount);
      DEBUG_SERIAL.print(outBuffer);
#endif
      delay(1000);
    }
#endif
*/
#ifdef SERIAL_DEBUG_GPS
    *outBuffer = 0;
    PString str(outBuffer, OUTBUFFER_SIZE);
    str.print("fix found! ");
    str.print(tinygps.location.lat(), 6);
    str.print(",");
    str.print(tinygps.location.lng(), 6);
    str.print(",");
    str.print(tinygps.speed.knots(), 1);
    str.print(",");
    str.print(tinygps.altitude.meters());
    str.print(",");
    str.print(tinygps.course.value() / 100);
    str.print("\r\n");
    DEBUG_SERIAL.print(outBuffer);
/*
#ifdef ALTITUDE_ARRAY
    str.begin();

    for (i = 0; i < WAVE_COUNT - 1; ++i) {
      str.print(waveData[i], 2);
      str.print(", ");
    }

    str.print(waveData[i], 2);
    str.print("\r\n");
#endif
*/
#endif
  }
#ifdef SERIAL_DEBUG_GPS
    else {
      DEBUG_SERIAL.print("No fix found.\r\n");
  }
#endif

  GPS_SERIAL.end();
  digitalWrite(GPS_POWER_PIN, LOW);
  return (fixfnd);
}

int gpsGetYear(void) { 
  return(tinygps.date.year());
}

int gpsGetMonth(void) {
  return(tinygps.date.month());
}

int gpsGetDay(void) {   
  return(tinygps.date.day());
}

int gpsGetHour(void) {  
  return(tinygps.time.hour());
}

int gpsGetMinute(void) {
  return(tinygps.time.minute());
}

int gpsGetSecond(void) {
  return(tinygps.time.second());
}

float gpsGetLatitude(void) {
  return(tinygps.location.lat());
}

float gpsGetLongitude(void) {
  return(tinygps.location.lng());
}

float gpsGetAltitude(void) {
  return(tinygps.altitude.meters());
}

float gpsGetSpeed(void) {
  return(tinygps.speed.knots());
}

float gpsGetCourse(void) {
  return(tinygps.course.deg());
}
