#ifndef SECTION_H
#define SECTION_H

#include "Entity.hpp"
#include "Valve.hpp"
#include "MoistSensor.hpp"
typedef uint32_t time_t;

enum MODE{MANUAL,AUTO,DEAD};

class Section : public Entity {
public:
  int unit;
  MODE mode;
  unsigned char min_humid;
  time_t water_time;

  time_t timer_start;
  time_t timer_end;

  Valve* valve;
  MoistureSensor *moisture;
  Section();
  Section(byte id,String name, byte min_humid, time_t water_time, Valve *valve);
  Section* setValve(Valve *valve);
  Section* setTimer(time_t time_start, time_t time_end);
  Section* setMinHumid(byte min_humid);
  Section* setWaterTime(time_t water_time);
  Section* setMoistureSensor(MoistureSensor *sensor);

  JsonObject toJson(JsonDocument &doc);
  boolean update(JsonObject &obj);
};

#endif
