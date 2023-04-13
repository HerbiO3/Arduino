#ifndef TANK_H
#define TANK_H

#include "Entity.h"
#include "Measurable.h"

class Tank : public Entity, public Measurable {
  private:
    byte echoPin; 
    byte trigPin; 
    byte value; // value in percentage
    short dist_full; // distance to water surface
    short dist_empty; // distance to the bottom of tank
  public:
    Tank(byte id, String name, byte echoPin, byte trigPin);
    Tank(byte id, String name, byte echoPin, byte trigPin, short dist_full, short dist_empty);

    float measure();

    // getters and setters
    byte getValue();
    void setDist_full(short dist_full);
    short getDist_full();
    void setDist_empty(short dist_empty);
    short getDist_empty();

    JsonObject toJson(JsonDocument &doc);
    boolean update(JsonObject &obj);
    Measurable* getMeasurable();
};

#endif
