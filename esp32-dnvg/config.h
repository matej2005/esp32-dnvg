// Some settings


#define UART_BAUD 115200

/*    Camera configuration  */
#define CAMERA_VERSION OV2640  // Not working right now, but planned
#define CAMERA_MODEL_AI_THINKER
#define CAM_XCLK_FREQ_HZ 20000000
#define CAM_FRAMESIZE FRAMESIZE_240X240
#define CAM_PIXFORMAT PIXFORMAT_RGB565
#define CAM_GRAP_MODE CAMERA_GRAB_LATEST  //option CAMERA_GRAB_WHEN_EMPTY
#define CAM_FB_LOCATION CAMERA_FB_IN_PSRAM
#define CAM_JMEG_QUALITY 12
#define CAM_FB_COUNT 2  //need more than 1 for latest grab

// Camera pins
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

/*    Default setings   */
Config_t defConfig = {
  .rotation = 2,  // Rotation of display 0, 1, 2, 3
  .filter = 0,    // Filter   0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  .wbV = 0        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
};

/*    Menu stuff    */
//#define MENU_TIMEOUT 10000
#define BUTTON_PIN 15  // Pin where is connected the button


/*    TESTING       */
#define USE_MENU
//#define USE_OTA      //Does not work properly
#define USE_CLI

/*    OTA           */
//#define SSID "ESP32-NODS"
//#define PASSWORD "test123456
//#define HTTP_PORT 80
//#define OTA_PASSWORD "admin"







/*    Macros      */
enum camera_type {
  OV2640,
  OV7725
};