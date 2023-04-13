#include "Section.h"

Section::Section() {}

Section::Section(byte id,String name, byte min_humid, uint water_time,Valve *valve) {
  this->id   = id;
  this->name = name;
  this->min_humid  = min_humid;
  this->water_time = water_time;
  this->valve = valve;
}

Section* Section::setValve(Valve *valve) {
  this->valve = valve;
  return this;
}

Section* Section::setTimer(uint time_start, uint time_end) {
  this->timer_start = time_start;
  this->timer_end = time_end;
  return this;
}

Section* Section::setMinHumid(byte min_humid) {
  this->min_humid = min_humid;
  return this;
}

Section* Section::setWaterTime(uint water_time) {
  this->water_time = water_time;
  return this;
}

Section* Section::setMoistureSensor(MoistureSensor *sensor) {
  this->moisture = sensor;
  return this;
}

JsonObject Section::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  json["timer"]       = this->timer->toJson(doc);
  json["water_time"]  = this->water_time;
  json["min_humidity"]= this->min_humid;
  json["valve"]       = this->valve->toJson(doc);
  json["moisture"]    = this->moisture->toJson(doc);
  return json;
}

boolean Section::update(JsonObject &obj) {
  if(obj["id"] != this->id)
    return false;
  this->name      = obj["name"].as<String>() ;
  this->water_time= obj["water_time"].as<int>();
  this->min_humid = obj["min_humidity"].as<int>();
  this->moisture = getEntity(obj["moisture"].as<int>());
  return true;
}
