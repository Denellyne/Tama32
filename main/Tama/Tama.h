#ifndef TAMA
#define TAMA

#include "esp_log.h"
#define YEAR_IN_HOURS 24
#define MAX_AGE 9

#include "../Sprites/Sprites.h"
#include <stdarg.h>
enum states { idle = 0, evolve, hungry, poop, refuse };

typedef struct {

  u8 hunger, discipline, poop, age, happiness, numSprites, posX, posY;
  bool direction;
  Sprite **animations;
} Tama;

Tama *newTama();
void freeTama(Tama *tama);

Tama *newTama() {
  Tama *tama = (Tama *)malloc(sizeof(Tama));
  if (tama == NULL)
    return NULL;

  tama->hunger = tama->discipline = tama->poop = tama->age = tama->happiness =
      tama->numSprites = tama->posX = tama->direction = 0;
  tama->posY = 16;
  return tama;
}

void setTamaSprites(Tama *tama, int numSprites, ...) {
  tama->numSprites = numSprites;
  va_list argptr;
  va_start(argptr, numSprites);

  ESP_LOGI("Tama.h", "Setting Tama animations");
  for (int counter = 0; counter < numSprites; counter++)
    tama->animations[counter] = va_arg(argptr, Sprite *);

  ESP_LOGI("Tama.h", "Tama animations set");
  va_end(argptr);
}

Tama *evolveTama(Tama *tama) {
  switch (tama->age) {
  case 1:
    // Child

    break;
  case 4: // Teen
    break;
  case 8: // Adult
    break;
  case 12: // Elder
    break;
  case 14: // Dead
    freeTama(tama);
    return newTama();
    break;
  default:
    return tama;
    break;
  }
  return tama;
}

void freeTama(Tama *tama) {

  for (int i = 0; i < tama->numSprites; i++)
    freeSprite(tama->animations[i]);
  free(tama);
}
void updatePosition(Tama *tama) {
  if (tama->direction) {
    if (tama->posX > 0)
      tama->posX -= 2;
    else
      tama->direction = 0;
  } else {
    if (tama->posX < 96)
      tama->posX += 2;
    else
      tama->direction = 1;
  }
}

#endif // TAMA
