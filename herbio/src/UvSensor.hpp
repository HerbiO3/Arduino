#ifndef UvSensor_h
#define UvSensor_h

#include <ArduinoJson.h>
#include "Entity.hpp"

class UVsensor : public Entity {
  private:
    byte pin;
  
  public:
    UVsensor(){};
    UVsensor(byte id, byte pin, const char* name);
    float measure();
    JsonObject toJson(JsonDocument &doc);
    byte dump(byte* buffer);
    void load(byte* buffer);
    byte size();

};

#endif


