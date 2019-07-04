#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// KAYNAK: https://www.electronicwings.com/nodemcu/http-server-on-nodemcu-with-arduino-ide
// TARIH: 2019-07-04, Nuri Akman

/*Put your SSID & Password*/
const char* ssid = "ssid";  // Enter SSID here
const char* password = "password";  //Enter Password here

ESP8266WebServer server(80);

uint8_t LED1pin = D1; bool LED1status = LOW;
uint8_t LED2pin = D2; bool LED2status = LOW;

void setup() {
  Serial.begin(9600);
  delay(100);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  } // while bitti
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);

  server.on("/led1on",  handle_led1on);
  server.on("/led2on",  handle_led2on);

  server.on("/led1off", handle_led1off);
  server.on("/led2off", handle_led2off);

  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
} // setup() bitti


void loop() {
  server.handleClient();
  if(LED1status) digitalWrite(LED1pin, HIGH); else digitalWrite(LED1pin, LOW);
  if(LED2status) digitalWrite(LED2pin, HIGH); else digitalWrite(LED2pin, LOW);
} // loop() bitti

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  server.send(200, "text/html", SendHTML()); }

void handle_led1on()  { LED1status = HIGH; server.send(200, "text/html", SendHTML()); }
void handle_led1off() { LED1status = LOW;  server.send(200, "text/html", SendHTML()); }

void handle_led2on()  { LED2status = HIGH; server.send(200, "text/html", SendHTML()); }
void handle_led2off() { LED2status = LOW;  server.send(200, "text/html", SendHTML()); }

void handle_NotFound(){ server.send(404, "text/plain", "Not found"); }


String SendHTML(){
  String ptr = "<!DOCTYPE html>\n";
  ptr +="<html>\n";
  ptr +="<head>\n";
  ptr +="<meta charset='utf-8'>\n";
  ptr +="<title>LED Kontrol</title>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>LED</h1>\n";
  ptr +="<p>LEDi kontrol etmek için dugmeleri kullanabilirsiniz.</p>\n";
  ptr +="<form method=\"get\">\n";

  if(LED1status == HIGH)
    ptr +="<input type=\"button\" value=\"LED 1 OFF\" onclick=\"window.location.href='/led1off'\">\n";
  else
    ptr +="<input type=\"button\" value=\"LED 1 ON\" onclick=\"window.location.href='/led1on'\">\n";

  if(LED2status == HIGH)
    ptr +="<input type=\"button\" value=\"LED 2 OFF\" onclick=\"window.location.href='/led2off'\">\n";
  else
    ptr +="<input type=\"button\" value=\"LED 2 ON\" onclick=\"window.location.href='/led2on'\">\n";

  ptr +="</form>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
