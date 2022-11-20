/******************************************

        NN   NN    OOOOOO    AAA
       NN N NN    OO  OO    AAAA
      NN   NN    OOOOOO    AA AA

  Proudly written in glorious Europe
  
******************************************/

// --- Libraries --- //
//      GPS
#include "Position.h"

//      LoRa
#include "LoRaTransmitter.h"

//      PulseSensor
#include "HeartBeatSensor.h"

//      OLED Display
#include "OledDisplay.h"

//      RFID Card
#include <MFRC522.h>

//      C Variadic functions
#include "Message.h"

// --- Common Constants ---//
//      RFID Pins
#define RFID_RST_PIN        (0)
#define RFID_SS_PIN         (5)

//      LoRa Pins
#define LORA_SCK_PIN        (5)
#define LORA_MISO_PIN       (19)
#define LORA_MOSI_PIN       (27)
#define LORA_SS_PIN         (18)
#define LORA_RST_PIN        (14)
#define LORA_DIO0_PIN       (26)
        
//      GPS Pins
#define GPS_RX_PIN          (16)
#define GPS_TX_PIN          (17)

//      Heartbeat Sensor Pins
#define HEART_BEAT_PIN      (34) // ADC1_CH6

/*      GEOGRAPHIC ZONES ---
 *          You can choose between: 
 *              Asia            0
 *              Europe          1
 *              North America   2
*/
#define ZONE                (1)

#if ZONE == 0 // ASIA
    #define LORA_BAND       (433E6)
#elif ZONE == 1 // EUROPE
    #define LORA_BAND       (868E6)
#elif ZONE == 2 // NORTH AMERICA
    #define LORA_BAND       (915E6)
#else
    #error "Invalid zone selected: Please select between (Asia = 0, Europe = 1, North America = 2)"
#endif


//      OLED Pins
#define OLED_SDA_PIN        (4)
#define OLED_SCL_PIN        (15) // 
#define OLED_RST_PIN        (16) // 
#define SCREEN_WIDTH        (128) // in pixels
#define SCREEN_HEIGHT       (64) // in pixels

// --- Functions --- //
void die()
{
    for(;;);
}

// --- Global Variables --- //
//      LoRa Pins
auto display = OledDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_RST, 1, OLED_SDA_PIN, OLED_SCL_PIN);
auto transmitter = LoRaTransmitter(LORA_BAND, LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
auto pos = Position(GPS_RX_PIN, GPS_TX_PIN, 50.62021924615207, 5.582367411365839);
auto heartBeatSensor = HeartBeatSensor(HEART_BEAT_PIN);
MFRC522::MIFARE_Key key;
auto rfid_reader = MFRC522(RFID_SS_PIN, RFID_RST_PIN);

// --- Main Code ---//
void setup() 
{
    // Initialize Serial Monitor
    Serial.begin(115200);

    // SPI
    SPI.begin();

    // Initialize OLED
    if(!display.begin(OLED_RST)) 
    { 
        Serial.println(F("SSD1306 allocation failed"));
        die();
    }

    display.push_line("Initializing...");
    display.push_line("Noa - Keep safe!");

    if(!transmitter.begin()) 
    {
        Serial.println("Starting LoRa failed!");
        die();
    }

    display.push_line("LoRa Initializing OK!");

    if(!pos.begin())
    {
        Serial.println("Starting GPS Module failed!");
    }
    
    delay(2000);
}

void loop() 
{
    /*heartBeatSensor.output();
    //Send LoRa packet to receiver
    Serial.println("read heartbeat");
    auto beat = heartBeatSensor.read();*/
    
    //Send LoRa packet to receiver
    Message msg(pos);
    pos.update();
    
    auto data = msg.to_json();
    transmitter.write(data);

    display.set_line(3);
    display.push_line("Sent:");
    display.push_line(data);

    delay(10000);
}
