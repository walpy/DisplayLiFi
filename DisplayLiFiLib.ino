#include "DisplayLiFi.h"
#include <Wire.h>

LightRecieverS11059_02DT Light;

void setup() {
  Serial.begin(9600); 
}

void loop() {
  Light.loop();
}

