#line 1 "/home/nixer/Desktop/arduino/herbio/src/Entity.hpp"
#ifndef ENTITY_H
#define ENTITY_H

#include <ArduinoJson.h> 
#include "Measurable.hpp"
#include <Arduino.h>
#define NAME_LENGTH 15

class Entity  {
  public: 
  byte id;
  const char *name; // not enough space to allocate, * -> .text
  Entity();
  Entity(byte id, const char* name);
  virtual Measurable* getMeasurable(){return nullptr;};  // returns (this) as Measurable, or nullptr.  shortcut for -fno-rtti (no dymamic_cast) 
  virtual JsonObject toJson(JsonDocument& doc);
  virtual boolean update(JsonObject &obj);
  void eToJson(JsonObject &json);

  virtual void dump(byte* buffer)=0;
  virtual void load(byte* buffer)=0;
  virtual byte size()=0;
};
Entity* getEntity(Entity* entities[], byte id);
#endif
