#include "MoistSensor.hpp"

MoistureSensor::MoistureSensor() {}

MoistureSensor::MoistureSensor(byte id, byte pin, String name) : Entity(id,name) {
  this->pin = pin;
  pinMode(pin, INPUT);
}

JsonObject MoistureSensor::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  measure();
  json["value"] = this->value;
  return json;
}

boolean MoistureSensor::update(JsonObject &obj) {
  if (obj["id"] != this->id) return false;
  this->name = obj["name"].as<String>();
  return true;
}

float MoistureSensor::measure() {
  value = map(analogRead(pin), 600, 360, 0, 100);
  delay(20);
  return value = max(value, 0);
}

Measurable* MoistureSensor::getMeasurable() {
  return this;
}
