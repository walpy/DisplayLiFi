// Copyright (c) 2015 IKEUCHI Yasuki

#ifndef DisplayLiFi_h
#define DisplayLiFi_h
#include "LightReciever.h"
#include "LightRecieverS11059_02DT.h"

class DisplayLiFi
{
public:
  DisplayLiFi();
  void setLightReciever(LightReciever& lightReciever);
};

class Protocol
{
public:
  void pushData(unsigned char data);
};
#endif
