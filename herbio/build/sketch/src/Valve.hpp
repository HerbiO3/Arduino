#line 1 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\src\\Valve.hpp"
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

  //boolean update(JsonObject &obj);
};

#endif
