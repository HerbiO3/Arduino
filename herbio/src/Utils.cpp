#include "Utils.hpp"
#include "Entity.hpp"

#define NELEM(x) (sizeof(x)/sizeof(*x))

Entity* getEntity(Entity* entities[], byte id){
  for(byte i=0; i < NELEM(entities)-1; i++){
    if(entities[i]->id == id)
      return entities[i];
  }
}
