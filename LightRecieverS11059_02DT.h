// Copyright (c) 2015 IKEUCHI Yasuki

#ifndef LightRecieverImpl_h
#define LightRecieverImpl_h
#include "LightReciever.h"
#include "arduino.h"

class LightRecieverS11059_02DT : public LightReciever
{
public:
  LightRecieverS11059_02DT();
  unsigned int resume();
};

#endif
