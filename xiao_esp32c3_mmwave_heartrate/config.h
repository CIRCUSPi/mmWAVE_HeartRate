#ifndef _CONFIG_H
#define _CONFIG_H

// #define DEBUG

const char*   ssid = "WIFI_SSID";                 // WIFI 連線名稱，可不填，連不上可以離線顯示
const char*   password = "WIFI_PASS";             // WIFI 連線密碼，可不填，連不上可以離線顯示
const char*   mqtt_server = "broker.emqx.io";     // MQTT Broker 名稱，預設使用 EMQX 開放 Broker
const char*   mqtt_user = "username_if_needed";   // MQTT 使用者名稱，依需求填入，EMQX 可不填
const char*   mqtt_key = "key_if_needed";         // MQTT 使用者名稱，依需求填入，EMQX 可不填
const char*   topic_hr = "CircusPi/HeartRate";    // 上傳心跳率的 Topic 名稱
const char*   topic_br = "CircusPi/BreathRate";   // 上傳呼吸率的 Topic 名稱

#define PIN_BUTTON          1

#define WIFI_RETRY_TIME     5

#define INTERVAL_MMWAVE     150
#define INTERVAL_OLED       500
#define INTERVAL_WRITESD    1000
#define INTERVAL_MQTT       15000


#define ICON_BIG_WIDTH      32
#define ICON_BIG_HEIGHT     32
#define ICON_SMALL_WIDTH    22
#define ICON_SMALL_HEIGHT   22

// OLED xy coordinate
#define PLANE_ORIGIN_X      0
#define PLANE_ORIGIN_Y      0
#define PLANE_B_ICON_UP_X   PLANE_ORIGIN_X
#define PLANE_B_ICON_UP_Y   PLANE_ORIGIN_Y
#define PLANE_B_ICON_DN_X   PLANE_ORIGIN_X
#define PLANE_B_ICON_DN_Y   (PLANE_B_ICON_UP_Y + ICON_BIG_HEIGHT)
#define PLANE_DATA_UP_X     40
#define PLANE_DATA_UP_Y     28
#define PLANE_DATA_DN_X     PLANE_DATA_UP_X
#define PLANE_DATA_DN_Y     60
#define PLANE_S_ICON_UP_X   104
#define PLANE_S_ICON_UP_Y   PLANE_ORIGIN_Y 
#define PLANE_S_ICON_MD_X   PLANE_S_ICON_UP_X
#define PLANE_S_ICON_MD_Y   (PLANE_S_ICON_UP_Y + ICON_SMALL_HEIGHT - 2)
#define PLANE_S_ICON_DN_X   PLANE_S_ICON_UP_X
#define PLANE_S_ICON_DN_Y   (PLANE_S_ICON_MD_Y + ICON_SMALL_HEIGHT - 1)

//XBM arrays for OLED disaply
const unsigned char heartbeat_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x0F, 0xF0, 0x07, 
  0xF0, 0x3F, 0xFC, 0x0F, 0xFC, 0x7F, 0xFE, 0x3F, 0xFC, 0xFF, 0xFF, 0x3F, 
  0xFE, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xDF, 0xFF, 0xFF, 0x9F, 0xDF, 0xFF, 0xFF, 0xDF, 0xDF, 0xFF, 
  0xFF, 0x9F, 0xDF, 0xFF, 0x1F, 0xB0, 0x41, 0xF0, 0xFE, 0xAF, 0xA6, 0x7F, 
  0xFC, 0x67, 0xAF, 0x3F, 0xFC, 0x67, 0xBF, 0x3F, 0xF8, 0x67, 0xFF, 0x1F, 
  0xF0, 0xFF, 0xFF, 0x0F, 0xE0, 0xFF, 0xFF, 0x07, 0xC0, 0xFF, 0xFF, 0x03, 
  0x80, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFE, 0x7F, 0x00, 
  0x00, 0xFC, 0x3F, 0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0xF0, 0x0F, 0x00, 
  0x00, 0xE0, 0x07, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x80, 0x01, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

