#line 1 "C:\\Users\\Matej-Windows\\Desktop\\sem8\\TP2\\arduino\\herbio\\src\\Entity.cpp"
#include "Entity.hpp"


Entity::Entity(byte id,const char* name){
    delay(200);
    Serial.println(name);
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
void Entity::eToJson(JsonObject &json){
    json["id"] = this->id;
    json["name"] = this->name;
}
JsonObject Entity::toJson(JsonDocument& doc){
    JsonObject json = doc.createNestedObject();
    json["id"] = this->id;
    json["name"] = this->name;
    return json;
}

boolean Entity::update(JsonObject &obj) {
  if (obj["id"] != this->id)
    return false;
  //strncpy(this->name, obj[name], NAME_LENGTH); //there is not enough space to allocate for name
  return true;
};

