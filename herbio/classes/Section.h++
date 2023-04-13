#ifndef SECTION_H
#define SECTION_H

#include "Entity.h"
#include "Timer.h"
#include "Valve.h"
#include "MoistSensor.h"

enum MODE{MANUAL,AUTO,DEAD};

class Section : public Entity {
public:
  int unit;
  MODE mode;
  unsigned char min_humid;
  uint water_time;

  uint timer_start;
  uint timer_end;

  Valve* valve;
  MoistureSensor *moisture;
  Section();
  Section(byte id,String name, byte min_humid, uint water_time,Valve *valve);

  Section* setValve(Valve *valve);
  Section* setTimer(uint time_start, uint time_end);
  Section* setMinHumid(byte min_humid);
  Section* setWaterTime(uint water_time);
  Section* setMoistureSensor(MoistureSensor *sensor);

  JsonObject toJson(JsonDocument &doc);
  boolean update(JsonObject &obj);
};

#endif
