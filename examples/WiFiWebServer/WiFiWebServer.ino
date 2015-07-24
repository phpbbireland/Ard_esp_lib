/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio=2?state=0 will set the GPIO 2 low,
 *    http://server_ip/gpio=A?state=1 will set the GPIO 10 high
 *  server_ip is the IP address of the ESP8266 module, will be
 *  printed to Serial when the module is connected.
 *  We can now set(1)/reset(0) any GPIO between 0 and 15...
 */

#include <ESP8266WiFi.h>

const char* ssid = "?????";
const char* password = "?????";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO0 & GPIO2 (ESP8266-01 boards) //
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  pinMode(0, OUTPUT);
  digitalWrite(0, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  unsigned int val;
  unsigned int gpio;
  unsigned int len = req.length();

  // the request will always have "/gpio" in it //
  unsigned int p1 = req.indexOf("gpio=");
  unsigned int p2 = req.indexOf("state=", p1 + 1);

  //char num = req[p1+5] + '\0';

  // convert ascii char to number //
  if (req[p1+5] + '\0' > 47 && req[p1+5] + '\0' < 58) // 0 to 9
  {
    gpio = req[p1+5] + '\0' - 48;
  }
  else if (req[p1+5] + '\0' > 64 && req[p1+5] + '\0' < 71) // A to F (10 to 15)
  {
    gpio = req[p1+5] + '\0' - 55;
  }
  /*
  else if (req[p1+5] + '\0' > 96 && req[p1+5] + '\0' < 103) // a to f (10 to 15)
  {
    gpio = req[p1+5] + '\0' - 87;
  }
  */
  else
  {
    len = 0;
  }

  val  = (req[p2+6] + '\0') - 48;

  if (len)
  {
    digitalWrite(gpio, val);
    client.flush();
  }
  else
  {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO (";
  s += gpio;
  s += ") has been set ";
  s += (val) ? "high" : "low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}

