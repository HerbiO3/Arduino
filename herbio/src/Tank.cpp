#include "Tank.hpp"
#include <Adafruit_MCP23X17.h>

/*Tank::Tank(byte id, const char* name, byte echoPin, byte trigPin,Adafruit_MCP23X17 *mcp) : Entity(id,name) {
  this->echoPin = echoPin;
  this->trigPin = trigPin;
  this->dist_full = this->dist_empty = 0;
  this->mcp = mcp;

  mcp->pinMode(this->trigPin,OUTPUT);
  mcp->pinMode(this->echoPin,INPUT);
}*/
Tank::Tank(byte id, const char* name, byte echoPin, byte trigPin, uint16_t dist_full, uint16_t dist_empty):Entity(id,name) {
  this->dist_full = dist_full;
  this->dist_empty = dist_empty;

  this->echoPin = echoPin;
  this->trigPin = trigPin;
  this->dist_full = this->dist_empty = 0;

  pinMode(this->trigPin,OUTPUT);
  pinMode(this->echoPin,INPUT);
}



float Tank::measure() {
  // send/rec, calc, store value
  Serial.print("trig: ");
  Serial.print(trigPin);
  Serial.print("  echo: ");
  Serial.println(echoPin);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin,HIGH);
  Serial.print(duration);
  Serial.print(" dist: ");

  uint16_t distance = duration * 0.034f / 2;
  float value = float(distance);
  if(dist_empty != dist_full)
    value =  map(distance, dist_empty, dist_full, 0, 100); // maps distance to a value in percentage
  Serial.println(value);

  return value;
}


JsonObject Tank::toJson(JsonDocument &doc) {
  JsonObject json = Entity::toJson(doc);
  json["dist_full"] = dist_full;
  json["dist_empty"] = dist_empty;
  json["value"] = measure();

  return json;
}

boolean Tank::update(JsonObject &obj) {
  if(! Entity::update(obj))
    return false;
  if(obj["dist_full"])
    dist_full = obj["dist_full"];
  if(obj["dist_empty"])
    dist_empty = obj["dist_empty"];
  return true;
}


byte Tank::dump(byte* buffer){
  memcpy(buffer, this, sizeof(Tank));
  return sizeof(Tank);

}
void Tank::load(byte* buffer) {
  memcpy(this, buffer, sizeof(Tank));

}
byte Tank::size() {
  return sizeof(Tank);
}
