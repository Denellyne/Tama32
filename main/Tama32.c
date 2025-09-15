#include "Sprites/Pookiee/Pookiee.h"
#include "Sprites/Sprites.h"
#include "Tama/Tama.h"
#include "u8g2_esp32_hal.h"
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdint.h>
#include <u8g2.h>

// Pins
// SDA - GPIO21
#define PIN_SDA 21
// SCL - GPIO22
#define PIN_SCL 22

static const char *TAG = "Tama32";

u8g2_t *setup() {

  u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
  u8g2_esp32_hal.bus.i2c.sda = PIN_SDA;
  u8g2_esp32_hal.bus.i2c.scl = PIN_SCL;
  u8g2_esp32_hal_init(u8g2_esp32_hal);
  u8g2_t *u8g2 = (u8g2_t *)(malloc(sizeof(u8g2_t)));

  u8g2_Setup_ssd1306_i2c_128x32_univision_f(
      u8g2, U8G2_R0,
      // u8x8_byte_sw_i2c,
      u8g2_esp32_i2c_byte_cb,
      u8g2_esp32_gpio_and_delay_cb); // init u8g2 structure
  u8x8_SetI2CAddress(&u8g2->u8x8, 0x78);

  ESP_LOGI(TAG, "u8g2_InitDisplay");
  u8g2_InitDisplay(u8g2); // send init sequence to the display, display is in
                          // sleep mode after this,

  // ESP_LOGI(TAG, "u8g2_SetFont");
  // u8g2_SetFont(u8g2, u8g2_font_ncenB14_tr);
  ESP_LOGI(TAG, "u8g2_ClearBuffer");
  u8g2_ClearBuffer(u8g2);
  u8g2_SendBuffer(u8g2);
  ESP_LOGI(TAG, "u8g2_SetPowerSave");
  u8g2_SetPowerSave(u8g2, 0); // wake up display
  // Put before Clear Buffer if bug appears

  return u8g2;
}

void app_main(void) {
  ESP_LOGI(TAG, "Hello,starting up!\n");
  u8g2_t *u8g2 = setup();
  Tama *tama = newTama();
  setTamaSprites(tama, 1,
                 newSprite(32, 32, 4, pookie_frame0, pookie_frame1,
                           pookie_frame2, pookie_frame1));

  while (1) {
    ESP_LOGI(TAG, "u8g2_ClearBuffer");
    u8g2_ClearBuffer(u8g2);
    drawSprite(u8g2, *tama->animations, tama->posX, tama->posY);
    ESP_LOGI(TAG, "u8g2_SendBuffer");
    u8g2_SendBuffer(u8g2);
    updatePosition(tama);
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }

  free(u8g2);
  freeTama(tama);
}
