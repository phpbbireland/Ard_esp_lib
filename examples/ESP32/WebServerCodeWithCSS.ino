/*
 Version 1.2.2 (updated) 26/04/2019 05:54

 Sample WiFi Web Server for ESP32 with correctly sized text for your phone,
 no need to keep resizing every time you click a button plus I've added some nice css...

 A simple web server that lets you blink a LED on you ESP32 via any browser...
 
 This sketch will print the IP address to the serial monitor once connected to the ESP32...
 From there, you can open that address in a web browser to turn on/off the LED and set GPIO 4 low/high (tested on the doit board which uses pin 2 for the blue led).

 To program your ESP32 using Arduino IDE, you may need to first set the ESP32 in program mode...
 Do this by holding down the BOOT button and then pressing the EN button, then release both...

 If you get compile errors, ensure you have the correct board selected (compiles with ESP32 Wrover Module)
 Opening the serial monitor may provide some useful information (depending on the previously uploaded program)...

 Note I use an old school placement for '{}' (curly braces) which makes the code more readable IMHO...
 Mike
*/

#include <WiFi.h>
#include <time.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif


#define BLINK_GPIO 2
#define GPIO4 4

const char* ssid     = "your ssid";
const char* password = "your password";

uint8_t state;
uint8_t state4;
uint8_t temp_farenheit;
float temp_celsius;

WiFiServer server(80);

const char* style = "<style type=\"text/css\"> a{color:#476669;text-decoration:none;}.formbox{margin:auto;width:90%;text-align:center;background-color:#fdf6e3;border:solid 1px #777;border-radius:20px;line-height:70px;box-shadow:2px 2px 2px 2px #558d93;border-radius:12px;padding:10px;} @media screen and (max-width:1024px){body{background:cyan;}.formbox{font-size:5vw;min-height:10vw;line-height:120px;}</style>";

const char* formboxstartx = "<div class=\"formboxx\">";
const char* formboxstart = "<div class=\"formbox\">";
const char* formboxend = "</div></a><br /><br />";
const char* formboxendx = "</div><br /><br />";

void setup()
{
    Serial.begin(115200);
    pinMode(BLINK_GPIO, OUTPUT);  // set the LED pin mode
    pinMode(GPIO4, OUTPUT);       // set the LED pin mode

    delay(10);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    printf("Using GPIO pin %d for LED\n", BLINK_GPIO);
    server.begin();
}

void loop()
{
  int onoff = 0;
  WiFiClient client = server.available();   // listen for incoming clients

  if (client)
  {                                         // if you get a client,
    Serial.println("New Client.");          // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected())
    {                                       // loop while the client's connected
      if (client.available())
      {                                     // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n')                      // if the byte is a newline character
        {                                   // if the current line is blank, you got two newline characters in a row. // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            break;                                                // break out of the while loop:
          }
          else
          {                                                       // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                                                         // if you got anything else but a carriage return character,
          currentLine += c;                                       // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /H2"))                      // Check to see if the client request was "GET /H" or "GET /L":
        {
          digitalWrite(BLINK_GPIO, HIGH);                         // GET /H turns the LED on
          Serial.println("\nTurning Led On\n");
        }
        else if (currentLine.endsWith("GET /L2")) {
          digitalWrite(BLINK_GPIO, LOW);                          // GET /L turns the LED off
          Serial.println("\nTurning Led Off\n");
        }

        if (currentLine.endsWith("GET /H4"))                      // Check to see if the client request was "GET /H" or "GET /L":
        {
          digitalWrite(GPIO4, HIGH);                              // GET /H turns the LED on
          Serial.println("\nTurning GPIO4 High\n");
        }
        else if (currentLine.endsWith("GET /L4"))
        {
          digitalWrite(GPIO4, LOW);                               // GET /L turns the LED off
          Serial.println("\nTurning GPIO4 Low\n");
        }
        
      }
    }

    client.println("HTTP/1.1 200 OK");  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK) and a content-type so the client knows what's coming, then a blank line:
    client.println("Content-type:text/html");
    client.println();
    client.print("<!doctype html><html lang=\"en\"><head><meta charset=\"UTF-8\">");  // the content of the HTTP response follows the header:
    client.print(style);
    client.print("</head><body style=\"margin: auto auto; width:94%; max-width: 800px; margin-top:60px; font-size:16px; color:#000; \" >");

    state = digitalRead(BLINK_GPIO);
    state4 = digitalRead(4);
    onoff = state;
    temp_celsius = get_temp();
    
    client.print(formboxstart);      
    client.printf("Temperature: %4.2f °C ", temp_celsius);
    client.print(formboxendx);
            
    if(!onoff)
    {
      client.print("<a href=\"/H2\">");
      client.print(formboxstart);
      client.print("Turn the led <strong>On</strong>");
      client.print(formboxend);
    } 
    else
    {
      client.print("<a href=\"/L2\">");
      client.print(formboxstart);
      client.print("Turn the Led <strong>off</strong>");
      client.print(formboxend);
    }

    if(!state4)
    {
      client.print("<a href=\"/H4\">");
      client.print(formboxstart);
      client.print("Set GPIO4 <strong>High</strong>");
      client.print(formboxend);
    } 
    else
    {
      client.print("<a href=\"/L4\">");
      client.print(formboxstart);
      client.print("Set GPIO4 <strong>Low</strong>");
      client.print(formboxend);
    }

    client.print("<a href=\"/\">");
    client.print(formboxstart);    
    client.print("Get System Status");
    client.print(formboxend);
    client.print("</body></html>");                       // The HTTP response ends with another blank line:
    client.println();
  
    // close the connection:
    client.stop();
    Serial.println("");        
    Serial.print("Temperature = [");
    Serial.print(temp_celsius);        
    Serial.println("]");
    Serial.println("Client Disconnected.");
  }
}

float get_temp()
{
  temp_farenheit= temprature_sens_read();
  temp_celsius = ( temp_farenheit - 32 ) / 1.8;
  return(temp_celsius);
}
