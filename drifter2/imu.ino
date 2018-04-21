
#include <Adafruit_Sensor.h>
#include "Adafruit_BNO055.h"
#include "imumaths.h"

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

    int i;
  sensors_event_t event;

  bno.getEvent(&event);
  imuOrient.pitch = event.orientation.y;
  imuOrient.roll = event.orientation.z;

#ifdef SERIAL_DEBUG_IMU
//  DEBUG_SERIAL.print("IMU accelZ = ");
//  DEBUG_SERIAL.print(imuOrient.heading);
  DEBUG_SERIAL.print(" pitch = ");
  DEBUG_SERIAL.print(imuOrient.pitch);
  DEBUG_SERIAL.print(" roll = ");
  DEBUG_SERIAL.print(imuOrient.roll);
#endif

#ifdef AVERAGE_ACCELZ
  imuOrient.accelZ = 0;

  for (i = 0; i < ACCELZ_AVG_SAMPLE_COUNT; ++i) {
      imu::Vector<3> accel  = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
      imuOrient.accelZ += accel.z();

      if (ACCELZ_AVG_SAMPLERATE_DELAY_MS >= BNO055_SAMPLERATE_DELAY_MS) {
          delay(ACCELZ_AVG_SAMPLERATE_DELAY_MS); 
      } else {
          delay(BNO055_SAMPLERATE_DELAY_MS);
      }
  }

  imuOrient.accelZ = imuOrient.accelZ / ACCELZ_AVG_SAMPLE_COUNT;
#else
  imu::Vector<3> accel  = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  imuOrient.accelZ = accel.z();
#endif

#ifdef SERIAL_DEBUG_IMU
  DEBUG_SERIAL.print(" accelZ = ");
  DEBUG_SERIAL.print(imuOrient.accelZ);
  DEBUG_SERIAL.print("\r\n"); 
#endif

  return (&imuOrient);
}

void imuPowerDown(void) {
  digitalWrite(IMU_POWER_PIN, LOW);
}



