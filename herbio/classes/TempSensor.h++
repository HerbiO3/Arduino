#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Entity.h>
#include <Measurable.h>
#include <ArduinoJson.h>

class TemperatureSensor : public Entity, public Measurable {
  public:
    TemperatureSensor();
    TemperatureSensor(byte id, byte pin, String name);
    float measure();
    JsonObject toJson(JsonDocument& doc);
    boolean update(JsonObject& obj);
    Measurable* getMeasurable();

  private:
    byte pin;
    String name;
    OneWire oneWire;
    DallasTemperature tempSensors;
};

#endif // TEMPERATURE_SENSOR_H
