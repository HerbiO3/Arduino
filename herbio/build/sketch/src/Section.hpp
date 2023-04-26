#line 1 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\src\\Section.hpp"
#ifndef SECTION_H
#define SECTION_H

#include "Entity.hpp"
#include "Valve.hpp"
#include "MoistSensor.hpp"
#include "RTClib.h"

typedef uint32_t time_t;
enum MODE{AUTO,TIMED,MANUAL};

class Section : public Entity {

public:
  Entity **global_entites;
  int unit;
  MODE mode;
  unsigned char min_humid;
  time_t water_time;
  time_t water_until;

  time_t water_start;
  time_t water_next;
  bool watered;
  Valve* valve;
  MoistureSensor *moisture;
  Section();
  Section(byte id, const char* name, byte min_humid, time_t water_time, Valve *valve, MoistureSensor *moisture);
  JsonObject toJson(JsonDocument &doc);
  boolean update(JsonObject &obj);
/**
 * @param current_time real time in seconds from 2000-01-01
*/
  void action(time_t current_time);
   void dump(byte* buffer);
   void load(byte* buffer);

private: 
  void checkAndStopWater(time_t curr_time);
  void water(time_t curr_time);

};

#endif
