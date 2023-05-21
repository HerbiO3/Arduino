#ifndef MOISTSENSOR_H
#define MOISTSENSOR_H

#include "Entity.hpp"
#include <ArduinoJson.h>
#include <Arduino.h>

class MoistureSensor : public Entity {
  private:
    byte pin;
    int value;
    
  public:
    MoistureSensor();
    MoistureSensor(byte id, byte pin, const char* name);
    JsonObject toJson(JsonDocument &doc) ;
    float measure();
    byte dump(byte* buffer);
    void load(byte* buffer);
    byte size();
};

#endif


