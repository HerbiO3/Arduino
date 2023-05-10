#ifndef MOISTSENSOR_H
#define MOISTSENSOR_H

#include "Entity.hpp"
#include "Measurable.hpp"
#include <ArduinoJson.h>
#include <Arduino.h>

class MoistureSensor : public Entity, public Measurable {
  private:
    byte pin;
    int value;
    
  public:
    MoistureSensor();
    MoistureSensor(byte id, byte pin, const char* name);
    JsonObject toJson(JsonDocument &doc) ;
    float measure();
    Measurable* getMeasurable();
    void dump(byte* buffer);
    void load(byte* buffer);
    byte size();
};

#endif


