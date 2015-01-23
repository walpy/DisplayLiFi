// Copyright (c) 2015 IKEUCHI Yasuki

#ifndef DisplayLiFi_h
#define DisplayLiFi_h
#include "LightReciever.h"
#include "LightRecieverS11059_02DT.h"

class DisplayLiFi {
  private:
    LightReciever *lightReciever_;
  public:
    DisplayLiFi();
    void setLightReciever(LightReciever *lightReciever);
};

#endif
