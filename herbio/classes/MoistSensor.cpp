#include "MoistSensor.h"

MoistureSensor::MoistureSensor() {}

MoistureSensor::MoistureSensor(byte id, byte pin, String name) {
  this->id = id;
  this->pin = pin;
  this->name = name;
  pinMode(pin, INPUT);
}

MoistureSensor::MoistureSensor(byte id, byte pin, String name, int tresh_min, int tresh_max)
  : MoistureSensor(id, pin, name) {
  this->tresh_min = tresh_min;
  this->tresh_max = tresh_max;
}

JsonObject MoistureSensor::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  measure();
  json["value"] = this->value;
  json["tresh_min"] = this->tresh_min;
  json["tresh_max"] = this->tresh_max;
  return json;
}

boolean MoistureSensor::update(JsonObject &obj) {
  if (obj["id"] != this->id) return false;
  this->name = obj["name"].as<String>();
  this->value = obj["value"];
  this->tresh_min = obj["tresh_min"];
  this->tresh_max = obj["tresh_max"];
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
