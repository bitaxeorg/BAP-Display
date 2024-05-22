
#include <Wire.h>

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <esp_task_wdt.h>
#include <OneButton.h>

#include "mbedtls/md.h"
#include "wManager.h"
#include "mining.h"
#include "monitor.h"
#include "drivers/displays/display.h"

#include "timeconst.h"
#include "drivers/devices/BAP.h"
#include <SPIFFS.h>

//3 seconds WDT
#define WDT_TIMEOUT 3
//15 minutes WDT for miner task
#define WDT_MINER_TIMEOUT 900

#ifdef PIN_BUTTON_1
  OneButton button1(PIN_BUTTON_1);
#endif

#ifdef PIN_BUTTON_2
  OneButton button2(PIN_BUTTON_2);
#endif

extern monitor_data mMonitor;


/**********************⚡ GLOBAL Vars *******************************/

unsigned long start = millis();
const char* ntpServer = "pool.ntp.org";

//void runMonitor(void *name);

/********* INIT *****/
void setup()
{
      //Init pin 15 to eneble 5V external power (LilyGo bug)
  #ifdef PIN_ENABLE5V
      pinMode(PIN_ENABLE5V, OUTPUT);
      digitalWrite(PIN_ENABLE5V, HIGH);
  #endif

#ifdef MONITOR_SPEED
    Serial.begin(MONITOR_SPEED);
#else
    Serial.begin(115200);
#endif //MONITOR_SPEED

  Serial.setTimeout(0);
  delay(SECOND_MS/10);

  esp_task_wdt_init(WDT_MINER_TIMEOUT, true);
  // Idle task that would reset WDT never runs, because core 0 gets fully utilized
  disableCore0WDT();
  //disableCore1WDT();

  /******** INIT NERDMINER ************/
  Serial.println("NerdMiner v2 starting......");

  SPIFFS.begin(true);

  /******** INIT DISPLAY ************/
  initDisplay();
  Serial.println("Setup......");
  drawSetupScreen();
  //drawLoadingScreen();
  TaskHandle_t displayTask = NULL;
  xTaskCreate(BAP_test, "BAP", 6000, (void*)1, 1, &displayTask);
  
  /******** PRINT INIT SCREEN *****/
  // drawLoadingScreen();
  // 

  /******** SHOW LED INIT STATUS (devices without screen) *****/
  //mMonitor.NerdStatus = NM_waitingConfig;


  /******** CREATE TASK TO PRINT SCREEN *****/
  //tft.pushImage(0, 0, MinerWidth, MinerHeight, MinerScreen);
  // Higher prio monitor task
  // Serial.println("");
  // Serial.println("Initiating tasks...");
  // char *name = (char*) malloc(32);
  // sprintf(name, "(%s)", "Monitor");
  // BaseType_t res1 = xTaskCreatePinnedToCore(runMonitor, "Monitor", 10000, (void*)name, 4, NULL,1);

  // /******** CREATE STRATUM TASK *****/
  // sprintf(name, "(%s)", "Stratum");
  // BaseType_t res2 = xTaskCreatePinnedToCore(runStratumWorker, "Stratum", 15000, (void*)name, 3, NULL,1);


  /******** CREATE MINER TASKS *****/
  //for (size_t i = 0; i < THREADS; i++) {
  //  char *name = (char*) malloc(32);
  //  sprintf(name, "(%d)", i);

  // Start mining tasks
  //BaseType_t res = xTaskCreate(runWorker, name, 35000, (void*)name, 1, NULL);
  // TaskHandle_t minerTask1, minerTask2 = NULL;
  // xTaskCreate(runMiner, "Miner0", 6000, (void*)0, 1, &minerTask1);
  // xTaskCreate(runMiner, "Miner1", 6000, (void*)1, 1, &minerTask2);
 
  // esp_task_wdt_add(minerTask1);
  // esp_task_wdt_add(minerTask2);

  /******** MONITOR SETUP *****/
//  setup_monitor();
}

void app_error_fault_handler(void *arg) {
  // Get stack errors
  char *stack = (char *)arg;

  // Print the stack errors in the console
  esp_log_write(ESP_LOG_ERROR, "APP_ERROR", "Pila de errores:\n%s", stack);

  // restart ESP32
  esp_restart();
}

void loop() {
  // keep watching the push buttons:
  #ifdef PIN_BUTTON_1
    button1.tick();
  #endif

  #ifdef PIN_BUTTON_2
    button2.tick();
  #endif
  
  wifiManagerProcess(); // avoid delays() in loop when non-blocking and other long running code

  vTaskDelay(50 / portTICK_PERIOD_MS);
}
