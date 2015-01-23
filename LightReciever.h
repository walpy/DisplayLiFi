// Copyright (c) 2015 IKEUCHI Yasuki

#ifndef LightReciever_h
#define LightReciever_h
#include "Protocol.h"

class LightReciever {
  public:
    virtual void loop() = 0;
    virtual void setProtocol(Protocol *protocol) = 0;
};

#endif
