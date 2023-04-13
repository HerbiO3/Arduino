#include "Pump.h"

Pump::Pump() {}

Pump::Pump(byte id, byte pin, String name, int timePerMililiter) {
  this->id = id;
  this->pin = pin;
  pinMode(pin, OUTPUT);

  if (name == nullptr || name.length() == 0)
    this->name = String("pump_p") + ((int)pin);
  else 
    this->name = name;

  this->timePerMl = timePerMililiter;
  this->running = false;
}

void Pump::turn(bool on) {
  this->running = on;
}

JsonObject Pump::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  json["value"] = this->running;
  json["timePerMl"] = this->timePerMl;
  return json;
}

boolean Pump::update(JsonObject &obj) {
  if (obj["id"] != this->id)
    return false;
  this->name = obj["name"].as<String>();
  this->running = obj["running"];
  this->timePerMl = obj["timePerMl"];
  return true;
}
