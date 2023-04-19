#ifndef PUMP_H
#define PUMP_H

#include "Entity.hpp"

class Pump : public Entity {
  private:
    byte pin;
    bool running;
    
  public: 
    Pump();
    Pump(byte id, byte pin, String name);

    void turn(bool on);

    virtual JsonObject toJson(JsonDocument &doc) override;
    virtual boolean update(JsonObject &obj) override;
};

#endif
