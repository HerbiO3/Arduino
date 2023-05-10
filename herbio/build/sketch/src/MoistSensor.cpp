#line 1 "/home/nixer/Desktop/arduino/herbio/src/MoistSensor.cpp"
#include "MoistSensor.hpp"

MoistureSensor::MoistureSensor() {}

MoistureSensor::MoistureSensor(byte id, byte pin, const char* name) : Entity(id,name) {
  this->pin = pin;
  pinMode(pin, INPUT);
}

JsonObject MoistureSensor::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  measure();
  json["value"] = this->value;
  return json;
}


float MoistureSensor::measure() {
  value = map(analogRead(pin), 600, 360, 0, 100);
  delay(20);
  return value = max(value, 0);
}

Measurable* MoistureSensor::getMeasurable() {
  return this;
}
void MoistureSensor::dump(byte* buffer){
  memcpy(buffer, this, sizeof(MoistureSensor));
}
void MoistureSensor::load(byte* buffer) {
  memcpy(this, buffer, sizeof(MoistureSensor));
}
byte MoistureSensor::size() {
  return sizeof(MoistureSensor);
}