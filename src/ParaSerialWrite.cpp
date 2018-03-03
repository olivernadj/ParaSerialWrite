/*
  ParaSerialWrite.cpp - Library for parallel serial communication ParaSerialWrite code.
  Created by Oliver Nadj, 2018.

  Inspired by https://www.arduino.cc/en/Reference/SoftwareSerial
*/


// When set, _DEBUG co-opts pins 11 and 13 for debugging with an
// oscilloscope or logic analyzer.  Beware: it also slightly modifies
// the bit times, so don't rely on it too much at high baud rates
#define _DEBUG 0
#define _DEBUG_PIN1 B010000
#define _DEBUG_PIN2 B100000


#include <Arduino.h>
#include <ParaSerialWrite.h>

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
inline void DebugPulse(uint8_t, uint8_t) {}
#endif

ParaSerialWrite::ParaSerialWrite(uint8_t pins) {
  _reg = B00000000;
  _pins = _MAX_PINS;
  if (pins < _MAX_PINS)
    _pins = pins;
  for (uint8_t i = 0; i < _pins; ++i) {
    _reg |= (1<<i);  
  }
  DDRB |= _reg;
  _portPrefix = PORTB & ~_reg;
}

void ParaSerialWrite::begin(long speed, uint8_t frameSize) {
  _frameSize = frameSize;
  // Precalculate the various delays, in number of 4-cycle delays
  _bitDelay = (F_CPU / speed);
  _tx_delay = 1;
  uint8_t tx_delay_map[6] = {36, 42, 48, 55, 62, 67};
  if (_bitDelay > tx_delay_map[_pins - 1])
    _tx_delay += (_bitDelay - tx_delay_map[_pins - 1]) / 4;
  _mask [_frameSize];
  for (uint8_t i = 0; i < _frameSize; ++i) {
    _mask [i] = 1<<i;
  }
  PORTB |= _reg;
}

bool ParaSerialWrite::write(uint16_t d0) {
  uint8_t newState;
  uint8_t i = 0;
  uint8_t oldSREG = SREG;
  cli();  // turn off interrupts for a clean txmit
  for (; ; ++i)  {
    newState = _reg;
    (d0 & _mask[i]) ? newState &= B111111 : newState &= B111110;
    PORTB = _portPrefix | newState;
    if (i == _frameSize) 
      break;
    else
      _delay_loop_2(_tx_delay);
  }
  PORTB |= _reg; // going back to idle state
  SREG = oldSREG; // turn interrupts back on
}

bool ParaSerialWrite::write(uint16_t d0, uint16_t d1) {
  uint8_t newState;
  uint8_t i = 0;
  uint8_t oldSREG = SREG;
  cli();  // turn off interrupts for a clean txmit
  for (; ; ++i)  {
    newState = _reg;
    (d0 & _mask[i]) ? newState &= B111111 : newState &= B111110;
    (d1 & _mask[i]) ? newState &= B111111 : newState &= B111101;
    PORTB = _portPrefix | newState;
    if (i == _frameSize) 
      break;
    else
      _delay_loop_2(_tx_delay);
  }
  PORTB |= _reg; // going back to idle state
  SREG = oldSREG; // turn interrupts back on
}

bool ParaSerialWrite::write(uint16_t d0, uint16_t d1, uint16_t d2) {
  uint8_t newState;
  uint8_t i = 0;
  uint8_t oldSREG = SREG;
  cli();  // turn off interrupts for a clean txmit
  for (; ; ++i)  {
    newState = _reg;
    (d0 & _mask[i]) ? newState &= B111111 : newState &= B111110;
    (d1 & _mask[i]) ? newState &= B111111 : newState &= B111101;
    (d2 & _mask[i]) ? newState &= B111111 : newState &= B111011;
    PORTB = _portPrefix | newState;
    if (i == _frameSize) 
      break;
    else
      _delay_loop_2(_tx_delay);
  }
  PORTB |= _reg; // going back to idle state
  SREG = oldSREG; // turn interrupts back on
}

