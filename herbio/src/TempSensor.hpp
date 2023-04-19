#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include "Entity.hpp"
#include "Measurable.hpp"
#include <ArduinoJson.h>
//SINGLETON (due to initalizing DallasTem. in constructor)
class TemperatureSensor : public Entity, public Measurable {
  public:
    TemperatureSensor();
    TemperatureSensor(byte id, byte pin, String name);
    float measure();
    JsonObject toJson(JsonDocument& doc);
    boolean update(JsonObject& obj);
    Measurable* getMeasurable();

  private:
    OneWire oneWire;
    DallasTemperature tempSensors;
};

#endif // TEMPERATURE_SENSOR_H
