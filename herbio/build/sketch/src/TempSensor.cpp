#line 1 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\src\\TempSensor.cpp"
#include "TempSensor.hpp"

//SINGLETON
TempSensor::TempSensor() {}

TempSensor::TempSensor(byte id, byte pin, const char* name) : Entity(id,name) {
  this->oneWire = OneWire(pin);
  this->tempSensors = DallasTemperature(&oneWire);
  this->tempSensors.begin();
}

float TempSensor::measure() {
  tempSensors.requestTemperatures();
  return tempSensors.getTempCByIndex(0);
}

JsonObject TempSensor::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  json["value"] = this->measure();
  return json;
}


Measurable* TempSensor::getMeasurable() {
  return this;
}

void TempSensor::dump(byte* buffer){
  memcpy(buffer, this, sizeof(TempSensor));
}
void TempSensor::load(byte* buffer) {
  memcpy(this, buffer, sizeof(TempSensor));
}