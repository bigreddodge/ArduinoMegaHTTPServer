/*
  Arduino Mega HTTP Server
  Imran A. Uddin
  Verified 7/24/2016
  
  Implementation Notes
  With Ethernet Shield Installed
  Static IP configuration
  Requires HTTP port forwarding to 192.168.1.200:8080
 */

#include <SPI.h>
#include <Ethernet.h>

//Declarations

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 200);

String q = "\"";
String link[3] = {"<br><a href=", "http://freedns.afraid.org/", ">Visit FreeDNS</a>"};

// Initialize the Ethernet server library
EthernetServer server(8080);

void setup() {
  pinMode(53, INPUT_PULLUP);
  Serial.begin(9600);
  while (!Serial) {}
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          sendHeader(client);
          // output the value of each analog input pin
          sendPage(client);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void sendHeader(EthernetClient client)
{
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head><title>vapid.serverpit.com:41</title></head>");
}
void sendPage(EthernetClient client)
{
          client.println("<body bgcolor=#000000 text=#ffffff>");
          client.println("<h1>http://vapid.serverpit.com:41</h1><p>");
          client.println("Arduino Mega 2560 R3</p><p>");         
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("Analog Channel ");
            client.print(analogChannel);
            client.print(": ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.print("Digital Input 53: ");
          client.println(digitalRead(53));
          client.println("</p><p>");
          //Debugging
            client.println("<form><button name="+q+"Button"+q+"value="+q+"Submit"+q+"type="+q+"button"+q+">Click Here</button></form>");
          //End Debugging
          client.println(link[0] + q + link[1] + q + link[2]);
          client.println("</p></body></html>");
}
