#include <SoftSerialParallelWrite.h>


SoftSerialParallelWrite mySerial(2);
signed int leftSpeed = 40;
signed int rightSpeed = 60;

void setup() { 
  mySerial.begin(26315, 12);
}

//0xC00 2 stop bits and <<1 for the start
uint16_t startSignal = 0xFC00 | (256 << 1); 
uint16_t endSignal = 0xFC00 | (85 << 1);

void loop() {
  mySerial.write(startSignal, startSignal);
  mySerial.write(
    0xFC00 | ((leftSpeed & 0xFF) << 1),
    0xFC00 | ((rightSpeed & 0xFF) << 1)
  );
  mySerial.write(
    0xFC00 | (((leftSpeed >> 8) & 0xFF) << 1),
    0xFC00 | (((rightSpeed >> 8) & 0xFF) << 1)
  );
  mySerial.write(
    0xFC00 | ((leftSpeed & 0xFF) << 1),
    0xFC00 | ((rightSpeed & 0xFF) << 1)
  );
  mySerial.write(
    0xFC00 | (((leftSpeed >> 8) & 0xFF) << 1),
    0xFC00 | (((rightSpeed >> 8) & 0xFF) << 1)
  );
  mySerial.write(endSignal, endSignal);
  delayMicroseconds(300);
}

void blink() {

}