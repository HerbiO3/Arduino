#include "Entity.hpp"

Entity::Entity(byte id, String name){
    this->id = id;
    this->name = name;
};

JsonObject Entity::toJson(JsonDocument& doc) {
    JsonObject json = doc.createNestedObject();
    json["id"] = this->id;
    json["name"] = this->name;
    return json;
};