bool ParaSerialWrite::write(uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3) {
  uint8_t newState;
  uint8_t i = 0;
  uint8_t oldSREG = SREG;
  cli();  // turn off interrupts for a clean txmit
  for (; ; ++i)  {
    newState = _reg;
    (d0 & _mask[i]) ? newState &= B111111 : newState &= B111110;
    (d1 & _mask[i]) ? newState &= B111111 : newState &= B111101;
    (d2 & _mask[i]) ? newState &= B111111 : newState &= B111011;
    (d3 & _mask[i]) ? newState &= B111111 : newState &= B110111;
    PORTB = _portPrefix | newState;
    if (i == _frameSize) 
      break;
    else
      _delay_loop_2(_tx_delay);
  }
  PORTB |= _reg; // going back to idle state
  SREG = oldSREG; // turn interrupts back on
}


bool ParaSerialWrite::write(uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3, uint16_t d4) {
  uint8_t newState;
  uint8_t i = 0;
  uint8_t oldSREG = SREG;
  cli();  // turn off interrupts for a clean txmit
  for (; ; ++i)  {
    newState = _reg;
    (d0 & _mask[i]) ? newState &= B111111 : newState &= B111110;
    (d1 & _mask[i]) ? newState &= B111111 : newState &= B111101;
    (d2 & _mask[i]) ? newState &= B111111 : newState &= B111011;
    (d3 & _mask[i]) ? newState &= B111111 : newState &= B110111;
    (d4 & _mask[i]) ? newState &= B111111 : newState &= B101111;
    PORTB = _portPrefix | newState;
    if (i == _frameSize) 
      break;
    else
      _delay_loop_2(_tx_delay);
  }
  PORTB |= _reg; // going back to idle state
  SREG = oldSREG; // turn interrupts back on
}



bool ParaSerialWrite::write(uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3, uint16_t d4, uint16_t d5) {
  uint8_t newState;
  uint8_t i = 0;
  uint8_t oldSREG = SREG;
  cli();  // turn off interrupts for a clean txmit
  for (; ; ++i)  {
    newState = _reg;
    (d0 & _mask[i]) ? newState &= B111111 : newState &= B111110;
    (d1 & _mask[i]) ? newState &= B111111 : newState &= B111101;
    (d2 & _mask[i]) ? newState &= B111111 : newState &= B111011;
    (d3 & _mask[i]) ? newState &= B111111 : newState &= B110111;
    (d4 & _mask[i]) ? newState &= B111111 : newState &= B101111;
    (d5 & _mask[i]) ? newState &= B111111 : newState &= B011111;
    PORTB = _portPrefix | newState;
    if (i == _frameSize) 
      break;
    else
      _delay_loop_2(_tx_delay);
  }
  PORTB |= _reg; // going back to idle state
  SREG = oldSREG; // turn interrupts back on
}


/***
bool ParaSerialWrite::write(uint16_t row[]) {
  uint16_t d0, d1, d2, d3, d4, d5 = 0xFFFF;
  switch (_reg) {
    case B111111: d5 = row[5];
    case B11111:  d4 = row[4];
    case B1111:   d3 = row[3];
    case B111:    d2 = row[2];
    case B11:     d1 = row[1];
    case B1:      d0 = row[0];
  }
  uint8_t matrix [16];
  uint8_t newState;
  uint8_t portPrefix = PORTB & ~_reg;
  uint8_t i = 0;
  for (; i < _frameSize; ++i)  {
    newState = _reg; 
    switch (_reg) {
      case B111111: if ( ! (d5 & (1<<i)) ) newState &= B011111;
      case B11111:  if ( ! (d4 & (1<<i)) ) newState &= B101111;
      case B1111:   if ( ! (d3 & (1<<i)) ) newState &= B110111;
      case B111:    if ( ! (d2 & (1<<i)) ) newState &= B111011;
      case B11:     if ( ! (d1 & (1<<i)) ) newState &= B111101;
      case B1:      if ( ! (d0 & (1<<i)) ) newState &= B111110;
    }
    matrix [i]= portPrefix | newState;
  }
  uint16_t _tx_delay = _bitDelay - 2;
  i = 0;
  uint8_t oldSREG = SREG;
  cli();  // turn off interrupts for a clean txmit
  switch (_frameSize) {
    case 16:  PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 15:  PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 14:  PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 13:  PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 12:  PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 11:  PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 10:  PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 9:   PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 8:   PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 7:   PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 6:   PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 5:   PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 4:   PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 3:   PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 2:   PORTB = matrix[i]; ++i; _delay_loop_2(_tx_delay);
    case 1:   PORTB = matrix[i]; ++i; //no need to waste the time here
  }
  PORTB |= _reg; // idle
  SREG = oldSREG; // turn interrupts back on
}
/***/
