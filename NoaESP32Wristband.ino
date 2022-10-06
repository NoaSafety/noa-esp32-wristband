/******************************************

        NN   NN    OOOOOO    AAA
       NN N NN    OO  OO    AAAA
      NN   NN    OOOOOO    AA AA

  Proudly written in glorious Europe
  
******************************************/

// --- Libraries --- //
//      LoRa
#include <SPI.h>
#include <LoRa.h>

//      OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//      C Variadic functions
#include <stdarg.h>

// --- Common Constants ---//
//      LoRa Pins
#define SCK                 (5)
#define MISO                (19)
#define MOSI                (27)
#define SS                  (18)
#define RST                 (14)
#define DIO0                (26)

/*      GEOGRAPHIC ZONES ---
 *          You can choose between: 
 *              Asia            0
 *              Europe          1
 *              North America   2
*/
#define ZONE                (1)

#if ZONE == 0 // ASIA
    #define BAND            (433E6)
#elif ZONE == 1 // EUROPE
    #define BAND            (866E6)
#elif ZONE == 2 // NORTH AMERICA
    #define BAND            (915E6)
#else
    #error "Invalid zone selected: Please select between (Asia = 0, Europe = 1, North America = 2)"
#endif


//      OLED Pins
#define OLED_SDA            (4)
#define OLED_SCL            (15) // 
#define OLED_RST            (16) // 
#define SCREEN_WIDTH        (128) // in pixels
#define SCREEN_HEIGHT       (64) // in pixels

// --- Functions --- //
void write_lora_packet(String data)
{
  LoRa.beginPacket();
  LoRa.print(data);
  LoRa.endPacket();
}

String format_position_to_json(double longitude, double latitude)
{
    char data[128];
    sprintf(data, "{\"longitude\":%f,\"latitude\":%f}", longitude, latitude);
    return data;
}

void die()
{
    for(;;);
}

// --- Classes --- //
class LoRaTransmitter
{
    public:
        LoRaTransmitter(int band) :
            m_band(band)
        {
            
        }

        bool begin()
        {
            // SPI LoRa Pins
            SPI.begin(SCK, MISO, MOSI, SS);
            // Setup LoRa transceiver module
            LoRa.setPins(SS, RST, DIO0);
          
            return LoRa.begin(m_band);
        }

        void write(String data)
        {
            LoRa.beginPacket();
            LoRa.print(data);
            LoRa.endPacket();
        }

    private:
        int m_band;
};

class OledDisplay
{
    public:
        OledDisplay(int width, int height, int resetPin, int size) : 
            m_display(width, height, &Wire, resetPin),
            m_size(size),
            m_lineSize(size * 10),
            m_line(0),
            m_width(width),
            m_height(height)
        {
            
        }

        bool begin(int reset)
        {
            pinMode(reset, OUTPUT);
            digitalWrite(reset, LOW);
            delay(20);
            digitalWrite(reset, HIGH);
  
            Wire.begin(OLED_SDA, OLED_SCL);
            if(m_display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) // 0x3C -> 128x32
            {
                clear();
                set_default_style();
                return true;
            }
            return false;
        }

        void clear()
        {
            m_display.clearDisplay();
            m_display.setCursor(0, 0);
            m_line = 0;
        }

        void set_default_style()
        {
            m_display.setTextColor(WHITE);
            m_display.setTextSize(m_size);
        }
            
        void push_line(String text)
        {
            clear_line(m_line);
            m_display.setCursor(0, m_lineSize * m_line);
            m_display.print(text);
            Serial.println(text);
            m_line ++;
            m_display.display();
        }

        void set_line(int line)
        {
            m_line = line;
            m_display.setCursor(0, m_lineSize * m_line);
        }

        void clear_line(int line)
        {
            for (auto y = m_lineSize * line, yMax = m_lineSize * (line + 1); y <= yMax; y ++)
            {
                for (auto x = 0; x < m_width; x ++)
                {
                    m_display.drawPixel(x, y, BLACK); 
                }
            }
        }
        
    private:
        Adafruit_SSD1306 m_display;
        int m_size;
        int m_lineSize;
        int m_line;
        int m_width;
        int m_height;
};

// --- Global Variables --- //
auto display = OledDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_RST, 1);
auto transmitter = LoRaTransmitter(BAND);

// --- Main Code ---//
void setup() 
{
    // Initialize Serial Monitor
    Serial.begin(115200);

    // Initialize OLED
    if(!display.begin(OLED_RST)) 
    { 
        Serial.println(F("SSD1306 allocation failed"));
        die();
    }
  
    display.push_line("Noa - Keep safe!");

    if(!transmitter.begin()) 
    {
        Serial.println("Starting LoRa failed!");
        die();
    }

    display.push_line("LoRa Initializing OK!");
    delay(2000);
}

void loop() 
{
    //Send LoRa packet to receiver
    auto data = format_position_to_json(10.0025, 156.18994);
    transmitter.write(data);

    display.set_line(3);
    display.push_line("Sent:");
    display.push_line(data);

    delay(10000);
}
