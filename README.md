# SoftSerialParallelWrite
Parallel Software Serial Write Library for Arduino.

SoftSerialParallelWrite let you write up to 6 serial ports. Write ONLY, with same frame size and baud rate. 
It is particularly useful when you want to transmit UART data to many receivers, simultaneously.

SoftSerialParallelWrite is capable of running up to following speed on 16 MHz AVR:
```
6 port max speed 237000Hz 4.22us
5 port max speed 260000Hz 3.84us
4 port max speed 288300Hz 3.47us
3 port max speed 329800Hz 3.03us
2 port max speed 376400Hz 2.66us
1 port max speed 438400Hz 2.28us
```
## Limitations
To earn that speed with simultaneous communication it provide very simple functionally with limitations. 
 - It implements [PortManipulation][PortManipulation] on PORTB which maps to Arduino digital pins 8 to 13 (The two high bits (6 & 7) map to the crystal pins and are not usable). Therefor the used pins are "hardcoded" and started with pin 8 and allocated incrementally up to 6 ports. Eg.: In case you want to use 2 ports, than pins 8, 9 will be used, and pins 10, 11, 12, 13 can be used freely.
 - It does not support serial read. Only transmission is possible. Read functionality require different approach, since no guaranty that the frames would arrive in same time.
 - You have to construct your UART frame: start bit, data bits, parity and stop bits. Basic understanding of bitwise operations and UART frame is useful.
 - Timing accuracy is depends on the divisibility of CPU cycles by [_delay_loop_1][_delay_loop_1] and [_delay_loop_2][_delay_loop_2] loops (3 and 4 cycles resctably). 

### Pin assignments
| Number of ports | Allocated pins       | Free to other use |
| --------------: | --------------------:| -----------------:|
|               1 |                    8 |  9, 10, 11, 12, 13|
|               2 |                 8, 9 |     10, 11, 12, 13|
|               3 |             8, 9, 10 |         11, 12, 13|
|               4 |         8, 9, 10, 11 |             12, 13|
|               5 |     8, 9, 10, 11, 12 |                 13|
|               6 | 8, 9, 10, 11, 12, 13 |                   |


## Basic Usage
`SoftSerialParallelWrite mySerial(2);`  
Create the SoftSerialParallelWrite object. Only one SoftSerialParallelWrite can be used, with the fixed pin assignments shown above.  
`mySerial.begin(baud_rate, frame_size);`  
Initialize the port to communicate at a specific baud rate, with the specific frame size. The frame size includes start bit, data bits, parity and stop bits.  
`mySerial.write(frameA, ...);`  
Transmit up to 6 UART frame. How to compose UART frame shown below.  
`that's all :)`

### How to compose UART frame
UART (Universal Asynchronous Receiver/Transmitter) device send a `start bit`, five to nine `data bits`, least-significant-first, an optional `parity bit` and then one, one and a half or two `stop bits`. 
 - The start bit is the opposite polaity of the data-line's idle state.
 - The stop bit is the data-line's idle state, and provides a delay before the next character can start. 
 - 0 data bit is the opposite polaity of the data-line's idle state.
 - 1 data bit is the data-line's idle state.

![UART frame format](https://github.com/olivernadj/SoftSerialParallelWrite/raw/master/UART_frame_format.jpg)

SoftSerialParallelWrite Library uses `uint16_t` (unsigned 16-bit integer) to keep the whole frame. Since the data transmits least-significant-first we need to left shift `<<` the data and put the start (0) bit in the least-significant position. Somehow like this: `uint16_t data = 0xA5 << 1;` `B0000000010100101 << 1; becomes: B0000000101001010`. 
We also need to put the stop and parity bits on the most significant position. Somehow like this (two stop bit on 8 bit long data): `data = 0x600 | data;`  `B11000000000 | B0000000101001010; becomes: B0000011101001010`.

## Example Program
```c
#include <SoftSerialParallelWrite.h>
SoftSerialParallelWrite mySerial(2);
void setup() {
  mySerial.begin(26315, 12);
}
void loop() {
  mySerial.write(0xFFFF, 0xAAAA);
  delayMicroseconds(300);
  mySerial.write(0xAAAA, 0xFFFF);
  delayMicroseconds(300);
  mySerial.write(0xFFFF, 0xFFFF);
  delayMicroseconds(300);
  mySerial.write(0xAAAA, 0xFFFF);
  delayMicroseconds(300);
}
```

## Accuracy is around 98% and 99.9%
The timing accuracy is vary between 98% and 99.9% according to my measurements. It depends mostly on the divisibility of CPU cycles by [_delay_loop_1][_delay_loop_1] and [_delay_loop_2][_delay_loop_2] loops (3 and 4 cycles resctably). The iteration time of sending one bit is vary between 32 and 58 CPU cycles, depends on the number of the ports.

Eg.: the accuracy with 6 ports and 230400 baud rate on 16 MHz AVR would be calculated following:
```
F_CPU = 16000000; // 16 MHz AVR
speed = 230400; // baud rate
delayCycles = F_CPU / speed;
delayCycles = 16000000 / 230400; // = 69.444 to int ~= 69
//Therefor the effective speed should be 231884Hz and the accuracy around 99.35%
```
In real there are 2 additional complications.
1) The delayCycles should be deducted by the numbers of cycles required for an UART bit writing loop. For 6 ports it is 58 CPU cycles. 
2) And the actual delayCycles need to be implemented by the combination of 3 and 4 cycles (_delay_loop_1, _delay_loop_2). 
```
//To follow the previous example it would looks like:
delayCycles = delayCycles - 58; // => 69 - 58 = 11 => 1 * 3 + 2 * 4;
//it means the accuracy will remain 99.35% since 11 is possible combinations of threes and fours. 
```

For the measurement I used [CY7C68013A Mini Board][CY7C68013A].
![Accuracy with 6 ports with 230400 baud rate](https://github.com/olivernadj/SoftSerialParallelWrite/raw/master/accuracy.png)


## Other Serial Port Options
1) HardwareSerial - Best performance. Always use this first, if available! Arduino Mega has 3 extra HardwareSerial ports. Arduino Uno has none.
2) AltSoftSerial - Can simultaneously transmit and receive. Minimal interference with simultaneous use of HardwareSerial and other libraries. Consumes a 16 bit timer (and will not work with any libraries which need that timer) and disables some PWM pins. Can be sensitive to interrupt usage by other libraries.
3) SoftwareSerial(formerly "NewSoftSerial") - Can have multiple instances on almost any pins, but only 1 can be active at a time. Can not simultaneously transmit and receive. Can interfere with other libraries or HardwareSerial if used at slower baud rates. Can be sensitive to interrupt usage by other libraries.

[//]: # (References)
[PortManipulation]:<https://www.arduino.cc/en/Reference/PortManipulation>
[_delay_loop_1]:<https://www.microchip.com/webdoc/AVRLibcReferenceManual/group__util__delay__basic_1ga4e3957917c4c447d0f9166dac881b4e3.html>
[_delay_loop_2]:<https://www.microchip.com/webdoc/AVRLibcReferenceManual/group__util__delay__basic_1ga74a94fec42bac9f1ff31fd443d419a6a.html>
[CY7C68013A]:<https://sigrok.org/wiki/Lcsoft_Mini_Board>