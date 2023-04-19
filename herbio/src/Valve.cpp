#include "Valve.hpp"
#include <Arduino.h>

Valve::Valve(byte id, byte pin, String name) : Entity(id,name) {
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

bool Valve::open(){
  digitalWrite(pin, HIGH);
  this->opened = true;
}

bool Valve::close() {
  digitalWrite(pin, LOW);
  this->opened = false;
}

JsonObject Valve::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  json["value"] = this->opened;
  return json;
}

boolean Valve::update(JsonObject &obj) {
  if (obj["id"] != this->id)
    return false;
  this->name = obj["name"].as<String>();
  return true;
}
