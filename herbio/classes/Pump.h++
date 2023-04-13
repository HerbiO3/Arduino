#ifndef PUMP_H
#define PUMP_H

#include "Entity.h"

class Pump : public Entity {
  private:
    byte pin;
    bool running;
    int timePerMl;
    
  public: 
    Pump();
    Pump(byte id, byte pin, String name, int timePerMililiter);

    void turn(bool on);

    virtual JsonObject toJson(JsonDocument &doc) override;
    virtual boolean update(JsonObject &obj) override;
};

#endif
