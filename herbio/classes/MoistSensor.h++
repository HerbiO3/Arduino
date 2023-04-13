#ifndef MOISTSENSOR_H
#define MOISTSENSOR_H

#include "Entity.h"
#include "Measurable.h"
#include <ArduinoJson.h>

class MoistureSensor : public Entity, public Measurable {
  private:
    byte pin;
    int value;
    int tresh_max;
    int tresh_min;
    
  public:
    MoistureSensor();
    MoistureSensor(byte id, byte pin, String name);
    MoistureSensor(byte id, byte pin, String name, int tresh_min, int tresh_max);
    virtual JsonObject toJson(JsonDocument &doc) override;
    virtual boolean update(JsonObject &obj) override;
    virtual float measure() override;
    virtual Measurable* getMeasurable() override;
};

#endif
