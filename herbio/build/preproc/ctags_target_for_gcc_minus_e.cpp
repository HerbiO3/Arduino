# 1 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino"
# 2 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino" 2
# 3 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino" 2
//#include <OneWire.h>
//#include <Wire.h>
# 6 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino" 2
# 7 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino" 2

//#include <Arduino.h>
//#include <Thread.h>
//#include <StaticThreadController.h>


//======Custom objects=============//
# 15 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino" 2

# 17 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino" 2
# 18 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino" 2
# 19 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino" 2
# 20 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino" 2
# 21 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino" 2
# 22 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino" 2
# 23 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\herbio.ino" 2
//=================================//



//RTC_DS1307 rtc;


extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory() {
  int free_memory;
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__heap_start);
    Serial.print((int)&free_memory,16);
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval);
    free_memory += (int)(&__brkval);
  }
  return free_memory;
}

// TODO:  STATE->MEMORY->INIT, updates 

//======== Entities for the system =============================//
//TempSensor themp(8,7,"thermometer");                          //
MoistureSensor* moist0;
MoistureSensor* moist1;
MoistureSensor* moist2;
MoistureSensor* moist3;
Tank* tank ;
Pump* pump ; //
UVsensor* uvSensor ;
Valve* valve0 ;
Valve* valve1 ;
Valve* valve2 ;
Valve* valve3 ;
Section* section0 ;
Section* section1 ;
Section* section2 ;
Section* section3 ;
byte buttonPin = 12; //
Entity *all_ents[18]{ //
  moist0, moist1, moist2, moist3, //
  valve0, valve1, valve2, valve3, //
  tank, pump, uvSensor, //
  /*themp,*/ //
  section0, //
  section1, //
  section2, //
  section3, //
  nullptr //
}; //
                                                                //
//==============================================================//

void printRAMstate(){
  int f = freeMemory();
  Serial.print("free memory: ");
  Serial.println(f);
}

void initObjs(){

  delay(1000);
  Serial.println("init Moisture:");
  printRAMstate();
  moist0 = new MoistureSensor(1,A0,"moist_A0");
  moist1 = new MoistureSensor(2,A1,"moist_A1");
  moist2 = new MoistureSensor(3,A2,"moist_A2");
  moist3 = new MoistureSensor(4,A3,"moist_A3");
  delay(2000);
  Serial.println("init util:");
    printRAMstate();
  tank = new Tank(5,"tank",3,2);
  pump = new Pump(6,4,"pump");
  uvSensor = new UVsensor(7,A4,"uv_7");
  delay(2000);
  Serial.println("init valve:");
    printRAMstate();
  valve0 = new Valve(9,6, "valve0",pump);
  valve1 = new Valve(10,8, "valve1",pump);
  valve2 = new Valve(11,9, "valve2",pump);
  valve3 = new Valve(12,10,"valve3",pump);
  delay(2000);
  Serial.println("init Section:");
  printRAMstate();
  section0 = new Section(20, "section0",20,90,valve0, moist0);
  section1 = new Section(21, "section1",20,90,valve1, moist1);
  section2 = new Section(22, "section2",20,90,valve2, moist2);
  section3 = new Section(23, "section3",20,90,valve3, moist3);
  section0->global_entites = all_ents;
  section1->global_entites = all_ents;
  section2->global_entites = all_ents;
  section3->global_entites = all_ents;
  Serial.println("init button");
  printRAMstate();
  pinMode(buttonPin, 0x0);
  Serial.println("init DONE...");

}



void setup()
{
  Serial.begin(9600);
  while(Serial.available());
  delay(500);
  printRAMstate();
  delay(500);
  initObjs();
  Serial.println("loading EEPROM state");


  EEPROM.begin();




  //Wire.begin();
  //rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // set RTC time to compile time

  // setup pins
  // declare relay as output
  // declare switch as input

}

char timestamp[20];
unsigned long watchdog=0;
void loop(){
  watchdog++;
  if(watchdog % 20000 == 0){
    Serial.print("watchdog: ");
    Serial.println(watchdog);
    Serial.println("im alive");
    //sprintf(timestamp, "%02d/%02d/%02d-%02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(),now.second()); // format timestamp
    //Serial.print(rtc.now().secondstime());
    //Serial.println(timestamp);

  }
  checkSerial();
/*
  section0.action(rtc.now().secondstime());
  section1.action(rtc.now().secondstime());
  section2.action(rtc.now().secondstime());
  section3.action(rtc.now().secondstime());
*/
  // delay(2000);
}



StaticJsonDocument<768> doc;
void command_get_names(){ // total: 559;  po2: 768  (closest power of 2)
  Serial.println("getting names...");
  doc.clear();
  Entity *e;
  Entity **it = all_ents;
  while((e = *it)){
    e->toJson(doc);
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

void command_get_entity(byte id){
  Entity *e = getEntity(all_ents,id);
  if(!e){
    Serial.print("Not found id");
    Serial.print(id);
  }
  _print_entity(e);
}
byte atob(const char* str) {
  byte result = 0;
  while (*str >= '0' && *str <= '9') {
    result = (result * 10) + (*str - '0');
    str++;
  }
  return result;
}

bool startsWith(const char* base, const char* start){
    return !strncmp(base,start, strlen(start));
}

//puts after every word \0 and do command
void execComand(const char* cmd){
  //Name begins after last controll keyword

  // "TOKENIZE STRING" 
  // strings points to first letter of word
  char *words[10];
  byte nWords = 1;
  byte len = strlen(cmd);
  words[0] = cmd;
  for(byte i=1; i < len;i++){
    if(cmd[i]== ' '){
      words[nWords++] = cmd+i+1;
    }
  }
  if(startsWith(words[0],"get")){ // command get <NAME_OF_ENTITY> prints Json of entity to Serial.
    if(nWords >=1 && startsWith(words[1],"all")){
      Serial.print("getting all entities");
      command_get_names();
    }
    else if(nWords >=1 && startsWith(words[1],"time")){
      char buff[30];
     // rtc.now().toString(buff);
      Serial.println(buff);
    }
    else if(nWords >=1){ // TREAT AS ID      
      byte id = atob(words[1]); //atoi for 2digit
      Serial.print("Gettig entity ");
      Serial.println(id);
      command_get_entity(id);
    } else {Serial.println("neplatny prikaz");}
  }

  if(strcmp(words[0],"set") ==0){
    if(strcmp(words[1],"time") ==0){
    /*  if(strcmp(words[2],"iso") ==0)
        rtc.adjust(DateTime(words[2]));
      if(strcmp(words[2],"epoch") ==0)
        rtc.adjust(DateTime(atol(words[2])));*/
    }
  }
}

void update_entity(){
  JsonObject incomingObj= doc.as<JsonObject>();
  //tries to update every entity, if succeeds breaks, i = last updated entity
  for(byte i = 0; all_ents[i] != nullptr; i++)
    if(all_ents[i]->update(incomingObj))
      break;
}

void checkSerial(){

  if (!Serial.available())
    return;
  doc.clear(); // clears the document for receiving/builing a new one
  DeserializationError err = deserializeJson(doc,Serial);
  switch (err.code()) {
    case DeserializationError::Ok:
        Serial.print("Entity update");
        update_entity();
        break;
    case DeserializationError::InvalidInput:
        char buff[100]={0};
        Serial.readBytes(buff, sizeof(buff));
        execComand(buff);
        break;
    default:
      Serial.print("bad input\n");
  }

}
