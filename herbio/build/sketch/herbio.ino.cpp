#include <Arduino.h>
#line 1 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
#include <DallasTemperature.h>
#include <ArduinoJson.h>
//#include <OneWire.h>
//#include <Wire.h>
#include "RTClib.h"
#include <EEPROM.h>

#define DEBUG 1
//#include <Arduino.h>
//#include <Thread.h>
//#include <StaticThreadController.h>


//======Custom objects=============//
#include "src/Entity.hpp"      //
#include "src/Measurable.hpp"  //
#include "src/MoistSensor.hpp" //
#include "src/Pump.hpp"        //
#include "src/Section.hpp"     //
#include "src/Tank.hpp"        //
#include "src/TempSensor.hpp"  //
#include "src/UvSensor.hpp"    //
#include "src/Valve.hpp"       //
//=================================//

#define NAME_LEN_MAX 20
#define EEPROM_ENTITIES_WRITTEN_FLAG_ADDRESS 0  // signal that entities were saved into memory
#define EEPROM_ENTITIES_OFFSET 10

RTC_DS1307 rtc;


// TODO:  STATE->MEMORY->INIT, updates 

//======== Entities for the system =============================//
TempSensor* themp;                          //
MoistureSensor* moist0;
MoistureSensor* moist1;
MoistureSensor* moist2;
MoistureSensor* moist3;
Tank* tank            ; 
Pump* pump            ;           //
UVsensor* uvSensor    ; 
Valve* valve0         ;
Valve* valve1         ;
Valve* valve2         ;
Valve* valve3         ;
Section* section0     ;
Section* section1     ;
Section* section2     ;
Section* section3     ;
byte buttonPin = 12   ;       
                                     //
Entity *all_ents[17]= {0};
                                                                //
//==============================================================//

#line 58 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
void initObjs();
#line 86 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
void load_entities();
#line 95 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
void update_EEPROM_entities();
#line 106 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
void setup();
#line 131 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
void loop();
#line 153 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
void command_get_names();
#line 168 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
void _print_entity(Entity *e);
#line 189 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
bool startsWith(const char* base, const char* start);
#line 192 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
byte atob(const char* str);
#line 201 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
byte execComand(const char* cmd);
#line 244 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
bool update_entity();
#line 256 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
void checkSerial();
#line 58 "/home/nixer/Desktop/arduino/herbio/herbio.ino"
void initObjs(){
  all_ents[0] = themp      = new TempSensor(8,7,"thermo"); 
  all_ents[1] = moist0     = new MoistureSensor(0,A0,"moist_A0");
  all_ents[2] = moist1     = new MoistureSensor(1,A1,"moist_A1");
  all_ents[3] = moist2     = new MoistureSensor(2,A2,"moist_A2");
  all_ents[4] = moist3     = new MoistureSensor(3,A3,"moist_A3");
  all_ents[5] = tank       = new Tank(5,"tank",3,2); 
  all_ents[6] = pump       = new Pump(6,4,"pump");                              
  all_ents[7] = uvSensor   = new UVsensor(7,A4,"uv_7");
  all_ents[8] = valve0      = new Valve(10,6,  "valve0",pump);
  all_ents[9] = valve1      = new Valve(11,8, "valve1",pump);
  all_ents[10] = valve2     = new Valve(12,9, "valve2",pump);
  all_ents[11] = valve3     = new Valve(13,10,"valve3",pump);
  all_ents[12] = section0   = new Section(20, "section0",20,90,valve0, moist0);
  all_ents[13] = section1   = new Section(21, "section1",20,90,valve1, moist1);
  all_ents[14] = section2   = new Section(22, "section2",20,90,valve2, moist2);
  all_ents[15] = section3   = new Section(23, "section3",20,90,valve3, moist3);

  section0->global_entites = all_ents;
  section1->global_entites = all_ents;
  section2->global_entites = all_ents;
  section3->global_entites = all_ents;
  pinMode(buttonPin, INPUT);

}



void load_entities(){
  byte EEcursor = EEPROM_ENTITIES_OFFSET;
  for(byte i=0; all_ents[i]!= nullptr; i++){ //iterate entities
      byte len = all_ents[i]->size();     //get size of entity
      for(byte ent_curr = 0; ent_curr < len; ent_curr++){ //for every byte of entity = EEPROM.read()
          *(((char*)all_ents[i])+ent_curr) = EEPROM.read(EEcursor++); //copies memory into bytes of entity
      }
  }
}
void update_EEPROM_entities(){
  byte EEcursor = EEPROM_ENTITIES_OFFSET;
  for(int i = 0; all_ents[i]!= nullptr; i++){ //iterate entities
    byte len = all_ents[i]->size();   //get size of entity
    for(byte ent_curr = 0; ent_curr < len; ent_curr++){ //for every byte of entity update EEPROM
      EEPROM.update(EEcursor++,*(((char*)all_ents[i])+ent_curr)); // writing value costs lifespan, .update() writes only if needed
    }
  }
  EEPROM.update(EEPROM_ENTITIES_WRITTEN_FLAG_ADDRESS,1);
}

