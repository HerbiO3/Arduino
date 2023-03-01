#pragma once

#include <ArduinoJson.h>

class Measurable{public:  virtual float     measure() = 0; };
class Entity  {
  public: 
  byte id;
  String name;
  virtual JsonObject toJson(JsonDocument &doc) = 0;
  virtual boolean update(JsonObject &doc) = 0;
  virtual Measurable* getMeasurable(){return nullptr;};  // returns (this) as Measurable, or nullptr.  shortcut for -fno-rtti (no dymamic_cast) 
  };
class TemperatureSensor : public Entity, public Measurable{
private:
  byte pin;
  String name;
  OneWire oneWire;
  DallasTemperature tempSensors;
public:
  TemperatureSensor(){};
  TemperatureSensor(byte id,byte pin, String name){
    this->id = id;
    this->name = name;
    this->pin  = pin;
    this->oneWire = OneWire(pin);
    this->tempSensors = DallasTemperature(&oneWire);
    this->tempSensors.begin();
  }
  float measure(){
    tempSensors.requestTemperatures();
    return tempSensors.getTempCByIndex(0);
  }
  JsonObject toJson(JsonDocument &doc){
    JsonObject json = doc.createNestedObject();
    json["id"] = this->id;
    json["name"] = this->name;
    json["value"] = this->measure();
    return json;
  }
  boolean update(JsonObject &obj){
    if(obj["id"] != this->id)
      return false;
    this->name       = obj["name"].as<String>();
    return true;
  }
  Measurable* getMeasurable(){return this;}
};
class MoistureSensor    : public Entity, public Measurable{private:
  byte pin;
public:
  int value;
  int tresh_max;
  int tresh_min;
  
  MoistureSensor(){};
  MoistureSensor(byte id, byte pin, String name){
    this->id = id;
    this->pin = pin;
    this->name = name;
    pinMode(pin, INPUT);
  }
  MoistureSensor(byte id,byte pin, String name,int tresh_min, int tresh_max) : MoistureSensor(id,pin,name) {
    this->tresh_min = tresh_min;
    this->tresh_max = tresh_max;
  }
  JsonObject toJson(JsonDocument &doc){
    measure();
    JsonObject json = doc.createNestedObject();        
    json["id"] = this->id;         
    json["name"] = this->name;         
    json["value"] = this->value;         
    json["tresh_min"] = this->tresh_min;
    json["tresh_max"] = this->tresh_max;
    return json;
  }
  boolean update(JsonObject &obj){
    if(obj["id"] != this->id)
      return false;
    this->name       = obj["name"].as<String>()    ; 
    this->value      = obj["value"]   ; 
    this->tresh_min  = obj["tresh_min"];
    this->tresh_max  = obj["tresh_max"];
    return true;
  }

  float measure(){
    value = map(analogRead(pin), 600, 360, 0, 100);
    delay(20);
    return value = max(value,0);
  }
  Measurable* getMeasurable(){return this;}
};
class UVsensor          : public Entity, public Measurable{private:
  byte pin;
public:
  UVsensor(){};
  UVsensor(byte id,byte pin, String name){
    this->id= id;
    this->pin = pin;
    this->name=name;
  }
  // UV sensor
  float measure(){
    return (byte) (analogRead(pin)/4);
  }

  JsonObject toJson(JsonDocument &doc){
    JsonObject json = doc.createNestedObject();
    json["id"] = this->id;
    json["name"] = this->name;
    json["value"] = measure();
    return json;
  }

      boolean update(JsonObject &obj){
    if(obj["id"] != this->id)
      return false;
    this->name       = obj["name"].as<String>();
    return true;
  }
  Measurable* getMeasurable(){return this;}
};
class Tank              : public Entity, public Measurable{private:
  byte echoPin; 
  byte trigPin; 
  byte value; // value in percentage
  short dist_full; // distance to water surface
  short dist_empty; // distance to the bottom of tank
public:
  Tank(byte id, String name, byte echoPIN, byte trigPIN){
    this->id = id;
    this->name = name;
    echoPin = echoPIN;
    trigPin = trigPIN;
  }

  Tank(byte id, String name, byte echoPIN, byte trigPIN, short dist_full, short dist_empty) : Tank(id,name,echoPin,trigPin) {
      this->dist_full = dist_full;
      this->dist_empty= dist_empty;
  }

  /**
   * @brief measure distance to water surface
   * @return returns value (percentage of  full-empty) 
   */
  float measure(){
    //send/rec,calc, store value
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    short distance = duration * 0.034f / 2;
    //value = (distance - dist_full) / (dist_empty - dist_full);
    return this->value = map(distance, dist_empty,dist_full, 0,100); //maps dist
  }

  //getters and setters
  /** @brief gets last meassuredValue (see measureValue()) */
  byte getValue(){             return value;  }

  void setDist_full(short d){  dist_full = d; }
  short getDist_full(){  return dist_full; }

  void setDist_empty(short d){  dist_empty = d; }
  short getDist_empty(){  return dist_empty; }

  JsonObject toJson(JsonDocument &doc){
    JsonObject json = doc.createNestedObject();
    json["id"] = this->id;
    json["name"] = this->name;
    json["value"] = this->value;
    json["dist_full"] = this->dist_full;
    json["dist_empty"] = this->dist_empty;
    return json;
  }
    boolean update(JsonObject &obj){
    if(obj["id"] != this->id)
      return false;
    this->name       = obj["name"].as<String>(); 
    this->dist_full  = obj["dist_full"] ;
    this->dist_empty = obj["dist_empty"];
    return true;
  }
  Measurable* getMeasurable(){return this;}
};
class Valve             : public Entity{private:
  byte pin;
  bool opened;
public:
  Valve(byte id,byte pin, String name){
    this->id = id;
    this->name = name;
    this->pin  = pin;
    pinMode(pin, OUTPUT);
  }
  bool isOpen(){
    return this->opened;
  }

  JsonObject toJson(JsonDocument &doc){
    JsonObject json = doc.createNestedObject();
    json["id"] = this->id;
    json["name"] = this->name;
    json["value"] = this->opened;
    return json;
  }

    boolean update(JsonObject &obj){
    if(obj["id"] != this->id)
      return false;
    this->name      = obj["name"].as<String>() ; 
    this->opened    = obj["opened"] ; 
    return true;
  }

};
class Pump              : public Entity{private:
  byte pin;
  bool running;
  int timePerMl;
public: 
  Pump(){};
  Pump(byte id, byte pin, String name, int timePerMililiter){
    Pump();
    this->id = id;
    this->pin = pin;
    pinMode(pin, OUTPUT);

    if(name == nullptr || name.length() == 0)
        this->name = String("pump_p")+((int)pin);
    else this->name = name;
    this->timePerMl = timePerMililiter;
    this-> running = false;
  }
  void turn(bool on){
    this->running = on;
  }


  JsonObject toJson(JsonDocument &doc){
    JsonObject json = doc.createNestedObject();    //"{\n"4 
    json["id"] = this->id;
    json["name"] = this->name;          //"  'name' : sensor_2"  20+  
    json["value"] = this->running;    //"  'running' : False"    25
    json["timePerMl"] = this->timePerMl;//"  'timePerMl' : 1256"    25
    // avg 80
    return json;
  }

  boolean update(JsonObject &obj){
    if(obj["id"] != this->id)
      return false;
    this->name      = obj["name"].as<String>()      ; 
    this->running   = obj["running"]   ; 
    this->timePerMl = obj["timePerMl"] ;
    return true;
  }
};
