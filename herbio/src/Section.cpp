#include "Section.hpp"

extern Entity **entites;
extern time_t curr_time;

Section::Section() {}


Section::Section(byte id,const char* name, byte min_humid, time_t water_time, Entity **all_ents, byte valve_id, byte moist_id) : Entity(id,name){
  this->min_humid  = min_humid;
  this->water_time = water_time;
  this->mode = MANUAL;
  this->water_now = 0;
  this->global_entites = all_ents;
  this->valve     = (Valve*)          getEntity(global_entites,valve_id);
  this->moisture  = (MoistureSensor*) getEntity(global_entites,moist_id);

}


JsonObject Section::toJson(JsonDocument& doc) {
  JsonObject json = Entity::toJson(doc);
  json["water_time"]    = this->water_time ;
  json["water_until"]   = this->water_until ;
  json["water_watered"] = this->_watering ;

  json["water_start"] = this->water_start;
  json["water_next"]  = this->water_next ;
  json["min_humidity"]= this->min_humid  ;
  json["water_now"]   = this->water_now;
  if(this->mode == AUTO ) json["mode"] = "auto"; 
  if(this->mode == TIMED) json["mode"] = "timed"; 
  if(this->mode == MANUAL ) json["mode"] = "manual"; 

  if(valve)    json["valve"]    = valve   ->toJson(doc); else Serial.println("Valve is null");
  if(moisture) json["moisture"] = moisture->toJson(doc); else Serial.println("moisture is null");


  return json;
}

boolean Section::update(JsonObject &obj){
  if(! Entity::update(obj))
    return false;
  
  if (obj.containsKey("water_time"))   this->water_time  = obj["water_time"].as<time_t>();
  if (obj.containsKey("water_start"))  this->water_start = obj["water_start"].as<time_t>();  
  if (obj.containsKey("water_next"))   this->water_next  = obj["water_next"].as<time_t>();
  if (obj.containsKey("min_humidity")) this->min_humid   = obj["min_humidity"].as<uint8_t>();
  if (obj.containsKey("water_now"))    this->water_now   = obj["water_now"];    
  
  if (obj.containsKey("valve_id")) 
      this->valve = (Valve*)getEntity(global_entites, obj["valve_id"].as<int8_t>());

  if (obj.containsKey("moisture_id")) 
      this->moisture = (MoistureSensor*)getEntity(global_entites, obj["moisture_id"].as<int8_t>());
  
  if(obj.containsKey("mode")){
    const char* new_mode = obj["mode"].as<const char*>(); 
    if(! strcmp(new_mode,"auto"  )) this->mode = AUTO;
    if(! strcmp(new_mode,"timed" )) this->mode = TIMED;
    if(! strcmp(new_mode,"manual")) this->mode = MANUAL;
  }

  return true;
}

void Section::water(time_t curr_time){
  this->water_until = curr_time + water_time;

  this->valve->open();
  this->_watering = true;
}
void Section::checkAndStopWater(time_t curr_time){
  if(!_watering) return; //not started or, valve was not opened
  if(curr_time > water_until){
    valve->close();
    _watering = false;
  }
}
// a <= b <= c
bool isBetween(time_t a,time_t b, time_t c){return a<=b && b<=c;}
void Section::action(time_t curr_time){
  //TIME REFRESH
  if(mode == TIMED){
    // far behind
    // lets have a sections  -0-|water_start|--A--|water_until|-------B---------...-water_next|--C--
    // A = watering begun on time and it is still going,
    // B = watering should end, and waiting for water_next time
    // C = period shifts (water_time=next, next=+'timeperiod'), so C becomes A

    // section C
    if( ! isBetween(water_start,curr_time,water_next)){
      time_t len = water_next - water_start;
      int k=0;  //shift once
      while((water_start + k * len) < curr_time) 
        k++;// find how many spans were skipped
      water_start += k*len; //shift k times
      water_next  += k*len;
    }
    
    // A      // water_until is calculated in water() so for now start+time
    if (isBetween(water_start, curr_time, water_start+water_time))
        if(!_watering)
          water(curr_time);
    // B
    //else if(isBetween(water_until,curr_time,water_next)) {}
    // same <|>
    //most of time try to stop
    checkAndStopWater(curr_time);
  }

  //WATER IF NEEDED
  if(mode == AUTO){
    if(min_humid >= moisture->measure())
      water(curr_time);
    checkAndStopWater(curr_time);
  }

  if(mode == MANUAL){ 
    if(water_now){
      water(curr_time);
      water_now = false;
    }
    checkAndStopWater(curr_time);
  }
}

byte Section::dump(byte* buffer){
  memcpy(buffer, this, sizeof(Section));
  buffer[offsetof(Section,valve)] = valve->id;
  buffer[offsetof(Section,moisture)] = moisture->id;
  return sizeof(Section);
}
void Section::load(byte* buffer) {
  memcpy(this, buffer, sizeof(Section));
  valve = (Valve*) getEntity(global_entites,buffer[offsetof(Section,valve)]);
  moisture = (MoistureSensor*) getEntity(global_entites,buffer[offsetof(Section,moisture)]);
}

byte Section::size() {
  return sizeof(Section);
}
