#ifndef Timer_h
#define Timer_h

#include "Entity.h"

class Timer : public Entity {
public:
  unsigned int start;
  unsigned int target;
  Timer();
  Timer(byte id, String name, uint start, uint target);
  Timer* setTimes(uint start, uint target);
  Timer* setName(String name);

  void next();
  boolean passed(unsigned int time);
  boolean passed(unsigned int time, bool toNext);

  JsonObject toJson(JsonDocument &doc);
  boolean update(JsonObject &obj);
};

#endif
