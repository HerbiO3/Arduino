#include "Valve.h"

Valve::Valve(byte id, byte pin, String name) {
  this->id = id;
  this->name = name;
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

bool Valve::isOpen() {
  return this->opened;
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
  this->opened = obj["opened"];
  return true;
}
