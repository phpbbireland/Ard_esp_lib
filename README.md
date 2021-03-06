###Arduino ESP8266 Library (Ard_esp_lib)

A planned library for development of Neuron Tech (https://hackaday.io/projects/hacker/31514) projects  

**Changes**
* Dropped the need for an additional microcontroller...
* Programmed with the Arduino IDE (https://github.com/esp8266/Arduino)...  

**ESP8266**
* Wireless SoC
* GPIO, I2C, ADC, SPI, PWM and some more
* Running at 80MHz
* 64KB of instruction RAM
* 96KB of data RAM
* 64KB boot ROM
* SPI flash (Winbond W25Q40BVNIG)
* RISC architecture
* Deep sleep power <10uA, Power down leakage current < 5uA
* Wake up and transmit packets in < 2ms
* Standby power consumption of < 1.0mW (DTIM3)

**Sample Sketches**  
A sample sketch in: WiFiWebServer/WiFiWebServer.ino compiled with arduino-1.6.5 IDE is included for testing. It will allow setting of any GPIO (assuming you have access to it) from the web interface (browser address bar)...  

In one of the planned application, several ESP modules incorporating various sensors will collect real time security information and forward this information via WiFi to a web server (local or remote)...  

**The proposed Web Server**  
* User editable pages...
* Database backbend for storing real time information...
* Graphical representation of your home (a floor plan using simple images)...
* Position and status of each ESP modules (on/off, battery condition etc)...
* Remote control of each ESP device via the web page (PC/Mobile)...
* Signaling via messaging or text to mobile...  

Added code for ESP32 Web Server with some nice CSS so you don't have to resize the page on your phone every time you click a button...


