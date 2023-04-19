#ifndef ENTITY_H
#define ENTITY_H

#include <ArduinoJson.h> 
#include "Measurable.hpp"
#include <Arduino.h>

class Entity  {
  public: 
  byte id;
  String name;
  Entity();
  Entity(byte id, String name);
  virtual boolean update(JsonObject &doc) = 0;
  virtual Measurable* getMeasurable(){return nullptr;};  // returns (this) as Measurable, or nullptr.  shortcut for -fno-rtti (no dymamic_cast) 
  virtual JsonObject toJson(JsonDocument& doc);
};

#endif
