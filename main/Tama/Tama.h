#ifndef TAMA
#define TAMA

#include "../Sprites/Sprites.h"
enum states { idle = 0, evolve, hungry, poop, refuse };

typedef struct {

  u8 hunger, discipline, poop, age;
  Sprite **animations;
} Tama;

Tama *newTama() {
  Tama *tama = (Tama *)malloc(sizeof(Tama));
  if (tama == NULL)
    return NULL;

  tama->hunger = tama->discipline = tama->poop = tama->age = 0;
  return tama;
}

#endif // TAMA
