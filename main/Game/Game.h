#ifndef GAME
#define GAME

#define U8G2_HEIGHT 64
#define U8G2_WIDTH 128
#define DEATH_TAMA 0
#define NEW_TAMA 1
#include "../Sprites/spritesData.h"
#include "../Tama/Tama.h"
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
                 newSprite(SPRITE_HEIGHT, SPRITE_WIDTH, 4, pookie0, pookie1,
                           pookie2, pookie1));

  return game;
}

void freeGame(Game *game) {
  freeTama(game->tama);
  free(game);
}
u8 getTimePassed() { return 3; }

void playAnimation(u8g2_t *u8g2, int animation) {
  switch (animation) {
  case DEATH_TAMA: // Death
    u8g2_ClearBuffer(u8g2);
    u8g2_SetDrawColor(u8g2, 0);
    // u8g2_DrawXBM(u8g2,32,64, 32, 32, );
    u8g2_SendBuffer(u8g2);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    break;
  case NEW_TAMA: // New Tama
    u8g2_ClearBuffer(u8g2);
    u8g2_SetDrawColor(u8g2, 0);
    u8g2_DrawBox(u8g2, 0, 0, U8G2_WIDTH, U8G2_HEIGHT);
    u8g2_SendBuffer(u8g2);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    u8g2_ClearBuffer(u8g2);
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawBox(u8g2, 0, 0, U8G2_WIDTH, U8G2_HEIGHT);
    u8g2_SendBuffer(u8g2);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    u8g2_ClearBuffer(u8g2);
    u8g2_SetDrawColor(u8g2, 0);
    u8g2_DrawBox(u8g2, 0, 0, U8G2_WIDTH, U8G2_HEIGHT);
    u8g2_SendBuffer(u8g2);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    u8g2_ClearBuffer(u8g2);
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawBox(u8g2, 0, 0, U8G2_WIDTH, U8G2_HEIGHT);
    u8g2_SendBuffer(u8g2);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    break;
  default:
    break;
  }
}
void updateGameState(Game *game, u8g2_t *u8g2) {
  ESP_LOGI("Game.h", "Updating game state");
  u8 time = getTimePassed();
  game->hours += ((time >> 1) & 1);

  ESP_LOGI("Game.h", "Happiness:%d ,Poop:%d ,Discipline:%d ,Hunger:%d",
           game->tama->happiness, game->tama->poop, game->tama->discipline,
           game->tama->hunger);

  bool tamaNeeds = (time & 1);

  updateTamaPosition(game->tama);

  if (tamaNeeds)
    updateTamaNeeds(game->tama);

  if (game->hours >= 24) {
    game->hours = 0;
    game->tama->age++;

    if (game->tama->sick)
      game->tama->daysSick++;

    game->tama = evolveTama(game->tama);
  }

  if (game->tama == NULL) {
    game->tama = newTama();
    setTamaSprites(game->tama, 1,
                   newSprite(32, 32, 4, pookie0, pookie1, pookie2, pookie1));
    playAnimation(u8g2, NEW_TAMA);

    u8g2_ClearBuffer(u8g2);
  }

  drawSprite(u8g2, *game->tama->sprites, game->tama->posX, game->tama->posY);
}

#endif
