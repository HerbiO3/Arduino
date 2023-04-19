#include "Tank.hpp"

Tank::Tank(byte id, String name, byte echoPin, byte trigPin) : Entity(id,name) {
  this->echoPin = echoPin;
  this->trigPin = trigPin;
  this->dist_full = this->dist_empty = 0;

}

Tank::Tank(byte id, String name, byte echoPin, byte trigPin, uint16_t dist_full, uint16_t dist_empty) : Tank(id, name, echoPin, trigPin){
  
  this->dist_full = dist_full;
  this->dist_empty = dist_empty;
}

float Tank::measure() {
  // send/rec, calc, store value
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  uint16_t distance = duration * 0.034f / 2;
  float value =  (dist_empty != dist_full) ?
                 map(distance, dist_empty, dist_full, 0, 100) // maps distance to a value in percentage
                 : float(distance);

  return value;
}

void Tank::setDist_full(uint16_t dist_full) {
  this->dist_full = dist_full;
}

short Tank::getDist_full() {
  return dist_full;
}

void Tank::setDist_empty(uint16_t dist_empty) {
  this->dist_empty = dist_empty;
}

short Tank::getDist_empty() {
  return dist_empty;
}

JsonObject Tank::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  json["dist_full"] = dist_full;
  json["dist_empty"] = dist_empty;
  return json;
}

boolean Tank::update(JsonObject &obj) {
  if(obj["id"] != id)
    return false;
  name = obj["name"].as<String>();
  if(obj["dist_full"])
    dist_full = obj["dist_full"];
  if(obj["dist_empty"])
    dist_empty = obj["dist_empty"];
  return true;
}

Measurable* Tank::getMeasurable() {
  return this;
}