void setup()
{
  
  Serial.begin(9600);
  while(!Serial.available());
  initObjs();
  EEPROM.begin();
  //EEPROM.update(EEPROM_ENTITIES_WRITTEN_FLAG_ADDRESS, -1);
  if(EEPROM.read(EEPROM_ENTITIES_WRITTEN_FLAG_ADDRESS) == 1){ //update only if entities were written first
    Serial.println("loading EEPROM");
    load_entities();
    Serial.println("ok");
  }
  
  Wire.begin();
  rtc.begin();
  //rtc.adjust(DateTime(__DATE__, __TIME__)); // set RTC time to compile time

  // setup pins
  // declare relay as output
  // declare switch as input
  
}


void loop(){
  Serial.println("im alive");
  //sprintf(timestamp, "%02d/%02d/%02d-%02d:%02d:%02d", rtc.now().year(), rtc.now().month(), rtc.now().day(), rtc.now().hour(), rtc.now().minute(),rtc.now().second()); // format timestamp
  Serial.println(rtc.now().timestamp());
  checkSerial();

  section0->action(rtc.now().secondstime());
  section1->action(rtc.now().secondstime());
  section2->action(rtc.now().secondstime());
  section3->action(rtc.now().secondstime());

  delay(DEBUG*2000);

  if(rtc.now().secondstime() % 10){ //every 10 seconds update entities
    update_EEPROM_entities();
  }

}



StaticJsonDocument<512> doc;
void command_get_names(){ // total: 559;  po2: 768  (closest power of 2)
  //Serial.println("getting names...");
  doc.clear();
  Entity *e;
  Entity **it = all_ents;
  while((e = *it)){
    JsonObject obji = doc.createNestedObject();
    obji["id"] = e->id;
    obji["name"] = e->name;
    it++;
  }
  serializeJson(doc,Serial);  
  Serial.println();
}

void _print_entity(Entity *e){
  Measurable *m = e->getMeasurable();
  if(m != nullptr)
    m->measure();
  doc.clear();
  e->toJson(doc);
  serializeJson(doc,Serial);
  Serial.println();
}

#define No_WORDS 10
/*
void command_get_entity(byte id){
  Entity *e = getEntity(all_ents,id);
  if(!e){
    Serial.print("Not found id: ");
    Serial.print(id);
  }
  else _print_entity(e);
}
*/
bool startsWith(const char* base, const char* start){
    return !strncmp(base,start, strlen(start));
}
byte atob(const char* str) {
  byte result = 0;
  while (*str >= '0' && *str <= '9') {
    result = (result * 10) + (*str - '0');
    str++;
  }
  return result;
}
//puts after every word \0 and do command
byte execComand(const char* cmd){
  //Name begins after last controll keyword

  // "TOKENIZE STRING" 
  // strings points to first letter of word
  char *words[No_WORDS];
  byte nWords = 1;
  byte len = strlen(cmd);
  words[0] = cmd;
  for(byte i=1; i < len;i++){  
    if(cmd[i]== ' '){
      words[nWords++] = cmd+i+1;
    }
  }
  Serial.println("chck2");
  if(startsWith(words[0],"get")){  // command get <NAME_OF_ENTITY> prints Json of entity to Serial.
    if(nWords >=1 && startsWith(words[1],"all")){
      //Serial.println("getting all entities");
      command_get_names();
      return true;
    }
    else if(nWords >=1 && startsWith(words[1],"time")){
      Serial.println(rtc.now().timestamp());
      return true;
    }
    else if(nWords >=1){ // TREAT AS ID      
      byte id = atob(words[1]); //simple atobyte for 2digit
      Serial.println("chck3");
      Serial.println(id);
      _print_entity(getEntity(all_ents,id));
      return true;
    } else {Serial.println("err");}
  }

  if(strcmp(words[0],"set") ==0){
    if(strcmp(words[1],"time") ==0){
      rtc.adjust(DateTime(words[2]));
      return true;
    }
  }
  return false;
}

bool update_entity(){
  JsonObject incomingObj= doc.as<JsonObject>();
  //tries to update every entity, if succeeds breaks, i = last updated entity
  for(byte i = 0; all_ents[i] != nullptr; i++){
    Serial.println(all_ents[i]->id);
    if(all_ents[i]->update(incomingObj)){
      return true;
    }
  }
  return false;
}

void checkSerial(){
  
  if (!Serial.available())
    return;
  doc.clear();                        // clears the document for receiving/builing a new one
  DeserializationError err = deserializeJson(doc,Serial);
  switch (err.code()) {
    case DeserializationError::Ok:
        if(update_entity()){
          Serial.println("ok");
        }
        break;
    case DeserializationError::InvalidInput:
        char buff[100]={0};
        Serial.readBytes(buff, sizeof(buff));
        Serial.println("chck1");
        if(execComand(buff)){
          Serial.println("ok");
        } else Serial.println("err");

        break;
    default:
      Serial.println("err");
  }

}

