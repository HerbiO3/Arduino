#ifndef ENTITY_H
#define ENTITY_H

#include <ArduinoJson.h> 
#include <Arduino.h>
#define NAME_LENGTH 15

class Entity  {
  public: 
  byte id;
  const char *name; // not enough space to allocate, * -> .text
  Entity();
  Entity(byte id, const char* name);
  virtual JsonObject toJson(JsonDocument& doc);
  virtual boolean update(JsonObject &obj);

  virtual byte dump(byte* buffer)=0; // fills the buffer and return written size
  virtual void load(byte* buffer)=0; // loads from buffer of size of entity (* dumping size)
  virtual byte size()=0;
};
Entity* getEntity(Entity* entities[], byte id);
#endif
