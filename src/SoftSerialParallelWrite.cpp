/*
  SoftSerialParallelWrite.cpp - Library for parallel serial communication SoftSerialParallelWrite code.
  Created by Oliver Nadj, 2018.

  Inspired by https://www.arduino.cc/en/Reference/SoftwareSerial
*/


// When set, _DEBUG co-opts pins 11 and 13 for debugging with an
// oscilloscope or logic analyzer.  Beware: it also slightly modifies
// the bit times, so don't rely on it too much at high baud rates
#define _DEBUG 1
#define _DEBUG_PIN1 B010000
#define _DEBUG_PIN2 B100000


#include <Arduino.h>
#include <SoftSerialParallelWrite.h>

//
// Debugging
//
// This function generates a brief pulse
// for debugging or measuring on an oscilloscope.
#if _DEBUG
inline void DebugPulse(uint8_t pin, uint16_t count)
{
  if (!( DDRB & pin ))
    DDRB |= pin;
  while (count--)
  {
    PORTB ^= pin;
    if ((count & B1111) == B1111)
      _delay_loop_2(2);
    PORTB ^= pin;
  }
}
#else
inline void DebugPulse(uint8_t, uint16_t) {}
#endif


// This functions is returns 3 and 4  
void SoftSerialParallelWrite::delayCalc(uint16_t i) {
  uint16_t x = 0, threes = 1, fours = 1;
  if (i >= 7) {
    i -= 7; //one cycles for both 3 and 4 is mandatory
  } else {
    i = 0;
  }

  // this logic is make sure we keep the accuracy above 6
  if (i >= 6) {
    x = i - 6; 
    fours += (x / 12) * 3;
    x = (x % 12) + 6;
  } else {
    x = i;
  }
  switch (x) {
    case 2:
      ++ threes;
      break;
    case 5:
      threes += 2;
      break;
    case 4:
    case 8:
    case 12:
    case 16:
    case 20:
      fours += x / 4;
      break;
    case 3:
    case 6:
    case 9:
    case 15:
    case 18:
      threes += x / 3;
      break;
    case 7:
      ++ threes; ++fours;
      break;
    case 10:
      threes += 2; ++fours;
      break;
    case 11:
      ++threes; fours += 2;
      break;
    case 13:
      threes += 3; ++fours;
      break;
    case 14:
      threes += 2; fours += 2;
      break;
    case 17:
      threes += 3; fours += 2;
      break;
    case 19:
      threes += 5; fours += 2;
      break;
      
  }
  _tx_delay3 = threes;
  _tx_delay4 = fours;
}


SoftSerialParallelWrite::SoftSerialParallelWrite(uint8_t pins) {
  // calculate register, pin and port prefix masks
  _reg = B00000000;
  _pins = _MAX_PINS;
  if (pins < _MAX_PINS)
    _pins = pins;
  for (uint8_t i = 0; i < _pins; ++i) {
    _reg |= (1<<i);  
  }
  DDRB |= _reg; // set up registers for output
  _portPrefix = PORTB & ~_reg;
}

void SoftSerialParallelWrite::begin(long speed, uint8_t frameSize) {
  _frameSize = frameSize; // number of bits in one frame included start, data, parity and stop

  // Precalculate the various delays, in number of 4-cycle and 3-cycle of delays
  float tx_delay_map[6] = {32, 37, 44, 48, 54, 58}; // numbers of cycles requred for a write loop
  float cycles = (F_CPU / speed) - (tx_delay_map[_pins - 1]);
  if (cycles < 0)
    cycles = 0;
  delayCalc( (int) cycles );
  DebugPulse(_DEBUG_PIN2, (int) cycles);
  DebugPulse(_DEBUG_PIN1, _tx_delay3);
  DebugPulse(_DEBUG_PIN2, _tx_delay4);

  // speed tweak. to save as many operation as possible in write loops
  _mask [_frameSize]; //
  for (uint8_t i = 0; i < _frameSize; ++i) {
    _mask [i] = 1<<i;
  }

  PORTB |= _reg; // may not necessary since write(0x00, ..) would do the job;
}

bool SoftSerialParallelWrite::write(uint16_t d0) {
  uint8_t newState; // holds current stat value and make possible one step register write operation
  uint8_t i = 0;
  uint8_t stop = _frameSize - 1;

  // turn off interrupts for a clean txmit
  uint8_t oldSREG = SREG; 
  cli();

  // do the actual transmission
  for (; ; ++i)  {
    newState = _reg;
    (d0 & _mask[i]) ? newState &= B111111 : newState &= B111110;
    PORTB = _portPrefix | newState;
    if (i == stop) 
      break; //delay does not happen after last bit
    else
      // daleys happen all the time. and addition if condition would take longer than actual delay loop
      _delay_loop_1(_tx_delay3);
      _delay_loop_2(_tx_delay4);
  }
  SREG = oldSREG; // turn interrupts back on
}

