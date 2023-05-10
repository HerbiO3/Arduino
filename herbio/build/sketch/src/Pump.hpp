#line 1 "/home/nixer/Desktop/arduino/herbio/src/Pump.hpp"
#ifndef PUMP_H
#define PUMP_H

#include "Entity.hpp"

class Pump : public Entity {
  private:
    byte pin;
    byte running;
    
  public: 
    Pump();
    Pump(byte id, byte pin, const char* name);

    byte down();
    byte up();
    byte kill();
    JsonObject toJson(JsonDocument &doc);
    boolean update(JsonObject &obj);
    void dump(byte* buffer);
    void load(byte* buffer);
    byte size();
};

#endif




