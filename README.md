# Development Environment Installation

1. Install Arduino IDE (lower than V2.0...)
2. Add ESP32 boards: 
	* Files > Preferences
	* Set Additional Boards Manager URLs: [https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json)
	* Press OK
	* Go to Tools > Boards > Boards Manager
	* Install esp32 (by Espressif Systems)
3. Set the good COM port (See in Devices Manager if you don't know) in Tools > Port
4. Set the right board in Tools > Board > ESP32 Arduino > TTGO LoRa32-OLED
5. Code
6. Press *Upload* to set your code

# Dependencies Requirement

In order to compile NoaWristband, you'll need to install the following dependencies : 
- Adafruit SSD1306 by Adafruit
- LoRa by Sandeep Mistry
- MFRC522 by Github Community
- NDEF_MFRC522 by CLaus Maveke
- PulseSensorPlayground Joel Murphy
- TaskScheduler by Anatoli Arkhipenko
- WPI430-VMA430 GPS
