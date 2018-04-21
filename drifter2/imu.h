#ifndef _IMU_H
  #define _IMU_H

#define IMU_POWER_PIN 17

#define BNO055_SAMPLERATE_DELAY_MS (100)

/* Set the delay between fresh samples */
#define IMU_SAMPLERATE_DELAY_MS (1000)

typedef struct imuVect{
    float pitch;
    float roll;
    float accelZ; 
} imuVect;

int imuPowerUp(void);
imuVect * getIMUPosition(void);
void imuPowerDown(void);

#endif



