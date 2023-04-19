#ifndef VALVE_H
#define VALVE_H

#include "Entity.hpp"

class Valve : public Entity {
private:
  byte pin;
  bool opened;

public:
  Valve(byte id, byte pin, String name);

  bool isOpen();
  bool open();
  bool close();

  JsonObject toJson(JsonDocument &doc);

  boolean update(JsonObject &obj);
};

#endif
