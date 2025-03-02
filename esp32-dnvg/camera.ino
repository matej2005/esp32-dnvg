#include "esp_camera.h"


TaskHandle_t Task1Cam_handle;  // Create handlerer for camera stream grabber
camera_config_t configCam;

void Task1code(void *pvParameters);

void setupCamera() {
  esp_log_level_set("camera", ESP_LOG_WARN);

  xTaskCreatePinnedToCore(
    Task1code,  // Task function.
    "Task1",    // name of task.
    100000,     // Stack size of task
    NULL,       // parameter of the task
    1,
    &Task1Cam_handle,  // Task handle to keep track of created task
    0);                // pin task to core 0
}

void Task1code(void *pvParameters) {
  //core0 setup
  configCam.ledc_channel = LEDC_CHANNEL_0;
  configCam.ledc_timer = LEDC_TIMER_0;
  configCam.pin_d0 = Y2_GPIO_NUM;
  configCam.pin_d1 = Y3_GPIO_NUM;
  configCam.pin_d2 = Y4_GPIO_NUM;
  configCam.pin_d3 = Y5_GPIO_NUM;
  configCam.pin_d4 = Y6_GPIO_NUM;
  configCam.pin_d5 = Y7_GPIO_NUM;
  configCam.pin_d6 = Y8_GPIO_NUM;
  configCam.pin_d7 = Y9_GPIO_NUM;
  configCam.pin_xclk = XCLK_GPIO_NUM;
  configCam.pin_pclk = PCLK_GPIO_NUM;
  configCam.pin_vsync = VSYNC_GPIO_NUM;
  configCam.pin_href = HREF_GPIO_NUM;
  configCam.pin_sscb_sda = SIOD_GPIO_NUM;
  configCam.pin_sscb_scl = SIOC_GPIO_NUM;
  configCam.pin_pwdn = PWDN_GPIO_NUM;
  configCam.pin_reset = RESET_GPIO_NUM;
  configCam.xclk_freq_hz = CAM_XCLK_FREQ_HZ;
  configCam.frame_size = CAM_FRAMESIZE;
  configCam.pixel_format = CAM_PIXFORMAT;
  configCam.grab_mode = CAMERA_GRAB_LATEST;
  configCam.fb_location = CAM_FB_LOCATION;
  configCam.jpeg_quality = CAM_JMEG_QUALITY;
  configCam.fb_count = CAM_FB_COUNT;

  esp_err_t err = esp_camera_init(&configCam);
  sensor_t *s = esp_camera_sensor_get();
  s->set_brightness(s, 0);                  // -2 to 2
  s->set_contrast(s, 0);                    // -2 to 2
  s->set_saturation(s, 0);                  // -2 to 2
  s->set_special_effect(s, 0);              // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_whitebal(s, 1);                    // 0 = disable , 1 = enable
  s->set_awb_gain(s, 1);                    // 0 = disable , 1 = enable
  s->set_wb_mode(s, 0);                     // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_exposure_ctrl(s, 1);               // 0 = disable , 1 = enable
  s->set_aec2(s, 0);                        // 0 = disable , 1 = enable
  s->set_ae_level(s, 0);                    // -2 to 2
  s->set_aec_value(s, 300);                 // 0 to 1200
  s->set_gain_ctrl(s, 1);                   // 0 = disable , 1 = enable
  s->set_agc_gain(s, 0);                    // 0 to 30
  s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
  s->set_bpc(s, 0);                         // 0 = disable , 1 = enable
  s->set_wpc(s, 1);                         // 0 = disable , 1 = enable
  s->set_raw_gma(s, 1);                     // 0 = disable , 1 = enable
  s->set_lenc(s, 1);                        // 0 = disable , 1 = enable
  s->set_hmirror(s, 0);                     // 0 = disable , 1 = enable
  s->set_vflip(s, 0);                       // 0 = disable , 1 = enable
  s->set_dcw(s, 1);                         // 0 = disable , 1 = enable
  s->set_colorbar(s, 0);                    // 0 = disable , 1 = enable

  //core0 loop
  for (;;) {
    if (cameraEnable) {  //Check if ota is enabled and dont continue to save cpu
      if (camSettingsChangedFlag) {
        ESP_LOGI("camera", "config updated: ");
        ESP_LOGI("camera", "Config.filter: %d", config.filter);
        ESP_LOGI("camera", "Config.wb: %d", config.wb);

        s->set_special_effect(s, (int)config.filter);
        //s->set_whitebal(s, config.wb);        // 0 = disable , 1 = enable
        s->set_wb_mode(s, config.wb);  //  0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
        camSettingsChangedFlag = false;
      }
      //take picture
      camera_fb_t *fb = NULL;
      fb = esp_camera_fb_get();
      //transfer frame buffer data to pointer

      for (size_t i = 0; i < 57600; i++) {  //240x240px = 57600
        byte first_byte = fb->buf[i * 2];
        byte second_byte = fb->buf[i * 2 + 1];
        scr[57600 - i] = (second_byte << 8) + first_byte;
      }

      screenRefreshFlag = true;
      esp_camera_fb_return(fb);  //return the frame buffer back to the driver for reuse
    }
  }
}