bool SoftSerialParallelWrite::write(uint16_t d0, uint16_t d1) {
  uint8_t newState;
  uint8_t i = 0;
  uint8_t stop = _frameSize - 1;
  uint8_t oldSREG = SREG;
  cli();
  for (; ; ++i)  {
    newState = _reg;
    (d0 & _mask[i]) ? newState &= B111111 : newState &= B111110;
    (d1 & _mask[i]) ? newState &= B111111 : newState &= B111101;
    PORTB = _portPrefix | newState;
    if (i == stop) 
      break;
    else
      _delay_loop_1(_tx_delay3);
      _delay_loop_2(_tx_delay4);
  }
  SREG = oldSREG;
}

bool SoftSerialParallelWrite::write(uint16_t d0, uint16_t d1, uint16_t d2) {
  uint8_t newState;
  uint8_t i = 0;
  uint8_t stop = _frameSize - 1;
  uint8_t oldSREG = SREG;
  cli();
  for (; ; ++i)  {
    newState = _reg;
    (d0 & _mask[i]) ? newState &= B111111 : newState &= B111110;
    (d1 & _mask[i]) ? newState &= B111111 : newState &= B111101;
    (d2 & _mask[i]) ? newState &= B111111 : newState &= B111011;
    PORTB = _portPrefix | newState;
    if (i == stop) 
      break;
    else
      _delay_loop_1(_tx_delay3);
      _delay_loop_2(_tx_delay4);
  }
  SREG = oldSREG;
}

bool SoftSerialParallelWrite::write(uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3) {
  uint8_t newState;
  uint8_t i = 0;
  uint8_t stop = _frameSize - 1;
  uint8_t oldSREG = SREG;
  cli();
  for (; ; ++i)  {
    newState = _reg;
    (d0 & _mask[i]) ? newState &= B111111 : newState &= B111110;
    (d1 & _mask[i]) ? newState &= B111111 : newState &= B111101;
    (d2 & _mask[i]) ? newState &= B111111 : newState &= B111011;
    (d3 & _mask[i]) ? newState &= B111111 : newState &= B110111;
    PORTB = _portPrefix | newState;
    if (i == stop) 
      break;
    else
      _delay_loop_1(_tx_delay3);
      _delay_loop_2(_tx_delay4);
  }
  SREG = oldSREG;
}


bool SoftSerialParallelWrite::write(uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3, uint16_t d4) {
  uint8_t newState;
  uint8_t i = 0;
  uint8_t stop = _frameSize - 1;
  uint8_t oldSREG = SREG;
  cli();
  for (; ; ++i)  {
    newState = _reg;
    (d0 & _mask[i]) ? newState &= B111111 : newState &= B111110;
    (d1 & _mask[i]) ? newState &= B111111 : newState &= B111101;
    (d2 & _mask[i]) ? newState &= B111111 : newState &= B111011;
    (d3 & _mask[i]) ? newState &= B111111 : newState &= B110111;
    (d4 & _mask[i]) ? newState &= B111111 : newState &= B101111;
    PORTB = _portPrefix | newState;
    if (i == stop) 
      break;
    else
      _delay_loop_1(_tx_delay3);
      _delay_loop_2(_tx_delay4);
  }
  SREG = oldSREG;
}



bool SoftSerialParallelWrite::write(uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3, uint16_t d4, uint16_t d5) {
  uint8_t newState;
  uint8_t i = 0;
  uint8_t stop = _frameSize - 1;
  uint8_t oldSREG = SREG;
  cli();
  for (; ; ++i)  {
    newState = _reg;
    (d0 & _mask[i]) ? newState &= B111111 : newState &= B111110;
    (d1 & _mask[i]) ? newState &= B111111 : newState &= B111101;
    (d2 & _mask[i]) ? newState &= B111111 : newState &= B111011;
    (d3 & _mask[i]) ? newState &= B111111 : newState &= B110111;
    (d4 & _mask[i]) ? newState &= B111111 : newState &= B101111;
    (d5 & _mask[i]) ? newState &= B111111 : newState &= B011111;
    PORTB = _portPrefix | newState;
    if (i == stop) 
      break;
    else
      _delay_loop_1(_tx_delay3);
      _delay_loop_2(_tx_delay4);
  }
  SREG = oldSREG;
}
