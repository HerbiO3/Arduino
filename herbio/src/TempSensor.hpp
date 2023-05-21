#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include "Entity.hpp"
#include <ArduinoJson.h>
//SINGLETON (due to initalizing DallasTem. in constructor)
class TempSensor : public Entity {
  public:
    TempSensor();
    TempSensor(byte id, byte pin, const char* name);
    //boolean update(JsonObject &obj);
    JsonObject toJson(JsonDocument& doc);
    float measure();
    
    byte dump(byte* buffer);
    void load(byte* buffer);
    byte size();

  private:
    OneWire oneWire;
    DallasTemperature tempSensors;
};

#endif // TEMPERATURE_SENSOR_H
