#ifndef VALVE_H
#define VALVE_H

#include "Entity.hpp"
#include "Pump.hpp"

class Valve : public Entity {
private:
  byte pin;
public:
  bool opened;
  Pump *pump;
  Valve(byte id, byte pin, const char* name, Pump *pump);
  bool open();
  bool close();

  JsonObject toJson(JsonDocument &doc);
  void dump(byte* buffer);
  void load(byte* buffer);
  byte size();

  //boolean update(JsonObject &obj);
};

#endif
