#ifndef SPRITES
#define SPRITES
#include "esp_log.h"
#include "u8g2.h"
#include <malloc.h>
#include <stdarg.h>
#include <stdint.h>

typedef unsigned short u8;

typedef struct {
  u8 width, height, maxFrame, currentFrame;
  uint8_t **animations;
} Sprite;

Sprite *newSprite(u8 width, u8 height, int maxFrame, ...) {
  Sprite *sprite = (Sprite *)malloc(sizeof(Sprite));
  if (sprite == NULL)
    return NULL;
  va_list argptr;
  va_start(argptr, maxFrame);

  sprite->width = width;
  sprite->height = height;
  sprite->currentFrame = 0;
  sprite->maxFrame = maxFrame - 1;
  sprite->animations = (uint8_t **)malloc(maxFrame * sizeof(uint8_t *));
  for (int counter = 0; counter < maxFrame; counter++)
    sprite->animations[counter] = va_arg(argptr, uint8_t *);

  va_end(argptr);

  return sprite;
}

void freeSprite(Sprite *sprite) {
  // for (int i = 0; i <= sprite->maxFrame; i++)
  //   free(sprite->animations[i]);
  free(sprite->animations);
  free(sprite);
}

void drawSprite(u8g2_t *u8g2, Sprite *sprite, unsigned posX, unsigned posY) {

  ESP_LOGI("Sprites.h", "Drawing sprite\n");

  u8g2_DrawXBM(u8g2, posX, -6, sprite->width, sprite->height,
               sprite->animations[sprite->currentFrame++]);
  if (sprite->currentFrame > sprite->maxFrame)
    sprite->currentFrame = 0;
}
#endif
