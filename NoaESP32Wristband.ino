/******************************************

        NN   NN    OOOOOO    AAA
       NN N NN    OO  OO    AAAA
      NN   NN    OOOOOO    AA AA

  Proudly written in glorious Europe
  
******************************************/

// --- Pre-processor --- //
#define USE_ARDUINO_INTERRUPTS false

// --- Libraries --- //
#include <memory>
#include <TaskScheduler.h>
#include <Preferences.h>
#include "StateManager.h"
#include "LoRaSender.h"
#include "GPSSensor.h"
#include "HeartBeatSensor.h"
#include "ButtonSensor.h"
#include "BuzzerSensor.h"
#include "LedSensor.h"
#include "OledDisplay.h"
#include "RFIDReader.h"
#include "Message.h"
#include "Position.h"
#include "SOSWave.h"
#include "Accelerometer.h"
#include "MicSensor.h"

// --- Constants --- //
#define APP_KEY             ("NOA_WRISTBAND")
#define LORA_BAND           (868E6) // 433E6 - Asia    868E6 - Europe     915E6 - NORTH AMERICA

#define RFID_SCK_PIN        (18) 
#define RFID_MISO_PIN       (19)
#define RFID_MOSI_PIN       (23)
#define RFID_SDA_PIN        (5) 
#define RFID_RST_PIN        (22)

#define LORA_SCK_PIN        (5)
#define LORA_MISO_PIN       (19)
#define LORA_MOSI_PIN       (27)
#define LORA_SS_PIN         (18)
#define LORA_RST_PIN        (14)
#define LORA_DIO0_PIN       (26)

#define AXIS_SDA_PIN        (4)
#define AXIS_SCL_PIN        (15)

#define GPS_RX_PIN          (34) // 16
#define GPS_TX_PIN          (38) // 17

#define HEART_BEAT_PIN      (38) // ADC1_CH6

#define OLED_SDA_PIN        (4)
#define OLED_SCL_PIN        (15) // 
#define OLED_RST_PIN        (16) // 
#define SCREEN_WIDTH        (128) // in pixels
#define SCREEN_HEIGHT       (64) // in pixels

#define BUTTON_PIN          (17)
#define BUZZER_PIN          (21)
#define LED_PIN             (13)
#define MIC_PIN             (36)

// --- Global Variables --- //
Scheduler runner;
Preferences store;

auto wave = SOSWave();
auto stateManager = StateManager(store);

