// 引入必要標頭檔案
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <60ghzbreathheart.h>
#include <U8g2lib.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>
#include "config.h"

// 宣告 mmWAVE R60A instance
BreathHeart_60GHz radar = BreathHeart_60GHz(&Serial1);
// 宣告 XIAO Expansion board OLED instance
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
// 宣告 XIAO Expansion board PCF8564 instance
RTC_PCF8563 rtc;
// 宣告 WiFi Client instance
WiFiClient espClient;
// 宣告 MQTT Client instance
PubSubClient client(espClient);

// 變數宣告
File            myFile;           // 檔案描述，用來操作讀寫檔案內容
unsigned long   time_oled;        // 任務時間戳記 for OLED
unsigned long   time_mmwave;      // 任務時間戳記 for mmWAVE
unsigned long   time_mqtt;        // 任務時間戳記 for MQTT upload
unsigned long   time_sd;          // 任務時間戳記 for SD 卡
int             sd_ok;            // sd card 初始化旗標
int             wifi_ok;          // wifi 初始化旗標

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  for (int ii=0; ii<WIFI_RETRY_TIME; ii++) {
    Serial.print('.');
    if (WiFi.status() == WL_CONNECTED) {
      wifi_ok = 1;
      break;
    }
    delay(1000);
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_key)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("outTopic", "hello world");
      // ... and resubscribe
      // client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // 處理訂閱資料
}

void setup() {
  int ret_rtc, ret_sht30, ret_sgp30, ret_sd;
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Serial1.begin(115200, SERIAL_8N1, 44, 43); // for XIAO ESP32S3
  Serial1.begin(115200, SERIAL_8N1, 20, 21);    // for XIAO ESP32C3

  pinMode(PIN_BUTTON, INPUT_PULLUP);
  
  // setup sd card
  pinMode(D2, OUTPUT);
  sd_ok = SD.begin(D2);

  u8g2.begin();
  u8g2.enableUTF8Print();
  
  u8g2.setFont(u8g2_font_courB12_tr);
  u8g2.setFontDirection(0);
  u8g2.clearDisplay();

  u8g2.firstPage();  
  do {
    u8g2.setCursor(0, 16);
    u8g2.print("initializing...");
    u8g2.sendBuffer();
  } while(u8g2.nextPage());
  u8g2.setFont(u8g2_font_helvB24_tr);

  // setup rtc
  ret_rtc = rtc.begin();
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.start();

  setup_wifi();
  if (wifi_ok) {
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
  }

  // radar.ModeSelect_fuc(1);  //1: indicates real-time transmission mode, 2: indicates sleep state mode.
  //After setting the mode, if you do not see data returned, you may need to re-power the sensor.
  
}

