// Copyright (c) 2015 IKEUCHI Yasuki

#ifndef LightRecieverS11059_02DT_h
#define LightRecieverS11059_02DT_h
#include "LightReciever.h"
#include <stdint.h>

#define DISPLAYLIFI_TRACE

class LightRecieverS11059_02DT : 
public LightReciever
{
private:
  uint16_t red, green, blue, ir;
  uint16_t max_red, max_green, max_blue;
  void getRGB();
#ifdef DISPLAYLIFI_TRACE
  void printRGB();
#endif

public:
  LightRecieverS11059_02DT();
  void loop();
};

#endif

