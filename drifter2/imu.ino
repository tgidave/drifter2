
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "imu.h"
#include "drifter2.h"

Adafruit_BNO055 bno = Adafruit_BNO055(55);

static imuVect imuOrient;


int imuPowerUp(void) {

  digitalWrite(IMU_POWER_PIN, HIGH);
  delay(1000);


  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    return(-1);
  }

  bno.setExtCrystalUse(true);
  return(0);
}

imuVect * getIMUPosition(void) {

  sensors_event_t event;

  bno.getEvent(&event);
  imuOrient.heading = event.orientation.x;
  imuOrient.pitch = event.orientation.y;
  imuOrient.roll = event.orientation.z;

#ifdef SERIAL_DEBUG_IMU
  DEBUG_SERIAL.print("IMU Heading = ");
  DEBUG_SERIAL.print(imuOrient.heading);
  DEBUG_SERIAL.print(" pitch = ");
  DEBUG_SERIAL.print(imuOrient.pitch);
  DEBUG_SERIAL.print(" roll = ");
  DEBUG_SERIAL.print(imuOrient.roll);
  DEBUG_SERIAL.print("\r\n");
#endif

  return(&imuOrient);
}

void imuPowerDown(void) {
  digitalWrite(IMU_POWER_PIN, LOW);
}
