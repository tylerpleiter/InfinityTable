// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "ssid";
const char* password = "password";

unsigned int choice = 1;
char colourChoice[] = "0261d5";
bool newValue = false;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
char serialBufSend[10];

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  //  Serial.print("Connecting to ");
  //  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //    Serial.print(".");
  }
  // Print local IP address and start web server
  //  Serial.println("");
  //  Serial.println("WiFi connected.");
  //  Serial.println("IP address: ");
  //  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (Serial.available() > 0) {
    if (newValue) {
      Serial.println(serialBufSend);
      newValue = false;
    }
    Serial.readStringUntil('\n');
  }

  if (client) {                             // If a new client connects,

    //    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            memset(serialBufSend, '\0', 10);
            if (header.indexOf("GET /c") >= 0) {
              strncpy(colourChoice, &header[header.indexOf("GET /c") + 6], 6);
              serialBufSend[0] = 'c';
              strncat(serialBufSend, colourChoice, 7);
              choice = 0;
              newValue = true;
            } else if (header.indexOf("GET /p") >= 0) {
              strncpy(serialBufSend, &header[header.indexOf("GET /p") + 5], 3);
              choice = (int)strtol(&header[header.indexOf("GET /p") + 6], NULL, 10);
              newValue = true;
            }


            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println("body {background-color: black;}\n h1 {color:white;}");
            client.println(".button { background-color: #7ab6fe; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; height: 70px; width: 200px;}");
            client.println(".active {background-color: #0261d5;}");
            client.println(".colorpicker {border: none; padding: none; height: 70px; width: 200px;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>Infinity Table </h1> ");

            // Color Picker at top of page
            client.print("<p><input type=\"color\" class=\"colorpicker\" id=\"colorPickerId\" value=\"#");
            client.print(colourChoice);
            client.println("\"></p>");

            if (choice == 0) {
              client.println("<p><a href = \"/c\" id=\"colorPickerLink\" onclick=\"getInputValue();\"><button class=\"button active\">Colour</button></a></p>");
            } else {
              client.println("<p><a href=\"/c\" id=\"colorPickerLink\" onclick=\"getInputValue();\"><button class=\"button\">Colour</button></a></p>");
            }

            if (choice == 1) {
              client.println("<p><a href=\"/p01\"><button class=\"button active\">Rainbow</button></a></p>");
            } else {
              client.println("<p><a href=\"/p01\"><button class=\"button\">Rainbow</button></a></p>");
            }

            if (choice == 2) {
              client.println("<p><a href=\"/p02\"><button class=\"button active\">Glitter</button></a></p>");
            } else {
              client.println("<p><a href=\"/p02\"><button class=\"button\">Glitter</button></a></p>");
            }

            if (choice == 3) {
              client.println("<p><a href=\"/p03\"><button class=\"button active\">Confetti</button></a></p>");
            } else {
              client.println("<p><a href=\"/p03\"><button class=\"button\">Confetti</button></a></p>");
            }

            if (choice == 4) {
              client.println("<p><a href=\"/p04\"><button class=\"button active\">Sinelon</button></a></p>");
            } else {
              client.println("<p><a href=\"/p04\"><button class=\"button\">Sinelon</button></a></p>");
            }

            if (choice == 5) {
              client.println("<p><a href=\"/p05\"><button class=\"button active\">Juggle</button></a></p>");
            } else {
              client.println("<p><a href=\"/p05\"><button class=\"button\">Juggle</button></a></p>");
            }

            if (choice == 6) {
              client.println("<p><a href=\"/p06\"><button class=\"button active\">BPM</button></a></p>");
            } else {
              client.println("<p><a href=\"/p06\"><button class=\"button\">BPM</button></a></p>");
            }

            if (choice == 7) {
              client.println("<p><a href=\"/p07\"><button class=\"button active\">Noise</button></a></p>");
            } else {
              client.println("<p><a href=\"/p07\"><button class=\"button\">Noise</button></a></p>");
            }

            if (choice == 8) {
              client.println("<p><a href=\"/p08\"><button class=\"button active\">NoiseSingle</button></a></p>");
            } else {
              client.println("<p><a href=\"/p08\"><button class=\"button\">NoiseSingle</button></a></p>");
            }

            if (choice == 9) {
              client.println("<p><a href=\"/p09\"><button class=\"button active\">Waves</button></a></p>");
            } else {
              client.println("<p><a href=\"/p09\"><button class=\"button\">Waves</button></a></p>");
            }

            if (choice == 10) {
              client.println("<p><a href=\"/p10\"><button class=\"button active\">Dripping</button></a></p>");
            } else {
              client.println("<p><a href=\"/p10\"><button class=\"button\">Dripping</button></a></p>");
            }

            // Send script to get color from colorpicker
            client.println("<script>\n function getInputValue() {\n var colorLink = \"/c\";");
            client.println(" colorLink = colorLink.concat(document.getElementById(\"colorPickerId\").value.substring(1));");
            client.println(" document.getElementById(\"colorPickerLink\").href=colorLink;\n}</script>");

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    //    Serial.println("Client disconnected.");
    //    Serial.println("");
  }
}