void loop()
{
  char str[64];
  static int hr=0, br=0, update_icon=0;
  
  // 讀取 mmWAVE 資料
  if ((millis() - time_mmwave) > INTERVAL_MMWAVE) {
    time_mmwave = millis();
    radar.Breath_Heart();           //Breath and heartbeat information output
    if (radar.sensor_report != 0x00) {
      switch (radar.sensor_report) {
        case HEARTRATEVAL:
          Serial.print("Sensor monitored the current heart rate value is: ");
          Serial.println(radar.heart_rate, DEC);
          hr = radar.heart_rate;
          update_icon = 1;
          Serial.println("----------------------------");
          break;
        case HEARTRATEWAVE:  //Valid only when real-time data transfer mode is on
          #ifdef DEBUG
          Serial.print("The heart rate waveform(Sine wave)-- point 1: ");
          Serial.print("-- point 1: ");
          Serial.print(radar.heart_point_1);
          Serial.print(", point 2 : ");
          Serial.print(radar.heart_point_2);
          Serial.print(", point 3 : ");
          Serial.print(radar.heart_point_3);
          Serial.print(", point 4 : ");
          Serial.print(radar.heart_point_4);
          Serial.print(", point 5 : ");
          Serial.println(radar.heart_point_5);
          Serial.println("----------------------------");
          #endif
          break;
        case BREATHNOR:
          Serial.println("Sensor detects current breath rate is normal.");
          Serial.println("----------------------------");
          update_icon = 1;
          break;
        case BREATHRAPID:
          Serial.println("Sensor detects current breath rate is too fast.");
          Serial.println("----------------------------");
          update_icon = 1;
          break;
        case BREATHSLOW:
          Serial.println("Sensor detects current breath rate is too slow.");
          Serial.println("----------------------------");
          update_icon = 1;
          break;
        case BREATHNONE:
          Serial.println("There is no breathing information yet, please wait...");
          Serial.println("----------------------------");
          break;
        case BREATHVAL:
          Serial.print("Sensor monitored the current breath rate value is: ");
          Serial.println(radar.breath_rate, DEC);
          br = radar.breath_rate;
          update_icon = 1;
          Serial.println("----------------------------");
          break;
        case BREATHWAVE:  //Valid only when real-time data transfer mode is on
          #ifdef DEBUG
          Serial.print("The breath rate waveform(Sine wave) -- point 1: ");
          Serial.print(radar.breath_point_1);
          Serial.print(", point 2 : ");
          Serial.print(radar.breath_point_2);
          Serial.print(", point 3 : ");
          Serial.print(radar.breath_point_3);
          Serial.print(", point 4 : ");
          Serial.print(radar.breath_point_4);
          Serial.print(", point 5 : ");
          Serial.println(radar.breath_point_5);
          Serial.println("----------------------------");
          #endif
          break;
      }
    }
  }

  // 更新 OLED
  if ((millis() - time_oled) > INTERVAL_OLED) {
    time_oled = millis();
    u8g2.clearBuffer();
    u8g2.firstPage();  
    do {
      u8g2.drawXBM(PLANE_B_ICON_UP_X, PLANE_B_ICON_UP_Y, ICON_BIG_WIDTH, ICON_BIG_HEIGHT, heartbeat_bits);
      u8g2.drawXBM(PLANE_B_ICON_DN_X, PLANE_B_ICON_DN_Y, ICON_BIG_WIDTH, ICON_BIG_HEIGHT, breath_bits);
      
      if (wifi_ok) {
        u8g2.drawXBM(PLANE_S_ICON_UP_X, PLANE_S_ICON_UP_Y, ICON_SMALL_WIDTH, ICON_SMALL_HEIGHT, wifi_bits);
      }
      if (sd_ok) {
        u8g2.drawXBM(PLANE_S_ICON_MD_X, PLANE_S_ICON_MD_Y, ICON_SMALL_WIDTH, ICON_SMALL_HEIGHT, microsd_bits);
      }
      if (update_icon) {
        u8g2.drawXBM(PLANE_S_ICON_DN_X, PLANE_S_ICON_DN_Y, ICON_SMALL_WIDTH, ICON_SMALL_HEIGHT, update_bits);
      }
      
      u8g2.setCursor(PLANE_DATA_UP_X, PLANE_DATA_UP_Y);
      u8g2.print(hr);
      u8g2.setCursor(PLANE_DATA_DN_X, PLANE_DATA_DN_Y);
      u8g2.print(br);
      u8g2.sendBuffer();
    } while(u8g2.nextPage());
    update_icon = 0;
  }

  // 如果 WIFI 連線，檢查 WIFI 狀態 並且傳送資料到 MQTT Broker
  if (wifi_ok) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.reconnect();
    }
    if (!client.connected()) {
      mqtt_reconnect();
    }
    client.loop();

    if ((millis() - time_mqtt) > INTERVAL_MQTT) {
      time_mqtt = millis();
      sprintf(str, "%d", hr);
      client.publish(topic_hr, str);
      sprintf(str, "%d", br);
      client.publish(topic_br, str);
    }
  }

  // 如果 SD 卡存在，把資料記錄到 SD 卡
  if (sd_ok) {
    if ((millis() - time_sd) > INTERVAL_WRITESD) {
      time_sd = millis();

      DateTime now = rtc.now();
      sprintf(str, "/%04d-%02d-%02d.csv", now.year(), now.month(), now.day());
      myFile = SD.open(str, FILE_WRITE);
      if (myFile) {
        myFile.seek(myFile.size()); // move to the end of file
        
        sprintf(str, "%02d:%02d:%02d,%d,%d", now.hour(), now.minute(), now.second(), hr, br);
        myFile.println(str);
        // close the file:
        myFile.close();
  #if DEBUG
        Serial.print("Writing to log file...");
        Serial.println(str);
  #endif
      } else {
        // if the file didn't open, print an error:
        Serial.println("error opening log file");
      }
    }  
  }
  
}
