#include <WiFi.h>

const char* ssid     = "WifiName";
const char* password = "Password";

int motorOneIn = 4;
int motorOneOut = 2;
int motorTwoIn = 17;
int motorTwoOut = 5;

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(motorOneIn, OUTPUT);      // set the pins for the Motors
    pinMode(motorOneOut, OUTPUT);
    pinMode(motorTwoIn, OUTPUT);
    pinMode(motorTwoOut, OUTPUT);

    delay(10);

    // We start by connecting to a WiFi network

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
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/oneIn\">here</a> to turn Motor one IN.<br>");
            client.print("Click <a href=\"/oneOut\">here</a> to turn Motor one OUT.<br>");
            client.print("Click <a href=\"/twoIn\">here</a> to turn Motor two IN.<br>");
            client.print("Click <a href=\"/twoOut\">here</a> to turn Motor two OUT.<br>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /oneIn" or "GET /oneOut":
        if (currentLine.endsWith("GET /oneIn")) {
          digitalWrite(motorOneIn, HIGH);               // GET /oneIn tightens the Motor 
          delay(500);
          digitalWrite(motorOneIn, LOW);                
        }
        if (currentLine.endsWith("GET /oneOut")) {
          digitalWrite(motorOneOut, HIGH);              // GET /oneOut loosens the Motor off
          delay(500);
          digitalWrite(motorOneOut, LOW);                
        }
        // Check to see if the client request was "GET /twoOut" or "GET /twoIn":
        if (currentLine.endsWith("GET /twoIn")) {
          digitalWrite(motorTwoIn, HIGH);               // GET /twoIn tightens the Motor
          delay(500);
          digitalWrite(motorTwoIn, LOW);                
        }
        if (currentLine.endsWith("GET /twoOut")) {
          digitalWrite(motorTwoOut, HIGH);              // GET /twoOut loosens the Motor off
          delay(500);
          digitalWrite(motorTwoOut, LOW);                
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
