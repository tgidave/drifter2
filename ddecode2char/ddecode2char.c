#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../drifter2/drifter2.h"

#define BUFFSIZE 512

char buff[BUFFSIZE];

drifterData0 dData0;
drifterData1 dData1;

void convertStringToStruct(char* charPtr, char* binPtr);
uint16_t convertCharToHex(char* ptr);

int main(int argc, char **argv) {
    char *ptr = buff;
    int i;
    char buffHold;
    int dataLen;

    if (argc != 2)  {
        printf("Invalid number of arguments received!");
        exit(1);
    }

    if ((dataLen = strlen(argv[1])) != 680) {
        printf("\r\nData invalid lentgh ");
        printf("%d", dataLen);
        printf(" should be 672!\r\n");
        exit(1);
    }

    convertStringToStruct(argv[1], (char *)&dData0);

    if (dData0.ddRecordType == 0) {
        printf("Sequence number %d Record type %d\r\n", dData0.ddSeqNbr, dData0.ddRecordType);
        printf("%02d/%02d/%d %02d:%02d:%02d\r\n",
               dData0.ddMonth,
               dData0.ddDay,
               dData0.ddYear,
               dData0.ddHour,
               dData0.ddMinute,
               dData0.ddSecond);
        printf("latitude: %f\r\n", dData0.ddLatitude);
        printf("longitude: %f\r\n", dData0.ddLongitude);
        printf("altitude: %f\r\n", dData0.ddAltitude);
        printf("speed: %f\r\n", dData0.ddSpeed);
        printf("course: %f\r\n", dData0.ddCourse);
        printf("temperature: %fC\r\n", dData0.ddTemperature);

        for (i = 0; i < VECT_COUNT_0; ++i) {
            printf("vect %02d: pitch = %f roll = %f, accelZ = %f\r\n",
                   i,
                   dData0.ddVect[i].pitch,
                   dData0.ddVect[i].roll,
                   dData0.ddVect[i].accelZ);
        }
    } else {

        convertStringToStruct(argv[1], (char *)&dData1);

        printf("Sequence number %d Record type %d\r\n", dData1.ddSeqNbr, dData1.ddRecordType); 

        for (i = 0; i < VECT_COUNT_1; ++i) {
            printf("vect %02d: pitch = %f roll = %f, accelZ = %f\r\n",
                   i,
                   dData1.ddVect[i].pitch,
                   dData1.ddVect[i].roll,
                   dData1.ddVect[i].accelZ);
        }
    }
}

void convertStringToStruct(char* charPtr, char* binPtr) {
  char hByte0;
  char hByte1;
  int i = 0;
  int j = 0;

  while (*charPtr != 0) {
    hByte0 = convertCharToHex(charPtr);
    ++charPtr;
    hByte1 = convertCharToHex(charPtr);
    ++charPtr;
    *binPtr = (hByte0 << 4) | hByte1;
    ++binPtr;
  }
}

uint16_t convertCharToHex(char* ptr) {
  if ((*ptr >= '0') && (*ptr <= '9')) {
    return (*ptr - '0');
  } else if ((*ptr >= 'a') && (*ptr <= 'f')) {
    return ((*ptr - 'a') + 10);
  } else if ((*ptr >= 'A') && (*ptr <= 'F')) {
    return ((*ptr - 'A') + 10);
  }

  printf("Invalid hex char found at position %ld\r\n", ((ptr - (char*)&buff) + 1));
  exit(1);
}

