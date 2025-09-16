#ifndef GAME
#define GAME

#include "../Tama/Tama.h"
#include "Sprites/Pookiee/Pookiee.h"
#include "freertos/idf_additions.h"
#include "u8g2.h"
#include <stdlib.h>
typedef struct {
  // Clock

  Tama *tama;
  u8 hours;
} Game;

Game *newGame() {

  ESP_LOGI("Game.h", "Starting the game");

  Game *game = (Game *)malloc(sizeof(Game));
  game->hours = 0;
  game->tama = newTama();
  setTamaSprites(game->tama, 1,
                 newSprite(32, 32, 4, pookie_frame0, pookie_frame1,
                           pookie_frame2, pookie_frame1));
  return game;
}
void freeGame(Game *game) {
  freeTama(game->tama);
  free(game);
}
u8 getTimePassed() { return 2; }

void playAnimation(u8g2_t *u8g2, int animation) {
  switch (animation) {
  case 0: // Death
    u8g2_ClearBuffer(u8g2);
    u8g2_SetDrawColor(u8g2, 0);
    u8g2_DrawBox(u8g2, 0, 0, 128, 64);
    u8g2_SendBuffer(u8g2);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    u8g2_ClearBuffer(u8g2);
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawBox(u8g2, 0, 0, 128, 64);
    u8g2_SendBuffer(u8g2);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    u8g2_ClearBuffer(u8g2);
    u8g2_SetDrawColor(u8g2, 0);
    u8g2_DrawBox(u8g2, 0, 0, 128, 64);
    u8g2_SendBuffer(u8g2);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    u8g2_ClearBuffer(u8g2);
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawBox(u8g2, 0, 0, 128, 64);
    u8g2_SendBuffer(u8g2);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    break;
  case 1: // New Tama
    break;

  default:
    break;
  }
}
void updateGameState(Game *game, u8g2_t *u8g2) {
  ESP_LOGI("Game.h", "Updating game state");
  u8 time = getTimePassed();
  game->hours += ((time >> 1) & 1);

  ESP_LOGI("Game.h", "%d", game->hours);
  bool tamaNeeds = (time & 1);

  updateTamaPosition(game->tama);

  if (tamaNeeds)
    updateTamaNeeds(game->tama);

  if (game->hours >= 24) {
    game->hours = 0;
    game->tama->age++;
    game->tama = evolveTama(game->tama);
  }

  if (game->tama == NULL) {
    game->tama = newTama();
    setTamaSprites(game->tama, 1,
                   newSprite(32, 32, 4, pookie_frame0, pookie_frame1,
                             pookie_frame2, pookie_frame1));
    playAnimation(u8g2, 0);
    u8g2_ClearBuffer(u8g2);
  }

  drawSprite(u8g2, *game->tama->sprites, game->tama->posX, game->tama->posY);
}

#endif
