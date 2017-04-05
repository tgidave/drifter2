#include "drifter2.h"
#include "atlastemp.h"

float currentTemp = 0;

void atlasIinitializeTemp(void) {
  int timeoutCount;
  String tempString = "";

  pinMode(TEMP_POWER_PIN, OUTPUT);
  digitalWrite(TEMP_POWER_PIN, HIGH);
  delay(1000);
  TEMP_SERIAL.begin(TEMP_BAUD);

#ifdef SERIAL_DEBUG_TEMP
  DEBUG_SERIAL.print("Resetting temp sensor to factory defaults.\r\n");
#endif

//  tempString = "";
  timeoutCount = 0;

  TEMP_SERIAL.print("Factory\r");
//  TEMP_SERIAL.write(13);

  while ((tempString = TEMP_SERIAL.readStringUntil(13)) != "*RE") {
#ifdef SERIAL_DEBUG_TEMP
    if (tempString == "") {
      DEBUG_SERIAL.print("Serial timeout\r\n");
    } else {
      DEBUG_SERIAL.print(tempString);
      DEBUG_SERIAL.print("\r\n");
    }
#endif
    if (tempString == "*ER") {
      TEMP_SERIAL.print("Factory\r");
    }

    tempString = "";

    if (++timeoutCount >= 10) {
#ifdef SERIAL_DEBUG_TEMP
      DEBUG_SERIAL.print("Could not reset temp controller!!!\r\nContinuing.\r\n");
#endif
      break;
    }

  }

  tempString = "";
  timeoutCount = 0;

#ifdef SERIAL_DEBUG_TEMP
  DEBUG_SERIAL.print("Turning off continuous mode.\r\n");
#endif

  TEMP_SERIAL.print("C,0\r");

  while (1) {

    tempString = TEMP_SERIAL.readStringUntil(13);

#ifdef SERIAL_DEBUG_TEMP
    if (tempString == "") {
      DEBUG_SERIAL.print("Serial timeout\r\n");
    } else {
      DEBUG_SERIAL.print(tempString);
      DEBUG_SERIAL.print("\r\n");
    }
#endif
    if (tempString == "*OK") {
      break;
    } else {
      if (tempString == "*ER")  {
        TEMP_SERIAL.print("C,0\r");
      }
    }

    tempString = "";

    if (++timeoutCount >= 10) {
#ifdef SERIAL_DEBUG_TEMP
      DEBUG_SERIAL.print("Could not stop continuous mode!!!\r\nContinuing.\r\n");
#endif
      break;
    }
  }
  TEMP_SERIAL.end();
  digitalWrite(TEMP_POWER_PIN, LOW);
}


float atlasGetCurrentTemp(void) {

  int retryCount = 0;
  String tempString = "";

  digitalWrite(TEMP_POWER_PIN, HIGH);
  delay(1000);
  TEMP_SERIAL.begin(TEMP_BAUD);
  TEMP_SERIAL.print("R\r");
  currentTemp = 0;

  while (retryCount < 10) {
    tempString = TEMP_SERIAL.readStringUntil(13);

#ifdef SERIAL_DEBUG_TEMP
    if (tempString == "") {
      DEBUG_SERIAL.print("Serial Timeout.\r\n");
    } else {
      DEBUG_SERIAL.print(tempString);
      DEBUG_SERIAL.print("\r\n");
    }
#endif

    if (tempString == "*ER") {
      TEMP_SERIAL.print("R\r");
      ++retryCount;
      continue;
    }

    if ((currentTemp = tempString.toFloat()) != 0) {
#ifdef SERIAL_DEBUG_TEMP
      DEBUG_SERIAL.print("Current temperature is ");
      DEBUG_SERIAL.print(currentTemp);
      DEBUG_SERIAL.print("\r\n");
#endif
      break;
    }
    ++retryCount;
  }

#ifdef SERIAL_DEBUG_TEMP
  if (currentTemp == 0) {
    DEBUG_SERIAL.print("Could not get current temperature!!!\r\nContinuing.\r\n");
  }
#endif

  TEMP_SERIAL.end();
  digitalWrite(TEMP_POWER_PIN, LOW);
  return (currentTemp);
}
