
#define TEMP_SERIAL Serial4
#define TEMP_SERIAL_EVENT serialEvent4
#define TEMP_RX 31
#define TEMP_TX 32
#define TEMP_BAUD 9600
#define TEMP_POWER_PIN 30

void atlasIinitializeTemp(void);
float atlasGetCurrentTemp(void);

