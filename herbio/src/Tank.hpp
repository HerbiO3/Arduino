#ifndef TANK_H
#define TANK_H

#include "Entity.hpp"
#include "Measurable.hpp"

class Tank : public Entity, public Measurable {
  private:
    byte echoPin; 
    byte trigPin; 
    uint16_t dist_full; // distance to water surface        // ** calculated on FE  
    uint16_t dist_empty; // distance to the bottom of tank  // if full==empty  measure() returns absolute cm
  public:
    Tank(byte id, String name, byte echoPin, byte trigPin);
    Tank(byte id, String name, byte echoPin, byte trigPin, uint16_t dist_full, uint16_t dist_empty);

    float measure();

    // getters and setters
    void setDist_full(uint16_t dist_full);
    short getDist_full();
    void setDist_empty(uint16_t dist_empty);
    short getDist_empty();

    JsonObject toJson(JsonDocument &doc);
    boolean update(JsonObject &obj);
    Measurable* getMeasurable();
};

#endif
