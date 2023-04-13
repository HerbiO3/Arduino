#ifndef UvSensor_h
#define UvSensor_h

#include <ArduinoJson.h>
#include "Entity.h"
#include "Measurable.h"

class UVsensor : public Entity, public Measurable {
  private:
    byte pin;
  
  public:
    UVsensor(){};
    UVsensor(byte id, byte pin, String name);
    float measure();
    JsonObject toJson(JsonDocument &doc);
    boolean update(JsonObject &obj);
    Measurable* getMeasurable();
};

#endif
