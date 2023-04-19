#include "Pump.hpp"

Pump::Pump() {}

Pump::Pump(byte id, byte pin, String name)  : Entity(id,name) { 
  this->pin = pin;
  pinMode(pin, OUTPUT);
  this->running = false;
}

void Pump::turn(bool on) {
  this->running = on;
}

JsonObject Pump::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  json["value"] = this->running;
  return json;
}

boolean Pump::update(JsonObject &obj) {
  if (obj["id"] != this->id)
    return false;
  this->name = obj["name"].as<String>();
  return true;
}
