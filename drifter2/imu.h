#ifndef _IMU_H
  #define _IMU_H

#define IMU_POWER_PIN 17

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (1000)

typedef struct imuVect{
  float heading;
  float pitch;
  float roll;
} imuVect;

int imuPowerUp(void);
imuVect * getIMUPosition(void);
void imuPowerDown(void);

#endif



