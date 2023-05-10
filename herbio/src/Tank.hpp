#ifndef TANK_H
#define TANK_H

#include "Entity.hpp"
#include "Measurable.hpp"

class Tank : public Entity, public Measurable {

    byte echoPin; 
    byte trigPin; 
public:
    uint16_t dist_full; // distance to water surface        // ** calculated on FE  
    uint16_t dist_empty; // distance to the bottom of tank  // if full==empty  measure() returns absolute cm
    Tank(byte id, const char* name, byte echoPin, byte trigPin);
    Tank(byte id, const char* name, byte echoPin, byte trigPin, uint16_t dist_full, uint16_t dist_empty);
    float measure();
    JsonObject toJson(JsonDocument &doc);
    boolean update(JsonObject &obj);
    Measurable* getMeasurable();
    void dump(byte* buffer);
    void load(byte* buffer);
    byte size();
};

#endif


