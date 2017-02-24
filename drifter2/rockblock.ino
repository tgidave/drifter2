
#include <IridiumSBD.h>

#include "drifter2.h"
#include "rockblock.h"

IridiumSBD isbd(ROCKBLOCK_SERIAL, ROCKBLOCK_SLEEP_PIN);

int transmitGPSFix(drifterData *ddPtr, int ddLen) {

  int rc;

  // Setup the RockBLOCK
#ifdef SERIAL_DEBUG_ROCKBLOCK
  isbd.attachConsole(DEBUG_SERIAL);
  isbd.attachDiags(DEBUG_SERIAL);
#endif
  isbd.setPowerProfile(1);

  digitalWrite(ROCKBLOCK_POWER_PIN, HIGH);
  delay(1000);
  ROCKBLOCK_SERIAL.begin(ROCKBLOCK_BAUD);
  // Step 3: Start talking to the RockBLOCK and power it up
#ifdef SERIAL_DEBUG_ROCKBLOCK
  DEBUG_SERIAL.flush();
  DEBUG_SERIAL.println("RockBLOCK begin");
  DEBUG_SERIAL.flush();
#endif
  //ROCKBLOCK_SERIAL.listen();

  if (isbd.begin() == ISBD_SUCCESS) {
#ifdef SERIAL_DEBUG_ROCKBLOCK
    DEBUG_SERIAL.flush();
    DEBUG_SERIAL.print("Transmitting address=");
    DEBUG_SERIAL.print((long)ddPtr);
    DEBUG_SERIAL.print(" length=");
    DEBUG_SERIAL.print(ddLen);
    DEBUG_SERIAL.print("\r\n");

    DEBUG_SERIAL.flush();
#endif
    rc = isbd.sendSBDBinary((const uint8_t *)ddPtr, ddLen);
#ifdef SERIAL_DEBUG_ROCKBLOCK
    DEBUG_SERIAL.flush();
    if (rc == 0) {
      DEBUG_SERIAL.print("Good return code from send!\r\n");
      DEBUG_SERIAL.flush();
    } else {
      DEBUG_SERIAL.print("Bad return code from send = ");
      DEBUG_SERIAL.print(rc);
      DEBUG_SERIAL.print("\r\n");
      DEBUG_SERIAL.flush();
    }
#endif
  }

  isbd.sleep();
  ROCKBLOCK_SERIAL.end();
  digitalWrite(ROCKBLOCK_POWER_PIN, LOW);
  return (0);
}
