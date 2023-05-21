#ifndef SECTION_H
#define SECTION_H

#include "Entity.hpp"
#include "Valve.hpp"
#include "MoistSensor.hpp"
#include "RTClib.h"

typedef uint32_t time_t;
enum MODE{AUTO = 'A',TIMED = 'T',MANUAL = 'M'};

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
  bool water_now;
  bool _watering;
  Valve* valve;
  MoistureSensor *moisture;
  Section();
  Section(byte id, const char* name, byte min_humid, time_t water_time, Entity **all_ents, byte valve_id, byte moist_id);
  JsonObject toJson(JsonDocument &doc);
  boolean update(JsonObject &obj);
/**
 * @param current_time real time in seconds from 2000-01-01
*/
  void action(time_t current_time);
  byte dump(byte* buffer);
  void load(byte* buffer);
  byte size();

private: 
  void checkAndStopWater(time_t curr_time);
  void water(time_t curr_time);

};

#endif
