
#include "IridiumSBD.h"

#include "drifter2.h"
#include "rockblock.h"

IridiumSBD isbd(ROCKBLOCK_SERIAL, ROCKBLOCK_SLEEP_PIN);

#ifdef SEND_2_RECORDS
int transmitGPSFix(drifterData0 *ddPtr0, int ddLen0, drifterData1 *ddPtr1, int ddLen1) {
#else
int transmitGPSFix(drifterData0 *ddPtr0, int ddLen0) {
#endif
  int rc;

  // Setup the RockBLOCK
#ifdef SERIAL_DEBUG_ROCKBLOCK
  isbd.attachConsole(DEBUG_SERIAL);
  isbd.attachDiags(DEBUG_SERIAL);
#endif
  isbd.setPowerProfile(1);

#ifdef SERIAL_DEBUG_ROCKBLOCK
  DEBUG_SERIAL.flush();
  DEBUG_SERIAL.println("Powering up RockBLOCK");
  DEBUG_SERIAL.flush();
#endif

#ifdef MANAGE_ROCKBLOCK_POWER
  digitalWrite(ROCKBLOCK_POWER_PIN, HIGH);
#endif

#ifdef SERIAL_DEBUG_ROCKBLOCK
  DEBUG_SERIAL.flush();
  DEBUG_SERIAL.println("RockBLOCK powered up");
  DEBUG_SERIAL.flush(); 
#endif

  delay(1000);
  ROCKBLOCK_SERIAL.begin(ROCKBLOCK_BAUD);
  // Step 3: Start talking to the RockBLOCK and power it up
#ifdef SERIAL_DEBUG_ROCKBLOCK
  DEBUG_SERIAL.flush();
  DEBUG_SERIAL.println("RockBLOCK begin");
  DEBUG_SERIAL.flush();
#endif
  //ROCKBLOCK_SERIAL.listen();

  if ((rc =isbd.begin()) == ISBD_SUCCESS) {
#ifdef SERIAL_DEBUG_ROCKBLOCK
      DEBUG_SERIAL.flush();
      DEBUG_SERIAL.print("Transmitting address=");
      DEBUG_SERIAL.print((long)ddPtr0);
      DEBUG_SERIAL.print(" length=");
      DEBUG_SERIAL.print(ddLen0);
      DEBUG_SERIAL.print("\r\n");

      DEBUG_SERIAL.flush();
#endif
      rc = isbd.sendSBDBinary((const uint8_t *)ddPtr0, ddLen0);
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

#ifdef SEND_2_RECORDS
  #ifdef SERIAL_DEBUG_ROCKBLOCK
      DEBUG_SERIAL.flush();
      DEBUG_SERIAL.print("Transmitting address=");
      DEBUG_SERIAL.print((long)ddPtr1);
      DEBUG_SERIAL.print(" length=");
      DEBUG_SERIAL.print(ddLen1);
      DEBUG_SERIAL.print("\r\n");
      DEBUG_SERIAL.flush();
  #endif
      rc = isbd.sendSBDBinary((const uint8_t *)ddPtr1, ddLen1);
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
#endif
  } else {
#ifdef SERIAL_DEBUG_ROCKBLOCK
    DEBUG_SERIAL.print("Bad return code from begin = ");
    DEBUG_SERIAL.print(rc);
    DEBUG_SERIAL.print("\r\n");
    DEBUG_SERIAL.flush();
#endif
  }

  isbd.sleep();
  ROCKBLOCK_SERIAL.end();

#ifdef MANAGE_ROCKBLOCK_POWER
  digitalWrite(ROCKBLOCK_POWER_PIN, LOW);
#endif
  return (0);
}



