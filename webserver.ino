
#include <ESP8266WiFi.h>
const char* ssid     = "xuanhoang";
const char* password = "xuanhoang";
WiFiServer server(80);
String header;
String led_stt = "off";
String speaker_stt= "off";

const int led = D4;
const int speaker = D3;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(speaker, OUTPUT);
  digitalWrite(led, LOW);
  digitalWrite(speaker, LOW);

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

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character

          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            }
            
            if (header.indexOf("GET /led1/on") >= 0) {
              Serial.println("led1 on");
              led_stt = "on";
              digitalWrite(led, HIGH);
            } else if (header.indexOf("GET /led1/off") >= 0) {
              Serial.println("led1 off");
              led_stt = "off";
              digitalWrite(led, LOW);
            } else if (header.indexOf("GET /led2/on") >= 0) {
              Serial.println("led2 on");
              speaker_stt= "on";
              digitalWrite(speaker, HIGH);
            } else if (header.indexOf("GET /led2/off") >= 0) {
              Serial.println("led2 off");
              speaker_stt = "off";
              digitalWrite(speaker, LOW);
            }
             // turns the GPIOs on and off
            
           for (int i = 0 ; i < 256 ; i ++) {
                char a[100],b[100];
                sprintf(a , "GET /1/%d/pwm", i);
                sprintf(b , "GET /2/%d/pwm", i);
                if( header.indexOf(a) >=0) {

                  Serial.print("led Get ");
                  Serial.println(i);
                  analogWrite(led , i);
               
                  }  
                if( header.indexOf(b) >=0) {
                  Serial.print("coi Get ");
                  Serial.println(i);
                  analogWrite(speaker , i);
        
                  }
             }
            
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<html lang=\"vi-VN\">");
            client.println("<meta charset=\"utf-8\">");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #A52A2A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 25px; margin-left: 400px; border-radius: 400px; cursor: pointer;}");
            
            client.println(".button:hover {background-color: red;font-size: 40px}");
             client.println("div { width:140px;height:100px;margin:50px;float:left;}");
            client.println(".led9{clear:left;margin-left: 450px;}");
            client.println(".button2 {background-color: #000000;}</style></head>");
            
            
            // Web Page Heading
            client.println("<body background=\"https://img.thuthuattinhoc.vn/uploads/2019/01/13/anh-dep-vung-nui-viet-nam_105620901.jpeg\"><h1>THỰC HÀNH CHUYÊN SÂU</h1>");
            client.println("<body><h2><b>SV: PHẠM HUY NAM</b></h2></body>");
            if (led_stt=="off") {
              client.println("<div <p><a href=\"/led1/on\"><button class=\"button\">LED_1 ON</button></a></p> </div>");
            } else {
              client.println("<div <p><a href=\"/led1/off\"><button class=\"button button2\">LED_1 OFF</button></a></p></div>");
            } 
                  
            if (speaker_stt=="off") {
              client.println("<div <p><a href=\"/led2/on\"><button class=\"button\">LED_2 ON</button></a></p></div>");
            } else {
              client.println("<div <p> <a href=\"/led2/off\"><button class=\"button button2\">LED_2 OFF</button></a></p></div>");
            }
            //////
            client.println("<div class = \"led9\" ><input type=\"range\" min=\"0\" max=\"255\" value=\"255\" class=\"value_led\" id=\"led_value\">");
            client.println("<p>STT_LED: <span id=\"led\"></span></p></div>");

            client.println("<div> <input type=\"range\" min=\"0\" max=\"255\" value=\"255\" class=\"value_speaker\" id=\"speaker_value\">");
            client.println("<p>STT_SPEAKER: <span id=\"coi\"></span></p></div>");

            
            client.println("<script> var value_led = document.getElementById(\"led_value\");");
            client.println("var led_output = document.getElementById(\"led\");");


            client.println("var value_speaker = document.getElementById(\"speaker_value\");");
            client.println("var coi_output = document.getElementById(\"coi\");");
            

            
            client.println("var huynam = new XMLHttpRequest();");
           
            client.println("led_output.innerHTML = value_led.value;");
            client.println("coi_output.innerHTML = value_speaker.value;");
            
            client.println("value_led.oninput = function() { led_output.innerHTML = this.value;");       
            client.println("huynam.open('GET', \"1/\" + led_output.innerHTML+\"/pwm\", true);");
            client.println("huynam.send();}");
            client.println("value_speaker.oninput = function() { coi_output.innerHTML = this.value;");       
            client.println("huynam.open('GET', \"2/\"+ coi_output.innerHTML+\"/pwm\", true);");
            client.println("huynam.send();} </script>");


            
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
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
