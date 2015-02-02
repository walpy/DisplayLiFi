// Copyright 2015 Yasuki IKEUCHI

#include "Protocol.h"

/**
 * Protocol:
 * It based on 'start-stop system' of Serial communication.
 * It accepts 7 bits characters only(<127).
 *
 * Example:
 * RGB = 111
 * 110 : start bits
 * xx1 : 1st two bits (xx_____)
 * xx1 : 2nd two bits (__xx___)
 * xx1 : 3rd two bits (____xx_)
 * xP1 : 4th one bit and even pertity (______x)
 **/

const int START_BIT = 6; // 0b110

Protocol::Protocol() :
  onCharacterRecieved_(0),
  onError_(0) {
}

void Protocol::setOnCharacterRecieved(CallbackCharacterRecieved callback) {
  onCharacterRecieved_ = callback;
}

void Protocol::setOnError(CallbackError callback) {
  onError_ = callback;
}

int even_parity_xor(unsigned int a) {
  // See http://d.hatena.ne.jp/houmei/20120531/1338459599
  unsigned int b;
  b = a >> 4;
  a = a ^ b;
  b = a >> 2;
  a = a ^ b;
  b = a >> 1;
  a = a ^ b;
  return a & 1;
}

void Protocol::callOnCharacterRecieved(char c) {
  if (onCharacterRecieved_)
    onCharacterRecieved_(c);
}

void Protocol::callOnError(int errorCode) {
  if (onError_)
    onError_(errorCode);
}

void Protocol::pushRGB(uint8_t data) {
  if (data == START_BIT) {
    buffer_ = 1;
    return;
  }
  if (buffer_ == 0) return;

  buffer_ = buffer_ << 1;

  buffer_ |= (data >> 2) & 1;
  if (buffer_ & 0x80) {
    uint8_t parityExpected = (data >> 1) & 1;
    buffer_ = buffer_ & 0x7F;
    uint8_t parityActual = even_parity_xor(buffer_);
    if (parityExpected == parityActual) {
      callOnCharacterRecieved(buffer_);
    } else {
      callOnError(DISPLAY_LIFI_ERROR_PARITY);
    }
  } else  {
    buffer_ = buffer_ << 1;
    buffer_ |= (data >> 1) & 1;
  }
}

uint8_t Protocol::getBufferForTest() {
  return buffer_;
}
