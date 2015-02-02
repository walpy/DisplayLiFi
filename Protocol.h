// Copyright (c) 2015 IKEUCHI Yasuki

#ifndef Protocol_h
#define Protocol_h

#include <stdint.h>

typedef void (*CallbackCharacterRecieved)(char c);
typedef void (*CallbackError)(int errorCode);
#define DISPLAY_LIFI_ERROR_NONE 0
#define DISPLAY_LIFI_ERROR_PARITY 1

class Protocol {
  private:
    uint8_t buffer_;
    CallbackCharacterRecieved onCharacterRecieved_;
    CallbackError onError_;
    void callOnCharacterRecieved(char c);
    void callOnError(int errorCode);
  public:
    Protocol();
    void pushRGB(uint8_t rgb);
    void setOnCharacterRecieved(CallbackCharacterRecieved callback);
    void setOnError(CallbackError callback);
    uint8_t getBufferForTest();
};

#endif
