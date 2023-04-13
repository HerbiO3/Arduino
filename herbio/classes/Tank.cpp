#include "Tank.h"

Tank::Tank(byte id, String name, byte echoPin, byte trigPin) {
  this->id = id;
  this->name = name;
  this->echoPin = echoPin;
  this->trigPin = trigPin;
}

Tank::Tank(byte id, String name, byte echoPin, byte trigPin, short dist_full, short dist_empty) : Tank(id, name, echoPin, trigPin) {
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
  short distance = duration * 0.034f / 2;
  value = map(distance, dist_empty, dist_full, 0, 100); // maps distance to a value in percentage
  return value;
}

byte Tank::getValue() {
  return value;
}

void Tank::setDist_full(short dist_full) {
  this->dist_full = dist_full;
}

short Tank::getDist_full() {
  return dist_full;
}

void Tank::setDist_empty(short dist_empty) {
  this->dist_empty = dist_empty;
}

short Tank::getDist_empty() {
  return dist_empty;
}

JsonObject Tank::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  json["value"] = value;
  json["dist_full"] = dist_full;
  json["dist_empty"] = dist_empty;
  return json;
}

boolean Tank::update(JsonObject &obj) {
  if(obj["id"] != id)
    return false;
  name = obj["name"].as<String>();
  dist_full = obj["dist_full"];
  dist_empty = obj["dist_empty"];
  return true;
}

Measurable* Tank::getMeasurable() {
  return this;
}
