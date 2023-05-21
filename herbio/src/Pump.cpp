#include "Pump.hpp"

Pump::Pump() {}

Pump::Pump(byte id, byte pin, const char* name)  : Entity(id,name) { 
  this->pin = pin;
  pinMode(pin, OUTPUT);
  this->running = false;
}

JsonObject Pump::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  json["value"] = this->running;
  return json;
}
byte Pump::up(){
  digitalWrite(pin, HIGH);
  running++;
  return running;
}
byte Pump::down(){
  if(running > 0){
    running--;
  }
  if(running==0)
    digitalWrite(pin, LOW);
  return running;
}
byte Pump::kill(){
  digitalWrite(pin, LOW);
  running = 0; //decrement value by 1/0
  return running;
}

boolean Pump::update(JsonObject &obj){
  return false;
}
byte Pump::dump(byte* buffer){
  memcpy(buffer, this, sizeof(Pump));
  return sizeof(Pump);
}
void Pump::load(byte* buffer) {
  memcpy(this, buffer, sizeof(Pump));
  running = 0;
}
byte Pump::size() {
  return sizeof(Pump);
}
