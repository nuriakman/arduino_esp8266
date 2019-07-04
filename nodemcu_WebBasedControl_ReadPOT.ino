#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// KAYNAK: https://www.electronicwings.com/nodemcu/http-server-on-nodemcu-with-arduino-ide
// TARIH: 2019-07-04, Nuri Akman

/*Put your SSID & Password*/
const char* ssid = "ssid";  // Enter SSID here
const char* password = "password";  //Enter Password here

ESP8266WebServer server(80);

#define pot A0
int POT_Degeri=0;

void setup() {
  Serial.begin(9600);
  delay(100);

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
  server.on("/potOKU", handle_potOKU);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
} // setup() bitti


void loop() {
  server.handleClient();
} // loop() bitti

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

void handle_potOKU() {
  // Pot değerini okuyalım...
  POT_Degeri  = analogRead(pot);
  Serial.println(POT_Degeri);
  
  server.send(200, "text/html", SendHTML());
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(){
  String ptr = "<!DOCTYPE html>\n";
  ptr +="<html>\n";
  ptr +="<head>\n";
  ptr +="<title>POT Okuma</title>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>POT Oku</h1>\n";
  ptr +="<p>POT değerini okumak için düğmeye tıklayınız</p>\n";
  ptr +="<form method=\"get\">\n";
  ptr +="<input type=\"button\" value=\"POT OKU\" onclick=\"window.location.href='/potOKU'\">\n";
  ptr +="</form>\n";
  ptr +="<h1>Okunan POT Değeri:</h1>\n";
  ptr +="<div style='color:red;font-size: 160px;'>";
  ptr +=String(POT_Degeri);
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
} // SendHTML() bitti
