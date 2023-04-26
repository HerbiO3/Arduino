#line 1 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\src\\Section.cpp"
#include "Section.hpp"

extern Entity **entites;


Section::Section() {}

Section::Section(byte id,const char* name, byte min_humid, time_t water_time, Valve *valve, MoistureSensor *moisture) : Entity(id,name){
  this->min_humid  = min_humid;
  this->water_time = water_time;
  this->valve = valve;
  this->moisture = moisture;
}


JsonObject Section::toJson(JsonDocument& doc) {
  JsonObject json = Entity::toJson(doc);
  json["water_time"]    = this->water_time ;
  json["water_until"]   = this->water_until ;
  json["water_watered"] = this->watered ;

  json["water_start"] = this->water_start;
  json["water_next"]  = this->water_next ;
  json["min_humidity"]= this->min_humid  ;


  if(valve)
    json["valve"] = valve->toJson(doc);
  else Serial.println("Valve is null");
  if(moisture)
    json["moisture"] = moisture->toJson(doc);
  else Serial.println("moisture is null");
  return json;
}

boolean Section::update(JsonObject &obj){
  if(! Entity::update(obj))
    return false;
  this->water_time = obj["water_time"]  .as<uint32_t>();
  this->water_start= obj["water_start"] .as<uint32_t>();
  this->water_next = obj["water_next"]  .as<uint32_t>();
  this->min_humid  = obj["min_humidity"].as<uint32_t>();

  this->valve     = (Valve*) getEntity(global_entites,obj["valve_id"].as<int8_t>());
  this->moisture  = (MoistureSensor*) getEntity(global_entites,obj["moisture_id"].as<int8_t>());


  obj["min_humidity"].as<uint32_t>();
  
  const char* new_mode = obj["mode"].as<const char*>();  // MODE EnumType.String
  if( strcmp(new_mode,"auto"))  this->mode = AUTO;
  if( strcmp(new_mode,"timed"))  this->mode = TIMED;
  if( strcmp(new_mode,"manual"))  this->mode = MANUAL;



  return true;
}
void Section::water(time_t curr_time){
  water_until = curr_time + water_time;
  valve->open();
  watered = true;
}
void Section::checkAndStopWater(time_t curr_time){
  if(!watered) return; //not started or, valve was not opened
  if(curr_time > water_until)
  valve->close();
  watered = true;
}
// a <= b <= c
bool isBetween(time_t a,time_t b, time_t c){return a<=b && b<=c;}
void Section::action(time_t curr_time){
  //TIME REFRESH
  if(mode == TIMED){

    //begin watering  on water_start
    if(isBetween(water_start,curr_time,water_next)){
      if(watered){
        return;
      }
    }
    else if( curr_time > water_next ){  // need to calculate new range
      time_t len = water_next - water_start;
      int k=1;  //shift once
      while((water_start + k * len) < curr_time) 
        k++;// find how many spans were skipped
      water_start += k*len; //shift k times
      water_next  += k*len;
      
      //water only if RTC is near water_start+10 (seconds)   
      if(isBetween(water_start, curr_time, water_start+10)){
        watered=false;
      } else watered = true;
    }
    else {
      water(curr_time);
    }
    checkAndStopWater(curr_time);
  }



  //WATER IF NEEDED
  if(mode == AUTO){
    if(min_humid >= moisture->measure())
      water(curr_time);
    checkAndStopWater(curr_time);
  }

  if(mode == MANUAL){
    ;
  }

}

void Section::dump(byte* buffer){
  memcpy(buffer, this, sizeof(Section));
}
void Section::load(byte* buffer) {
  memcpy(this, buffer, sizeof(Section));
}