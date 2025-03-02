#ifdef USE_MENU

#include "Preferences.h"
Preferences prefs;

#include "./lib/AwesomeClickButton.h"
AwesomeClickButton buttonConfig(BUTTON_PIN);

#ifdef USE_CLI
#include "SimpleCLI.h"
#include "PreferencesCLI.h"

PreferencesCLI prefCli(prefs);
SimpleCLI cli;
void handleCliError(CommandError &e, Stream &outputStream);
#endif


size_t configLen;

uint16_t xw, yh;

// Variables
const String menu_items[] = {
  "Exit & save",
  "Filter",
  "Rotation",
  "Wb",
  "Restore",  "Restore"

#ifdef USE_OTA
  "OTA"
#endif
};

const byte menu_items_n = sizeof(menu_items) / sizeof(menu_items[0]);
byte menuCursor = 0, menuCursorLast = 0, configMode = 0;

#define MenuX xw - 40
#define MenuY yh - 50


void setupMenu() {
  esp_log_level_set("menu", ESP_LOG_WARN);
  esp_log_level_set("config", ESP_LOG_WARN);

  xw = tft.width() / 2;  // xw, yh is middle of screen
  yh = tft.height() / 2;

  // Button lib init
  buttonConfig.setOnClickListener(btnConfigClick);
  buttonConfig.setOnMultiClickListener(btnConfigMulti);
  buttonConfig.setOnLongClickListener(btnConfigLong);
  buttonConfig.setSingleClickTime(500);
  buttonConfig.setMultipleClickTime(400 * 2);
  buttonConfig.setLongClickTime(1000);
  buttonConfig.setDebounceTime(50);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  prefs.begin("esp32-dnvg");

  if (not prefs.isKey("ver")) {  // If config does not exist create one form default
    restoreConfig();
    ESP_LOGI("config", "does not exist, creating");
  } else loadConfig();  // Else load config

  camSettingsChangedFlag = true;

#ifdef USE_CLI
  prefCli.registerCommands(cli);
#endif
}

void loopMenu() {
  buttonConfig.update();
#ifdef USE_CLI
  //Check if the user has sent text in via serial, then have the CLI parse it as a command
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    Serial.println(line);
    cli.parse(line);
  }

  //If the user input was successfulyl parsed by the CLI as a known command then it will be available in the CLI's queue
  if (cli.available()) {
    bool handled = false;
    //'pop' the first command from the CLI parsed command queue
    Command command = cli.getCommand();
    //check to see if the command is related to the preferences library and handle it if it is.
    //The command might not be for the preferences library, so you may need to go 'ask' other parts of your code if they can handle this command.
    handled &= prefCli.handleCommand(command, Serial);
    Serial.print("\n> ");
  }

  //if the input passed into the CLI parser is not a valid command then it can be popped off the error queue
  if (cli.errored()) {
    CommandError e = cli.getError();
    handleCliError(e, Serial);
  }
#endif
}

void loadConfig() {
  prefs.getBytes("rotation", &config.rotation, 1);
  prefs.getBytes("filter", &config.filter, 1);
  prefs.getBytes("wb", &config.wb, 1);
  ESP_LOGI("config", "Config loaded");
}

void storeConfig() {
  prefs.putBytes("ver", &config.ver, 1);
  prefs.putBytes("rotation", &config.rotation, 1);
  prefs.putBytes("filter", &config.filter, 1);
  prefs.putBytes("wb", &config.wb, 1);
  ESP_LOGI("config", "Config stored");
}

void restoreConfig() {
  prefs.putBytes("rotation", &defConfig.rotation, 1);
  prefs.putBytes("filter", &defConfig.filter, 1);
  prefs.putBytes("wb", &defConfig.wb, 1);
  ESP_LOGI("config", "Config restored");
}

void drawMenu() {
  spr.drawString(String(frameTime), 100, 220, 2);  //print frame time in milliseconds
  spr.drawString("ms", 125, 220, 2);

  spr.drawString(String(FPS), 100, 200, 2);  //print frame time in milliseconds
  spr.drawString("fps", 130, 200, 2);

  //spr.pushToSprite(&menuSpr, xw - 40, yh - 50);

  //menuSpr.fillScreen(TFT_TRANSPARENT);

  spr.fillRect(MenuX, MenuY, 80, 20 * menu_items_n, TFT_BLUE);
  spr.fillRect(MenuX, MenuY + (20 * menuCursor), 10, 20, TFT_BLACK);

  for (byte i = 0; i < menu_items_n; i++) {
    spr.drawString(menu_items[i], 100, (i * 20) + MenuY, 2);
  }
}

void menuEnter(byte cursor) {
  switch (cursor) {
    case 0:  // Exit
      menuShown = false;
      storeConfig();  // Store config
      ESP_LOGI("menu", "exit");
      break;

    case 1:  // Filter
      if (config.filter == 6) config.filter = 0;
      else config.filter++;
      camSettingsChangedFlag = true;
      ESP_LOGI("menu", "filter");

      break;

    case 2:  // Rot
      if (config.rotation == 3) config.rotation = 0;
      else config.rotation++;
      tft.setRotation(config.rotation);
      ESP_LOGI("menu", "rotation");
      break;

    case 3:  // Wb
      if (config.wb == 4) config.wb = 0;
      else config.wb++;
      camSettingsChangedFlag = true;
      ESP_LOGI("menu", "white balance");
      break;

    case 4:
      restoreConfig();
      ESP_LOGI("menu", "Restore");
      camSettingsChangedFlag = true;
      break;

#ifdef EN_OTA
    case 5:  // OTA
      ESP_LOGI("menu", "OTA");
      setupOTA();
      break;
#endif
  }
}

void btnConfigClick() {
  if (menuShown) {
    if (menuCursor == menu_items_n - 1) menuCursor = 0;
    else menuCursor++;
  } else menuShown = true;
}

void btnConfigMulti(int n) {  //Enter
  ESP_LOGD("menu", "Multi click: %d", n);
  if (n == 2 || n == 3) menuEnter(menuCursor);
}

void btnConfigLong(int t) {
  ESP_LOGD("menu", "Long click: %d", t);
  menuShown = false;
  storeConfig();  // Store config
}

#ifdef USE_CLI
//this function handles displaying an error message over serial if the text sent does not parse as a known, valid command
void handleCliError(CommandError &e, Stream &outputStream) {
  CommandError cmdError(e);
  outputStream.print("ERROR: ");
  outputStream.println(cmdError.toString());

  //This is the case where the CLI was smart enough to know what command was requested, but the parameters sent with the command don't make sense for that command
  if (cmdError.hasCommand()) {
    outputStream.println(cmdError.getCommand().toString());
  }
  outputStream.print("\n> ");
}
#endif

#endif