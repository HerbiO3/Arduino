#include "Entity.hpp"


Entity::Entity(byte id,const char* name){
  //#ifdef DEBUG
    delay(50);
    Serial.println(name);
  //#endif
    this->id = id;
    this->name = name;
};

Entity* getEntity(Entity** entities, byte id){
  Entity* e;
  while((e = *entities)){
    if(e->id == id){
      return e;
    }
    entities++;
  }
  return nullptr;
}

JsonObject Entity::toJson(JsonDocument& doc){
    JsonObject json = doc.createNestedObject();
    json["id"] = this->id;
    json["name"] = this->name;
    return json;
}

boolean Entity::update(JsonObject &obj) {
  return obj["id"] == this->id;
};

