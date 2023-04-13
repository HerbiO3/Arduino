#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <Wire.h>
#include "RTClib.h"

//#include <Arduino.h>
#include "pins_arduino.h"
//#include <Thread.h>
//#include <StaticThreadController.h>


//======Custom objects=============//
#include "classes/Entity.h"        //
#include "classes/Measurable.h"    //
#include "classes/MoistSensor.h"   //
#include "classes/Pump.h"          //
#include "classes/Section.h"       //
#include "classes/Tank.h"          //
#include "classes/TempSensor.h"    //
#include "classes/Utils.h"         //
#include "classes/UvSensor.h"      //
#include "classes/Valve.h"         //
//=================================//

#define NAME_LEN_MAX 20

RTC_DS1307 rtc;

//======== Entities for the system ====================//
MoistureSensor moist0(1,A0,"moist_A0",30,50);          //
MoistureSensor moist1(2,A1,"moist_A1",30,50);          //
MoistureSensor moist2(3,A2,"moist_A2",30,50);          //
MoistureSensor moist3(4,A3,"moist_A3",30,50);          //
                                                       //
Tank tank(5,"g_tank",3,2);                             //
Pump pump(6,4,"g_pump",60);                            //
UVsensor uv(7,A4,"uv_sensor");                         //
TemperatureSensor themp(8,7,"thermometer");            //
                                                       //
Valve valve0(9,6,  "valve0");                          //
Valve valve1(10,8, "valve1");                          //
Valve valve2(11,9, "valve2");                          //
Valve valve3(12,10,"valve3");                          //
Section section0(20,"section0",20,90,&valve0,&moist0); //
Section section1(20,"section0",20,90,&valve1,&moist1); //
Section section2(20,"section0",20,90,&valve2,&moist2); //
Section section3(20,"section0",20,90,&valve3,&moist3); //
                                                       //
byte buttonPin = 12;                                   //
Entity *entities[]{                                    //
  &moist0, &moist1, &moist2, &moist3,                  //
  &valve0, &valve1, &valve2, &valve3,                  //
  &tank, &pump, &uv, &themp,                           //
  &section0,                                           //
  &section1,                                           //
  &section2,                                           //
  &section3                                            //
};                                                     //
//=====================================================//

void setup()
{
  delay(1000);
  pinMode(buttonPin, INPUT);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // set RTC time to compile time

  // setup pins
  Serial.begin(9600);
  // declare relay as output
  // declare switch as input
  
}

char timestamp[20];
void loop(){

  // Serial.println("im alive");
  // DateTime now = rtc.now(); // get current time
  // sprintf(timestamp, "%02d/%02d/%02d-%02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(),now.second()); // format timestamp
  // Serial.println(timestamp);
  checkSerial();

  // delay(2000);
}

Entity* getEntity(const char *name){
  for(byte i=0; i < NELEM(entities)-1; i++){
    if(entities[i]->name.equals(name))
      return entities[i];
  }
}


StaticJsonDocument<768> doc;

void command_get_names(){ // total: 559;  po2: 768  (closest power of 2)
  doc.clear();
  for(byte i=0; i < NELEM(entities)-1; i++){
    JsonObject doc_i = doc.createNestedObject();
    doc_i["id"] = entities[i]->id;
    doc_i["name"] = entities[i]->name;
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
void command_get_entity(const char *name){
  Entity *e = getEntity(name);
  _print_entity(e);
}
void command_get_entity(byte id){
  Entity *e = getEntity(id);
  _print_entity(e);
}

bool startsWith(const char* base, const char* start){
    return !strncmp(base,start, strlen(start));
}

//puts after every word \0 and do command
void execComand(const char* cmd){
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
  if(startsWith(words[1],"get")){  // command get <NAME_OF_ENTITY> prints Json of entity to Serial.
    if(startsWith(words[2],"-all")){
      command_get_names();
    }
    else{ // TREAT AS NAME      
      //command_get_entity(strings[2]);
      byte id = atoi(words[2]);
      command_get_entity(id);
    }
  }
  if(strcmp(words[1],"set") ==0){
    if(strcmp(words[2],"time") ==0){

    }
  }
}

void checkSerial(){
  if (!Serial.available())
    return;
  doc.clear();                        // clears the document for receiving/builing a new one
  String input = Serial.readString();
  
  if(input.startsWith("command")){    // COMMAND found
    execComand(input.c_str());
  }
  else if(input[0]=='{' || input[0]=='['){ // JSON found, lets deserialize it
    DeserializationError err = deserializeJson(doc,input);
    if(!err){
      Serial.print("JSON parsing error: ");
      Serial.println(err.c_str());
    }


    JsonObject jo= doc.as<JsonObject>();    
    byte entityFound = -1;
    for(byte i=0; i < NELEM(entities)-1; i++){    // search through entities if some to be updated
      if(entities[i]->update(jo)){ // if update sucseed, break
        entityFound = i;
        break;
      }
    }
    if(entityFound == -1){
      Serial.println("error: no entity found (by \"id\") to be updated");
    }
    else{
      _print_entity(entities[entityFound]);
    }
  }
}
