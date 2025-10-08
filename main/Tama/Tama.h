#ifndef TAMA
#define TAMA

#include "esp_log.h"

#include "../Sprites/Sprites.h"
#include <stdarg.h>
enum states { idle = 0, evolve, hungry, poop, refuse };

#define TAMA_EGG 0
#define TAMA_CHILD 1
#define TAMA_TEEN 4
#define TAMA_ADULT 8
#define TAMA_ELDER 12
#define TAMA_DEAD 14

#define TAMA_MAX_SICK_DAYS 3
#define TAMA_DIRTY 210
#define TAMA_UNDERWEIGHT 3
#define TAMA_OVERWEIGHT 30

typedef struct {

  u8 hunger, weight, discipline, poop, age, happiness, daysSick, numSprites,
      posX, posY;
  bool direction, badEvolve, sick;
  Sprite **sprites;
} Tama;

Tama *newTama();
void freeTama(Tama *tama);

Tama *newTama() {
  Tama *tama = (Tama *)malloc(sizeof(Tama));
  if (tama == NULL)
    return NULL;

  tama->hunger = 90;
  tama->weight = 3;
  tama->happiness = 50;

  tama->discipline = tama->poop = tama->daysSick = tama->age =
      tama->numSprites = tama->posX = tama->direction = tama->badEvolve =
          tama->sick = 0;
  tama->posY = 16;
  return tama;
}
Tama *newTamaEX(u8 hunger, u8 weight, u8 happiness, u8 discipline, u8 poop,
                u8 daysSick, u8 age, bool badEvolve, bool sick) {
  Tama *tama = (Tama *)malloc(sizeof(Tama));
  if (tama == NULL)
    return NULL;
  tama->sick = sick;
  tama->badEvolve = badEvolve;
  tama->age = age;
  tama->daysSick = daysSick;
  tama->discipline = discipline;
  tama->poop = poop;
  tama->happiness = happiness;
  tama->hunger = hunger;
  tama->weight = weight;
  tama->posX = tama->numSprites = tama->direction = 0;
  tama->posY = 16;
  return tama;
}

void setTamaSprites(Tama *tama, int numSprites, ...) {

  for (int i = 0; i < tama->numSprites; i++)
    freeSprite(tama->sprites[i]);

  tama->numSprites = numSprites;
  va_list argptr;
  va_start(argptr, numSprites);

  ESP_LOGI("Tama.h", "Setting Tama animations");
  for (int counter = 0; counter < numSprites; counter++)
    tama->sprites[counter] = va_arg(argptr, Sprite *);

  ESP_LOGI("Tama.h", "Tama animations set");
  va_end(argptr);
}

Tama *evolveTama(Tama *tama) {
  if (tama->age >= TAMA_DEAD || tama->daysSick >= TAMA_MAX_SICK_DAYS) {
    ESP_LOGI("Tama.h", "Tama died");
    freeTama(tama);
    return NULL;
  }

  if (tama->badEvolve)
    return tama;

  if (tama->poop >= TAMA_DIRTY) {
  }
  if (tama->weight >= TAMA_OVERWEIGHT) {
  }
  if (tama->weight >= TAMA_UNDERWEIGHT) {
  }
  switch (tama->age) {
  case TAMA_CHILD: {
    Tama *evolved = newTamaEX(150, 4, 20, 20, 0, 0, tama->age, 0, 0);
    freeTama(tama);
    return evolved;
  } break;
  case TAMA_TEEN:  // Teen
  case TAMA_ADULT: // Adult
  case TAMA_ELDER: // Elder
  {

    Tama *evolved = newTamaEX(tama->hunger, tama->weight, tama->happiness,
                              tama->discipline, tama->poop, tama->daysSick,
                              tama->age, tama->badEvolve, tama->sick);
    freeTama(tama);
    return evolved;
  } break;
  default:
    return tama;
    break;
  }
  return tama;
}

void updateTamaNeeds(Tama *tama) {
  if (tama->age < 1)
    return;
  int needs = rand();

  tama->hunger =
      (tama->hunger + (needs % 10) <= 255) * (tama->hunger + (needs % 10)) +
      (tama->hunger + (needs % 10) > 255) * 255;
  needs >>= 1;

  tama->discipline = (tama->discipline - (needs % 10) >= 0) *
                     (tama->discipline - (needs % 10));
  needs >>= 1;

  tama->poop =
      (tama->poop + (needs % 10) <= 255) * (tama->poop + (needs % 10)) +
      (tama->poop + (needs % 10) > 255) * 255;
  needs >>= 1;

  tama->happiness =
      (tama->happiness - (needs % 10) >= 0) * (tama->happiness - (needs % 10));
  needs >>= 1;

  int sickness = rand() % 255;
  if (sickness <= (1 + (tama->poop >> 2)))
    tama->sick = true;
}

void freeTama(Tama *tama) {

  for (int i = 0; i < tama->numSprites; i++)
    freeSprite(tama->sprites[i]);
  free(tama);
}
void updateTamaPosition(Tama *tama) {
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
