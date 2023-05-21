#include <DallasTemperature.h>
#include <ArduinoJson.h>
//#include <OneWire.h>
//#include <Wire.h>
#include "RTClib.h"
#include <EEPROM.h>


// ifdefs debug options
// #define DEBUG
//#define DEBUG_WATCHDOG
//#define DEBUG_EEPROM


//#include <Arduino.h>
//#include <Thread.h>
//#include <StaticThreadController.h>


//======Custom objects=============//
#include "src/Entity.hpp"          //
#include "src/MoistSensor.hpp"     //
#include "src/Pump.hpp"            //
#include "src/Section.hpp"         //
#include "src/Tank.hpp"            //
#include "src/TempSensor.hpp"      //
#include "src/UvSensor.hpp"        //
#include "src/Valve.hpp"           //
//=================================//

#define NAME_LEN_MAX 20
#define EEPROM_ENTITIES_WRITTEN_FLAG_ADDRESS 0  // signal that entities were saved into memory
#define EEPROM_ENTITIES_OFFSET 20  //size 400B
#define EEPROM_RTC_OFFSET 10 // size 2B
#define No_WORDS 10

#define CMD_failed 0
#define CMD_get_all 1
#define CMD_get_time 2
#define CMD_get_dump 3
#define CMD_get_id 4
#define CMD_set_time 5
#define CMD_set_open 6
#define CMD_set_close 6
#define CMD_set_invalid 7
#define CMD_set_empty 8
#define CMD_set_num 9
#define CMD_set_dump 10
#define CMD_set_from_init 11
#define CMD_set_from_eeprom 12
#define CMD_updateJson 13
#define CMD_exit 99

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
#define ENITIES 17
Entity *all_ents[ENITIES]= {0};
                                                                //
//==============================================================//
//not called
void initObjs(){
  byte i =0;
//  all_ents[0] = themp       = new TempSensor(8,7,"thermo"); 
  all_ents[i++] = moist0     = new MoistureSensor(0,A0,"moist_A0");
  all_ents[i++] = moist1     = new MoistureSensor(1,A1,"moist_A1");
  all_ents[i++] = moist2     = new MoistureSensor(2,A2,"moist_A2");
  all_ents[i++] = moist3     = new MoistureSensor(3,A3,"moist_A3");
  all_ents[i++] = tank       = new Tank(5,"tank",3,2); 
  all_ents[i++] = pump       = new Pump(6,4,"pump");
  all_ents[i++] = uvSensor   = new UVsensor(7,A4,"uv_7");
  all_ents[i++] = valve0     = new Valve(10,6,  "valve0",pump);
  all_ents[i++] = valve1     = new Valve(11,8, "valve1",pump);
  all_ents[i++] = valve2     = new Valve(12,9, "valve2",pump);
  all_ents[i++] = valve3     = new Valve(13,10,"valve3",pump);
  all_ents[i++] = section0   = new Section(20, "section0",20,90,all_ents,valve0->id, moist0->id);
  all_ents[i++] = section1   = new Section(21, "section1",20,90,all_ents,valve1->id, moist1->id);
  all_ents[i++] = section2   = new Section(22, "section2",20,90,all_ents,valve2->id, moist2->id);
  all_ents[i++] = section3   = new Section(23, "section3",20,90,all_ents,valve3->id, moist3->id);
  #ifdef DEBUG 
    Serial.println("objects initialized");
  #endif
  section0->global_entites = all_ents;
  section1->global_entites = all_ents;
  section2->global_entites = all_ents;
  section3->global_entites = all_ents;
  pinMode(buttonPin, INPUT);
}



void load_entities(){
  int EEcursor = EEPROM_ENTITIES_OFFSET;
  for(byte i=0; all_ents[i]!= nullptr; i++){ //iterate entities
      byte buff[sizeof(Section)+4];
      #ifdef DEBUG_EEPROM
        Serial.print("loading: ");
        Serial.print(all_ents[i]->name);
        Serial.print("  size: ");
        Serial.print(all_ents[i]->size());
        Serial.print("  ");
      #endif
      char tmp[4];
      for(byte j = 0; j < all_ents[i]->size(); j++){ //for every byte of entity = EEPROM.read()
          buff[j] = EEPROM.read(EEcursor++); //copies memory into bytes of entity
          sprintf(tmp,"%02hhx ",buff[j]);
          Serial.print(tmp);
      }
      Serial.println();
      all_ents[i]->load(buff);
  }
  EEcursor = EEPROM_RTC_OFFSET;
  for(byte i=0; i < sizeof(rtc);i++){
    *(((char*)(&rtc))+i) = EEPROM.read(EEcursor++);
  }
}

