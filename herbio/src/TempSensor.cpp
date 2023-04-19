#include "TempSensor.hpp"

//SINGLETON
TemperatureSensor::TemperatureSensor() {}

TemperatureSensor::TemperatureSensor(byte id, byte pin, String name) : Entity(id,name) {
  this->oneWire = OneWire(pin);
  this->tempSensors = DallasTemperature(&oneWire);
  this->tempSensors.begin();
}

float TemperatureSensor::measure() {
  tempSensors.requestTemperatures();
  return tempSensors.getTempCByIndex(0);
}

JsonObject TemperatureSensor::toJson(JsonDocument& doc) {
  JsonObject json = Entity::toJson(doc);
  json["value"] = this->measure();
  return json;
}

boolean TemperatureSensor::update(JsonObject& obj) {
  if (obj["id"] != this->id) {
    return false;
  }
  this->name = obj["name"].as<String>();
  return true;
}

Measurable* TemperatureSensor::getMeasurable() {
  return this;
}
