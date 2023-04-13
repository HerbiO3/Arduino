#include "Timer.h"

Timer::Timer() {
  start = 0;
  target = 0;
  name = "untitled";
}

Timer::Timer(byte id, String name, uint start, uint target){
  this->id = id;
  this->name=name;
  this->start=start;
  this->target=target;
}

Timer* Timer::setTimes(uint start, uint target){
  this->start=start;
  this->target=target;
  return this;
}

Timer* Timer::setName(String name){
  this->name=name;
  return this;
}

void Timer::next(){
  start = target;   
  target += target - start; // push `next` to future
}

boolean Timer::passed(unsigned int time){
  return target < time;
}

boolean Timer::passed(unsigned int time, bool toNext){
  if(target < time){
    next();
    return true;
  }
  return false;
}

JsonObject Timer::toJson(JsonDocument &doc){
  JsonObject json = Entity::toJson(doc);
  json["start"] = this->start;
  json["target"] = this->target;

  return json;
}

boolean Timer::update(JsonObject &obj){
  if(obj["id"] != this->id)
    return false;
  this->name   = obj["name"].as<String>() ; 
  this->start  = obj["start"];
  this->target = obj["target"];
  return true;
}