void dump_EEPROM_entities(){
  int EEcursor = EEPROM_ENTITIES_OFFSET;
    #ifdef DEBUG_EEPROM
      Serial.print("all_ents[i]!= nullptr   ");
      Serial.println(all_ents[0]!= nullptr);
    #endif
  for(int i = 0; all_ents[i]!= nullptr; i++){ //iterate entities
    #ifdef DEBUG_EEPROM
      Serial.print("dumping: ");
      Serial.println(all_ents[i]->name);
    #endif

    byte buff[sizeof(Section)+4];
    byte s = all_ents[i]->dump(buff);

    #ifdef DEBUG_EEPROM
      Serial.print(s);
      Serial.print(" Bytes dumped: ");
      char tmp[10];
      for(byte j = 0; j < s; j++){
        sprintf(tmp,"%02hhx ",buff[j]);
        Serial.print(tmp);
      }
      Serial.println();

      Serial.print(s);
      Serial.println(" bytes writing");
    #endif

    for(byte j = 0; j < s ; j++){ //for every byte of entity update EEPROM
      EEPROM.update(EEcursor++,buff[j]); // writing value costs lifespan, .update() writes only if needed
      #ifdef DEBUG_EEPROM
        sprintf(tmp, "{%2hhx: %2hhx}\n",EEcursor-1,buff[j]);
        Serial.print(tmp);
      #endif
    }
    #ifdef DEBUG_EEPROM
      Serial.println();
    #endif

  }
  Serial.println();
  EEcursor = EEPROM_RTC_OFFSET;
  for(byte i=0; i < sizeof(rtc);i++){
      EEPROM.update(EEcursor++,*(((char*)(&rtc))+i));
  }
  EEPROM.update(EEPROM_ENTITIES_WRITTEN_FLAG_ADDRESS,1);
}

void eeprom_dump(){
  char tmp[7];
  int cursor = 0;
  for(byte i = 0; i < 20; i++){
    for(byte j = 0; j < 50; j++){
      sprintf(tmp,"%02hhx ",EEPROM.read(cursor++));
      Serial.print(tmp);
    }
    Serial.println();
  }
}

