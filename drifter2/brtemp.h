#ifndef BRTEMP_H
  #define BRTEMP_H


#define TEMP_POWER_PIN 39

void brIinitializeTemp(void);
float brGetCurrentTemp(void);
void brShutdownTemp(void);

#endif



