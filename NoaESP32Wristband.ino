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

//      GPS
//#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//      PulseSensor
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

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

//      GPS Pins
#define GPS_RX              (16)
#define GPS_TX              (17)

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
    #define BAND            (433E6)
#elif ZONE == 1 // EUROPE
    #define BAND            (868E6)
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
void die()
{
    for(;;);
}

// --- Classes --- //
class IDataFragment
{
    public:
        virtual void update() = 0;
};

class Position : public IDataFragment
{
    public:
        Position() : m_ss(16, 17) // RX, TX
        {
           
        }

        bool begin() 
        {
            clear_buffer();
            m_ss.begin(9600);  
            return true;
        }
        
        inline double Longitude() const { return m_longitude; }
        inline double Latitude() const { return m_latitude; }
        
        void update() override
        {
            while(m_ss.available())
            {
                m_buffer[m_messageLength++] = m_ss.read();
                if(m_messageLength == 128)
                    break;

                Serial.write(m_buffer, m_messageLength);
                clear_buffer();
            }
        }

    private:
        void clear_buffer()
        {
            for(auto i = 0; i < m_messageLength; i ++)
            {
                m_buffer[i] = '\0';
            }

            m_messageLength = 0;
        }
        
    private:
        double m_latitude { 50.62021924615207 };
        double m_longitude { 5.582367411365839 };
        unsigned char m_buffer[128];
        size_t m_messageLength { 128 };
        //TinyGPSPlus m_gps;
        SoftwareSerial m_ss;
};

class ISerializable
{
    public:
        virtual String to_json() = 0;
};

class Message : public ISerializable
{
    public:
        Message(Position& position) : m_position(position)
        {
          
        }
        
        String to_json() override
        {
            char data[128];
            sprintf(data, "{\"longitude\":%f,\"latitude\":%f}", m_position.Longitude(), m_position.Latitude());
            return data;
        }

    private:
        Position &m_position;
};

class HeartBeatSensor
{
    public:
        HeartBeatSensor(int pin, int threshold = 550) :
            m_pin(pin),
            m_threshold(threshold)
        {
        
        }

        bool begin()
        {
            m_pulseSensor.analogInput(m_pin);
            /*m_pulseSensor.blinkOnPulse(PULSE_BLINK);
            m_pulseSensor.fadeOnPulse(PULSE_FADE);*/
            
            m_pulseSensor.setSerial(Serial);
            m_pulseSensor.setOutputType(SERIAL_PLOTTER);
            m_pulseSensor.setThreshold(m_threshold);
            return true;
        }

        void output()
        {
            m_pulseSensor.outputSample();
        }

        void output_beat()
        {
            if(m_pulseSensor.sawStartOfBeat())
            {
                m_pulseSensor.outputBeat();
            }
        }

        inline int read()
        {
            // return analogRead(m_pin);
            return m_pulseSensor.getLatestSample();
        }
        
    private:
        int m_pin;
        int m_threshold;
        PulseSensorPlayground m_pulseSensor;
};

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
Position position;
auto heartBeatSensor = HeartBeatSensor(HEART_BEAT_PIN);

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

    if(!position.begin())
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
    // auto data = format_position_to_json(10.0025, 156.18994);
    Message msg(position);
    position.update();
    
    auto data = msg.to_json();
    // auto data = "Salut les benz !\r\n";
    transmitter.write(data);

    display.set_line(3);
    display.push_line("Sent:");
    display.push_line(data);

    delay(10000);
}