void eeprom_empty(){
  for(int i = 0; i < 1024; i++){
      EEPROM.update(i,(byte)255);
  }
}
byte setNum;
void setup()
{
  EEPROM.begin();
  Serial.begin(9600);
  #ifdef DEBUG
    while(!Serial.available());
  #endif
  Serial.print("version: ");
  Serial.println(101);
  #ifdef DEBUG
    Serial.println("hello");
    valve0->open();
    delay(1000);
    valve0->close();
    Serial.println("pre init commands:");
    while(checkSerial() != CMD_exit);
  #endif
  initObjs();

  if(EEPROM.read(EEPROM_ENTITIES_WRITTEN_FLAG_ADDRESS) == 1){ //update only if entities were written first
    Serial.println("loading EEPROM");
    load_entities();
    Serial.println("ok");
  }
  rtc.begin();

  #ifdef DEBUG
    Serial.print("post init commands:");
    while(checkSerial() != CMD_exit);
  #endif

  // setup pins
  // declare relay as output
  // declare switch as input
  
}
#define UPDATE_TIME()   int EEcursor = EEPROM_RTC_OFFSET;   for(byte i=0; i < sizeof(rtc);i++)     EEPROM.update(EEcursor++,*(((char*)(&rtc))+i))
#define CURRTIME (rtc.now().secondstime())
time_t last_save = 0;
void loop(){
  #ifdef DEBUG
    delay(2000);
    Serial.println("im alive");
    //sprintf(timestamp, "%02d/%02d/%02d-%02d:%02d:%02d", rtc.now().year(), rtc.now().month(), rtc.now().day(), rtc.now().hour(), rtc.now().minute(),rtc.now().second()); // format timestamp
    Serial.println(rtc.now().secondstime());
  #endif

  checkSerial();
  section0->action(CURRTIME);
  section1->action(CURRTIME);
  section2->action(CURRTIME);
  section3->action(CURRTIME);

  if(last_save + 3600 < CURRTIME){
    last_save = CURRTIME;
    UPDATE_TIME();
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
  doc.clear();
  e->toJson(doc);
  serializeJson(doc,Serial);
  Serial.println();
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
byte execComand(char* cmd){
  byte len = strlen(cmd);
  for(byte i=0; i < len; i++)
    if(cmd[i]=='\n' || cmd[i]=='\r')
      cmd[i] = '\0';
  //Name begins after last controll keyword

  // "TOKENIZE STRING" 
  // strings points to first letter of word
  char *words[No_WORDS];
  byte nWords = 1;
  words[0] = cmd;
  
  while(*cmd){
    if(*cmd == ' '){
      words[nWords++] = cmd+1;
      *cmd = 0;
    }
    cmd++;
  }

  if(!strcmp(words[0],"exit")){
    return CMD_exit;
  }
    

  if(!strcmp(words[0],"get")){  // command get <NAME_OF_ENTITY> prints Json of entity to Serial.
    if(nWords >=1 && !strcmp(words[1],"all")){
      command_get_names();
      return CMD_get_all;
    }
    else if(nWords >=1 && !strcmp(words[1],"time")){
      Serial.println(rtc.now().timestamp());
      return CMD_get_time;
    }
    else if(nWords >=1 && !strcmp(words[1],"dump")){
      eeprom_dump();
      return CMD_get_dump;
    }
    else if(nWords >=1){ // TREAT AS ID      
      byte id = atob(words[1]); //simple atobyte for 2digit
      _print_entity(getEntity(all_ents,id));
      return CMD_get_id;
    }
  }

  if(!strcmp(words[0],"set")){
    if(!strcmp(words[1],"time")){
      rtc.adjust(DateTime(words[2]));
      UPDATE_TIME();
      return CMD_set_time;
    }
    if(!strcmp(words[1],"open")){
      Valve* v = (Valve*) getEntity(all_ents,atob(words[2]));
      v->open();
      return CMD_set_open;
    }
    if(!strcmp(words[1],"close")){
      Valve* v = (Valve*) getEntity(all_ents,atob(words[2]));
      v->close();
      return CMD_set_close;
    }
    if(!strcmp(words[1],"invalid")){
      EEPROM.update(EEPROM_ENTITIES_WRITTEN_FLAG_ADDRESS, -1);
      return CMD_set_invalid;
    }
    else if(nWords >=1 && !strcmp(words[1],"empty")){
      eeprom_empty();
      Serial.println("emptied");
      return CMD_set_empty;
    }else if(nWords>= 2 &&!strcmp(words[1],"num")){
        setNum = atob(words[2]);
        return CMD_set_num;
    }
    else if(nWords>= 2 &&!strcmp(words[1],"dump")){
        dump_EEPROM_entities();
        return CMD_set_dump;
    } else if(nWords>= 3 &&!strcmp(words[1],"from")){
        if(nWords>= 3 &&!strcmp(words[2],"eeprom")){
          load_entities();
          return CMD_set_from_eeprom;
        }
        if(nWords>= 3 &&!strcmp(words[2],"init")){
          //free first
          for (int i = 0; i < ENITIES; i++)
            delete all_ents[i];
          
          initObjs();
          return CMD_set_from_eeprom;
        }
    }
  }
  return false;
}

bool update_entity(){
  JsonObject incomingObj= doc.as<JsonObject>();
  //tries to update every entity, if succeeds breaks, i = last updated entity
  for(byte i = 0; all_ents[i] != nullptr; i++){
    if(all_ents[i]->update(incomingObj)){
      return true;
    }
  }
  dump_EEPROM_entities();
  return false;
}

byte checkSerial(){
  byte exitCode = 0;
  if (!Serial.available())
    return exitCode;
  doc.clear();                        // clears the document for receiving/builing a new one
  DeserializationError err = deserializeJson(doc,Serial);
  switch (err.code()) {
    case DeserializationError::Ok:
        if(update_entity()){
          Serial.println("ok");
          exitCode = CMD_updateJson;
        }
        break;
    case DeserializationError::InvalidInput:
        char buff[100]={0};
        Serial.readBytes(buff, sizeof(buff));
        exitCode = execComand(buff);
        if(exitCode){
          Serial.print("ok");
          Serial.print(' ');
          Serial.println(exitCode);
        } else {
          Serial.println("err");
        }
        break;
    default:
      Serial.println("err");
  }
  return exitCode;

}
