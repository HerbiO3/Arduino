#include "UVsensor.h"

UVsensor::UVsensor(byte id, byte pin, String name) {
  this->id = id;
  this->pin = pin;
  this->name = name;
}

float UVsensor::measure() {
  return (byte) (analogRead(pin)/4);
}

JsonObject UVsensor::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  json["value"] = measure();
  return json;
}

boolean UVsensor::update(JsonObject &obj) {
  if(obj["id"] != this->id)
    return false;
  this->name = obj["name"].as<String>();
  return true;
}

Measurable* UVsensor::getMeasurable() {
  return this;
}
