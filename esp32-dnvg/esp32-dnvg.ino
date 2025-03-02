typedef struct config_t {
  byte rotation;
  byte filter;
  byte wb;
  byte ver = 1;
} Config_t;

Config_t config;

// Include libs
#include "./config.h"
#include <TFT_eSPI.h>  // Hardware-specific library
#include <SPI.h>

#include "esp_log.h"

// TFT
TFT_eSPI tft = TFT_eSPI();  // Invoke custom library
TFT_eSprite spr = TFT_eSprite(&tft);
TFT_eSprite menuSpr = TFT_eSprite(&tft);
uint16_t *scr;
long initalTime = 0, frameTime = 1;
volatile bool screenRefreshFlag = true, camSettingsChangedFlag = true;


bool menuShown = false;
bool cameraEnable = true;

void loopMenu();
void setupMenu();
void setupCamera();
void drawMenu();

/*    setup   */
void setup() {
  Serial.begin(UART_BAUD);
  Serial.println(F("----------------------------"));
  Serial.println(F("I: ES32-dnvg v1.0 booting..."));
  esp_log_level_set("main", ESP_LOG_WARN);

#ifdef USE_MENU
  setupMenu();
#endif

  // Lcd init
  tft.init();
  tft.setRotation(config.rotation);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLUE, TFT_WHITE);
  scr = (uint16_t *)spr.createSprite(240, 240);
  tft.drawString("Loading...", 105, 105, 2);

  setupCamera();

  delay(200);
  ESP_LOGI("main", "Setup done");
}

//////////////////////////////////
long FPS_timer = 0;
byte fpsC, FPS;
void loop() {

#ifdef USE_MENU
  loopMenu();
#endif

#ifdef USE_OTA
  loopOTA();
#endif

  if (screenRefreshFlag) {  // Refresh display if there is a new image from the camera
    initalTime = millis();
    spr.pushSprite(0, 0);
    screenRefreshFlag = false;
    frameTime = millis() - initalTime;
    fpsC++;
  }

#ifdef USE_MENU
  else if (menuShown) {
    drawMenu();
    if (millis() - FPS_timer > 1000) {
      FPS_timer = millis();
      FPS = fpsC;
      fpsC = 0;
    }
  }
#endif
}