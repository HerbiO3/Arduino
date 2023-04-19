#ifndef MOISTSENSOR_H
#define MOISTSENSOR_H

#include "Entity.hpp"
#include "Measurable.hpp"
#include <ArduinoJson.h>

class MoistureSensor : public Entity, public Measurable {
  private:
    byte pin;
    int value;
    
  public:
    MoistureSensor();
    MoistureSensor(byte id, byte pin, String name);
    virtual JsonObject toJson(JsonDocument &doc) override;
    virtual boolean update(JsonObject &obj) override;
    virtual float measure() override;
    virtual Measurable* getMeasurable() override;
};

#endif
