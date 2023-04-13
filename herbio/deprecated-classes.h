

// THIS FILE IS DEPRECATED (refactored into separate files "classes/*(.h)|(.cpp)"   
// this was a trash file to be honest, but fullfiled fast impl. purpose
#if 0

#pragma once
#include <Arduino.h>
#include "LinkedList.h"
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
class MoistureSensor    : public Entity, public Measurable{
private:
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

class Timer : Entity {
public:
  unsigned int start;
  unsigned int target;
  Timer() {
    start = 0;
    target = 0;
    name = "untitled";
  }
  Timer(byte id, String name, uint start, uint target){
    this->id = id;
    this->name=name;
    this->start=start;
    this->target=target;
  }
  Timer* setTimes(uint start, uint target){
    this->start=start;
    this->target=target;
    return this;
  }
  Timer* setName(String name){
    this->name=name;
    return this;
  }

  void next(){
    start = target;   
    target += target - start; // push `next` to future
  }

  boolean passed(unsigned int time){
    return target < time;
  }
  
  boolean passed(unsigned int time, bool toNext){
    if(target < time){
      next();
      return true;
    }
    return false;
  }

  JsonObject toJson(JsonDocument &doc){
    JsonObject json = doc.createNestedObject();
    json["id"] = this->id;
    json["name"] = this->name;
    json["start"] = this->start;
    json["target"] = this->target;

    return json;
  }

  boolean update(JsonObject &obj){
    if(obj["id"] != this->id)
      return false;
    this->name   = obj["name"].as<String>() ; 
    this->start  = obj["start"];
    this->target = obj["target"];
    return true;
  }

};
class Section : public Entity{
public:
  enum MODE{MANUAL,AUTO,DEAD};
  int unit;
  MODE mode;
  unsigned char min_humid;
  uint water_time;

  Timer* timer;
  Valve* valve;
  MoistureSensor *moisture;
  Section(){}
  Section(byte id,String name, byte min_humid, uint water_time,Valve *valve){
    this->id   = id;
    this->name = name;
    this->min_humid  = min_humid;
    this->water_time = water_time;
    this->valve = valve;
  }

  Section* setValve(Valve *valve){
    this->valve = valve;
    return this;
  }
  Section* setTimer(Timer *timer){
    this->timer = timer;
    return this;
  }
  Section* setMinHumid(byte min_humid){
    this->min_humid = min_humid;
    return this;
  }
  Section* setWaterTime(uint water_time){
    this->water_time = water_time;
    return this;
  }
  Section* setMoistureSensor(MoistureSensor *sensor){
    this->moisture = sensor;
    return this;
  }
  JsonObject toJson(JsonDocument &doc){
    JsonObject json = doc.createNestedObject();
    json["id"]          = this->id;
    json["name"]        = this->name;
    json["timer"]       = this->timer->toJson(doc); //???
    json["water_time"]  = this->water_time;
    json["min_humidity"]= this->min_humid;
    json["valve"]       = this->valve->toJson(doc); //???
    json["moisture"]   = this->moisture->toJson(doc);
    return json;
  }
  /**
   * Updates only plain values (not nested entities)
   * such as: name,water_time,min_humid
  */
  
  boolean update(JsonObject &obj){
    if(obj["id"] != this->id)
      return false;
    this->name      = obj["name"].as<String>() ; 
    this->water_time= obj["water_time"].as<int>();
    this->min_humid = obj["min_humidity"].as<int>();
    this->moisture = getEntity(obj["moisture"].as<int>());
    return true;
  }

};


Entity* getEntity(byte id);
//usual constructors: (id, pin, name,  ..sensorParams)
MoistureSensor moist0(1,A0,"moist_A0",30,50);
MoistureSensor moist1(2,A1,"moist_A1",30,50);
MoistureSensor moist2(3,A2,"moist_A2",30,50);
MoistureSensor moist3(4,A3,"moist_A3",30,50);

Tank tank(5,"g_tank",3,2);
Pump pump(6,4,"g_pump",60);
UVsensor uv(7,A4,"uv_sensor");
TemperatureSensor themp(8,7,"thermometer");

Valve valve0(9,6,  "valve0");        
Valve valve1(10,8, "valve1");        
Valve valve2(11,9, "valve2");        
Valve valve3(12,10,"valve3");        
Section section0(20,"section0",20,90,&valve0,&moist0);
Section section1(20,"section0",20,90,&valve1,&moist1);
Section section2(20,"section0",20,90,&valve2,&moist2);
Section section3(20,"section0",20,90,&valve3,&moist3);

byte buttonPin = 12;
Entity *entities[]{ 
  &moist0, &moist1, &moist2, &moist3, 
  &valve0, &valve1, &valve2, &valve3,
  &tank, &pump, &uv, &themp,
  &section0,
  &section1,
  &section2,
  &section3
};

#define NELEM(x) (sizeof(x)/sizeof(*x))

Entity* getEntity(byte id){
  for(byte i=0; i < NELEM(entities)-1; i++){
    if(entities[i]->id == id)
      return entities[i];
  }
}
#endif