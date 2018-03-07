#include <Ard_esp_lib.h>
#include <SoftwareSerial.h>
#define ESP_EN 6
#define ESP_Rx 2
#define ESP_Tx 3
#define LED 7

/**
*  Set the ESP to Access Point and use iPhone to
*  control a given led... You could also send some text
*  in the form of 1's and 0's and translate to HEX to
*  control other devices such as an array of Leds...
*
*  A free apps (WiFi TCP Test Tool) from the App Store is used in these tests...
*  It supports sending three pieces of information...
*  For out testing we send: "LED ON", "LED OFF" and some text...
**/


ESP wifi(ESP_Rx, ESP_Tx, ESP_EN);

void print_debug(String data)
{
  Serial.print(data);
}
void setup()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.begin(9600);  // as desired //
  wifi.begin(9600);    // esp8266 baud rate as appropriate //
  
  Serial.println("Test: Closed AP 12345678");
  wifi.reset();

  // Set up closed Access Point //
  wifi.setmode(STATION_AP);
  
  //  look like something doesn't like spaces in the access point name, is this normal? //
  
  if(wifi.setAP("ESP_AP_Secure", "12345678", 1, 4)) 
  {
    wifi.multipleconnect(MULTIPLE);
    wifi.startserver("8000");
    String ip = wifi.myip();
    Serial.println(ip);
  } else
  {
    Serial.print("Heuston!");
  }
}

void loop()
{
  String data = wifi.readstringdata();

  if(data.length())
  {
    if(data == "LED ON")
    {
      digitalWrite(LED, HIGH);
      Serial.println(data);
    }
    else if(data == "LED OFF")
    {
      digitalWrite(LED, LOW);
      Serial.println(data);
    }
    else
    {
      Serial.println("[" + data + "]");
    }
  }
/*
  if (wifi.available())
  {
    if(wifi.find("Link"))
    {
      while(!wifi.find("Unlink"))
      {
        if(!wifi.println(0, "Send from esp8266"))
        {
          break;
        }
        delay(500);
      }
      wifi.debug("Connection closed");
    }
  }
*/  
  // jic //
  data = "";
}
