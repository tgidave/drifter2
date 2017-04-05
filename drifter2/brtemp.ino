
#include <i2c_t3.h>

#include "TSYS01.h"

TSYS01 sensor;

void brIinitializeTemp(void) {

#ifdef SERIAL_DEBUG_TEMP
  DEBUG_SERIAL.println("brIinitializeTemp called.");
#endif

  digitalWrite(TEMP_POWER_PIN, HIGH);
  delay(1000);

//  Wire.begin();
  sensor.init();
#ifdef SERIAL_DEBUG_TEMP
  DEBUG_SERIAL.println("brIinitializeTemp return.");
#endif
}

float brGetCurrentTemp(void) {

#ifdef SERIAL_DEBUG_TEMP
  DEBUG_SERIAL.println("brIinitializeTemp reading temp.");
#endif
  sensor.read();
  return(sensor.temperature());
}

void brShutdownTemp(void) {

#ifdef SERIAL_DEBUG_TEMP
  DEBUG_SERIAL.println("brIinitializeTemp shutdown.");
#endif
//    Wire.end();
    digitalWrite(TEMP_POWER_PIN, LOW);
}




