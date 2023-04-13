#ifndef VALVE_H
#define VALVE_H

#include "Entity.h"

class Valve : public Entity {
private:
  byte pin;
  bool opened;

public:
  Valve(byte id, byte pin, String name);

  bool isOpen();

  JsonObject toJson(JsonDocument &doc);

  boolean update(JsonObject &obj);
};

#endif
