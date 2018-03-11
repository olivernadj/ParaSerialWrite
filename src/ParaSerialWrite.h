/*
  ParaSerialWrite.h - Library for parallel serial communication ParaSerialWrte code.
  Created by Oliver Nadj, 2018.

  Inspired by https://www.arduino.cc/en/Reference/SoftwareSerial
*/
#ifndef ParaSerialWrite_h
#define ParaSerialWrite_h

#include <Arduino.h>
#include <inttypes.h>

#define _MAX_PINS 6

class ParaSerialWrite
{
  public:
    ParaSerialWrite(uint8_t pins);
    void begin(long speed, uint8_t frameSize);
    //bool write(uint16_t row[]);
    bool write(uint16_t d0);
    bool write(uint16_t d0, uint16_t d1);
    bool write(uint16_t d0, uint16_t d1, uint16_t d2);
    bool write(uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3);
    bool write(uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3, uint16_t d4);
    bool write(uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3, uint16_t d4, uint16_t d5);
  private:
    uint8_t _reg;
    uint8_t _pins;
    uint16_t _tx_delay3;
    uint16_t _tx_delay4;
    uint8_t _frameSize;
    uint8_t _portPrefix;
    uint16_t _mask [];
    void delayCalc(uint16_t i);
};

#endif
