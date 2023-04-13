#pragme once
#include <ArduinoJson.h> 
#include <Measurable.h>

class Entity  {
  public: 
  byte id;
  String name;
  virtual boolean update(JsonObject &doc) = 0;
  virtual Measurable* getMeasurable(){return nullptr;};  // returns (this) as Measurable, or nullptr.  shortcut for -fno-rtti (no dymamic_cast) 

  virtual JsonObject toJson(JsonDocument& doc) {
    JsonObject json = doc.createNestedObject();
    json["id"] = this->id;
    json["name"] = this->name;
    return json;
  }
};