auto loraSender = std::make_shared<LoRaSender>(stateManager, LORA_BAND, LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
auto buzzerSensor = std::make_shared<BuzzerSensor>(BUZZER_PIN, wave);
auto ledSensor = std::make_shared<LedSensor>(LED_PIN);
auto display = std::make_shared<OledDisplay>(stateManager, SCREEN_WIDTH, SCREEN_HEIGHT, OLED_RST, 1, OLED_SDA_PIN, OLED_SCL_PIN);

auto rfidReader = RFIDReader(stateManager, RFID_SCK_PIN, RFID_MISO_PIN, RFID_MOSI_PIN, RFID_SDA_PIN, RFID_RST_PIN);
auto gpsSensor = GPSSensor(stateManager, GPS_RX_PIN, GPS_TX_PIN);
auto accelerometer = Accelerometer(AXIS_SDA_PIN, AXIS_SCL_PIN);
auto heartBeatSensor = HeartBeatSensor(HEART_BEAT_PIN);
auto buttonSensor = ButtonSensor(BUTTON_PIN);
auto micSensor = MicSensor(MIC_PIN);

// --- Tasks --- //
Task lora_check_task(5000, TASK_FOREVER, [] { loraSender->update(); });
Task gps_check_task(5000, TASK_FOREVER, [] { /* gpsSensor.checkGPS(); */ });
Task rfid_check_task(1000, TASK_FOREVER, [] { rfidReader.checkRFID(); });
Task btn_check_task(20, TASK_FOREVER, [] { buttonSensor.checkButton(); });
Task mic_check_task(20, TASK_FOREVER, [] { micSensor.checkMic(); });
Task buzzer_check_task(20, TASK_FOREVER, [] { buzzerSensor->update(); });
Task led_check_task(20, TASK_FOREVER, [] { ledSensor->update(); });
Task oled_refresh_task(50, TASK_FOREVER, [] { display->update(); });
Task axis_refresh_task(100, TASK_FOREVER, [] { accelerometer.update(); });
Task heart_refresh_task(100, TASK_FOREVER, [] { heartBeatSensor.update(); });

// --- Functions --- //
void die(byte code)
{
    display->clear();
    display->set_line(0);

    display->push_line("General failure:");
    switch(code)
    {
        case 0x01: // LoRa failed
            display->push_line("LoRa module init (0x01)");
            Serial.println("Starting LoRa failed!");
            break;
        case 0x02:
            display->push_line("GPS module init (0x02)");
            Serial.println("GPS module init (0x02)");
            break;
        case 0x03:
            display->push_line("Heartbeat sensor init (0x03)");
            Serial.println("Heartbeat sensor init (0x03)");
            break;
        case 0x04:
            display->push_line("SSD1306 allocation failed (0x04)");
            Serial.println("SSD1306 allocation failed (0x04)");
            break;
        case 0x05:
            display->push_line("RFID module init (0x05)");
            Serial.println("RFID module init (0x05)");
            break;
        case 0x06:
            display->push_line("Accelerometer init (0x06)");
            Serial.println("Accelerometer init (0x06)");
            break;
    }
    
    for(;;);
}

// --- Main Code ---//
void setup() 
{
    // Initialize Serial Monitor
    Serial.begin(9600);
    SPI.begin();
    store.begin(APP_KEY, false);
    stateManager.loadData();
            
    if(!display->begin(OLED_RST)) 
        die(0x04);
        
    display->set_line(0);
    display->push_line("Initializing...");
    // display->setLoading(true);

    if(!loraSender->begin()) 
        die(0x01);

    if(!rfidReader.begin())
        die(0x05);

    if(!gpsSensor.begin())
        die(0x02);

    /*if(!heartBeatSensor.begin())
        die(0x03); */

    if(!accelerometer.begin())
        die(0X05); 
    
    delay(2000); // Fake loading time for the sake of being absolutes code masters

    display->set_line(0);
    display->push_line("Initialization OK");

    display->set_line(1);
    display->push_line(stateManager.getUserId());

    stateManager.addToggleSensor(buzzerSensor);
    stateManager.addToggleSensor(ledSensor);
    stateManager.addToggleSensor(loraSender);
    stateManager.addToggleSensor(display);

    micSensor.setOnSound([]()
    {
        Serial.println("Toggle sound");
        stateManager.enableSOSMode(true);
    });

    buttonSensor.setOnPressedListener([](bool pressed) 
    { 
        stateManager.toggleSOSMode(); 
    });

    accelerometer.setOnShakedListener([]()
    {
        stateManager.enableSOSMode(true); 
    });

    setup_tasks();
}

void setup_tasks() {
    runner.addTask(lora_check_task);
    runner.addTask(gps_check_task);
    runner.addTask(rfid_check_task);
    runner.addTask(btn_check_task);
    runner.addTask(buzzer_check_task);
    runner.addTask(oled_refresh_task);
    runner.addTask(led_check_task);
    runner.addTask(axis_refresh_task);
    runner.addTask(heart_refresh_task);
    runner.addTask(mic_check_task);

    lora_check_task.enable();
    gps_check_task.enable();
    rfid_check_task.enable();
    btn_check_task.enable();
    buzzer_check_task.enable();
    oled_refresh_task.enable();
    led_check_task.enable();
    axis_refresh_task.enable();
    // heart_refresh_task.enable();
    mic_check_task.enable();
}

void loop() 
{
    runner.execute();
}