const unsigned char breath_bits[] = {
  0x00, 0x7E, 0x00, 0x00, 0x80, 0xE7, 0x1F, 0x00, 0xC0, 0x00, 0x36, 0x00, 
  0x60, 0x00, 0x60, 0x00, 0x30, 0x00, 0xC0, 0x00, 0x18, 0x00, 0x40, 0x00, 
  0x08, 0x00, 0x60, 0x00, 0x0C, 0x00, 0x3E, 0x00, 0x0C, 0x00, 0x1B, 0x00, 
  0x08, 0x00, 0x11, 0x00, 0x0C, 0x9C, 0x31, 0x00, 0x08, 0xF6, 0x30, 0x00, 
  0x08, 0x66, 0x20, 0x00, 0x18, 0x46, 0x60, 0x00, 0x18, 0x06, 0xC0, 0x18, 
  0x10, 0x04, 0x60, 0x3C, 0x30, 0x0C, 0x30, 0x66, 0xE0, 0x06, 0xB0, 0xC3, 
  0xE0, 0x07, 0x98, 0xC1, 0x40, 0x00, 0x30, 0xC3, 0x60, 0x00, 0x30, 0x66, 
  0x20, 0x30, 0x10, 0x3C, 0x60, 0xE0, 0x1F, 0x08, 0xF0, 0x63, 0x0A, 0x00, 
  0x18, 0x3F, 0x00, 0x00, 0x0C, 0x70, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 
  0x02, 0xC0, 0x00, 0x00, 0x02, 0x80, 0x01, 0x00, 0x03, 0x00, 0x03, 0x00, 
  0x07, 0x80, 0x07, 0x00, 0x7F, 0xFB, 0x07, 0x00, };

const unsigned char wifi_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0xE0, 0xFF, 0x00, 
  0xF8, 0xFF, 0x07, 0xFC, 0xF5, 0x0F, 0x3E, 0x00, 0x1F, 0x8F, 0x7F, 0x3C, 
  0xE3, 0xFF, 0x19, 0xF0, 0xFF, 0x03, 0xF8, 0xE0, 0x07, 0x78, 0x0A, 0x07, 
  0x00, 0x7F, 0x00, 0xC0, 0x7F, 0x00, 0xC0, 0xFB, 0x00, 0xC0, 0xC0, 0x00, 
  0x00, 0x0C, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
  
const unsigned char update_bits[] = {
  0x00, 0x1E, 0x00, 0xC0, 0xFF, 0x00, 0xF0, 0xFF, 0x3B, 0xF8, 0xFF, 0x3F, 
  0xFC, 0xC0, 0x3F, 0x3C, 0x80, 0x3F, 0x1E, 0x80, 0x3F, 0x1E, 0xC0, 0x3F, 
  0x0F, 0xC0, 0x3F, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x20, 0x00, 0x18, 0xFF, 0x00, 0x1C, 0xFF, 0x00, 0x1E, 0x7F, 0x00, 0x1F, 
  0x7F, 0x00, 0x0F, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0x07, 0xF7, 0xFF, 0x03, 
  0xC0, 0xFF, 0x00, 0x00, 0x3E, 0x00, };

const unsigned char microsd_bits[] = {
  0x00, 0x00, 0x00, 0xC0, 0xFF, 0x07, 0xE0, 0xFF, 0x0F, 0x60, 0x44, 0x0C, 
  0x60, 0x44, 0x0C, 0x60, 0x44, 0x0C, 0x60, 0x44, 0x0C, 0xE0, 0xFF, 0x0F, 
  0xE0, 0xFF, 0x0F, 0xF0, 0xFF, 0x0F, 0xF8, 0xFF, 0x0F, 0xFC, 0xFF, 0x0F, 
  0xFC, 0xFF, 0x0F, 0xFC, 0xFF, 0x0F, 0xFC, 0xFF, 0x0F, 0xFC, 0xFF, 0x0F, 
  0xFC, 0xFF, 0x0F, 0xFC, 0xFF, 0x0F, 0xFC, 0xFF, 0x0F, 0xFC, 0xFF, 0x0F, 
  0xFC, 0xFF, 0x07, 0x00, 0x00, 0x00, };


#endif  //_CONFIG_H