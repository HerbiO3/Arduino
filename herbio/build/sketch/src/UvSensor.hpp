#line 1 "/home/nixer/Desktop/arduino/herbio/src/UvSensor.hpp"
#ifndef UvSensor_h
#define UvSensor_h

#include <ArduinoJson.h>
#include "Entity.hpp"
#include "Measurable.hpp"

class UVsensor : public Entity, public Measurable {
  private:
    byte pin;
  
  public:
    UVsensor(){};
    UVsensor(byte id, byte pin, const char* name);
    float measure();
    JsonObject toJson(JsonDocument &doc);
    //boolean update(JsonObject &obj);
    Measurable* getMeasurable();
    void dump(byte* buffer);
    void load(byte* buffer);
    byte size();

};

#endif


