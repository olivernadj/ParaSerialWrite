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
 - It implements [PortManipulation][PortManipulation] on PORTB which maps to Arduino digital pins 8 to 13 The two high bits (6 & 7) map to the crystal pins and are not usable. The settings is how many port you want to use, and not the list of ports. Eg.: in case you want use 4 ports, than digital pins 8, 9, 10, 11 will be used, and so on.
 - It does not support serial read. Only transmission is possible. Read functionality require different approach, since no guaranty that the frames would arrive in same time.
 - You have to construct your UART frame: start bit, data bits, parity and stop bits. Basic understanding of bitwise operations and UART frame is useful.
 - Timing accuracy is depends on the divisibility of CPU cycles by [_delay_loop_1][_delay_loop_1] and [_delay_loop_2][_delay_loop_2] loops (3 and 4 cycles respectably).

## Accuracy
Accuracy with 6 ports with 230400 baud rate
![Accuracy with 6 ports with 230400 baud rate](https://github.com/olivernadj/SoftSerialParallelWrite/raw/master/accuracy.png)

## Other Serial Port Options
1) HardwareSerial - Best performance. Always use this first, if available! Arduino Mega has 3 extra HardwareSerial ports. Arduino Uno has none.
2) AltSoftSerial - Can simultaneously transmit and receive. Minimal interference with simultaneous use of HardwareSerial and other libraries. Consumes a 16 bit timer (and will not work with any libraries which need that timer) and disables some PWM pins. Can be sensitive to interrupt usage by other libraries.
3) SoftwareSerial(formerly "NewSoftSerial") - Can have multiple instances on almost any pins, but only 1 can be active at a time. Can not simultaneously transmit and receive. Can interfere with other libraries or HardwareSerial if used at slower baud rates. Can be sensitive to interrupt usage by other libraries.



[//]: # (References)
[PortManipulation]:<https://www.arduino.cc/en/Reference/PortManipulation>
[_delay_loop_1]:<https://www.microchip.com/webdoc/AVRLibcReferenceManual/group__util__delay__basic_1ga4e3957917c4c447d0f9166dac881b4e3.html>
[_delay_loop_2]:<https://www.microchip.com/webdoc/AVRLibcReferenceManual/group__util__delay__basic_1ga74a94fec42bac9f1ff31fd443d419a6a.html>