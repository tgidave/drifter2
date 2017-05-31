#ifndef _IMU_H
  #define _IMU_H

#define IMU_POWER_PIN 17

/* Set the delay between fresh samples */
#define IMU_SAMPLERATE_DELAY_MS (100)

typedef struct imuVect{
    float pitch;
    float roll;
    float accelZ; 
} imuVect;

int imuPowerUp(void);
imuVect * getIMUPosition(void);
void imuPowerDown(void);

#endif



