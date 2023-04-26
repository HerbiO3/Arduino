#line 1 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\src\\UvSensor.cpp"
#include "UVsensor.hpp"

UVsensor::UVsensor(byte id, byte pin, const char* name) : Entity(id,name) {
  this->pin = pin;
}

float UVsensor::measure() {
  return (byte) (analogRead(pin)/4);
}

JsonObject UVsensor::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  json["value"] = measure();
  return json;
}

Measurable* UVsensor::getMeasurable() {
  return this;
}

void UVsensor::dump(byte* buffer){
  memcpy(buffer, this, sizeof(UVsensor));
}
void UVsensor::load(byte* buffer) {
  memcpy(this, buffer, sizeof(UVsensor));
}