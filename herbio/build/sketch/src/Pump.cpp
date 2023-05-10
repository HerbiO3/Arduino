#line 1 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\src\\Pump.cpp"
#include "Pump.hpp"

Pump::Pump() {}

Pump::Pump(byte id, byte pin, const char* name)  : Entity(id,name) { 
  this->pin = pin;
  pinMode(pin, OUTPUT);
  this->running = false;
}

JsonObject Pump::toJson(JsonDocument &doc) {
  JsonObject json = 
  Entity::toJson(doc);
  json["value"] = this->running;
  return json;
}
byte Pump::up(){
  digitalWrite(pin, HIGH);
  running++;
}
byte Pump::down(){
  digitalWrite(pin, LOW);
  running--;
  if(running < 0)
    running = 0;
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
void Pump::dump(byte* buffer){
  memcpy(buffer, this, sizeof(Pump));
}
void Pump::load(byte* buffer) {
  memcpy(this, buffer, sizeof(Pump));
}
byte Pump::size() {
  return sizeof(Pump);
}
