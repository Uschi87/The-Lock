#include <Arduino.h>
#include <base64.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <Servo.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

String randNumber;
String toEncode;
String encoded;
String lockstate ="Locked";

AsyncWebServer server(80);

//Define Servo
//Servo Servo_SG90;
//int pos = 0;

// Create new WiFi AP
const char* ssid = "THE-LOCK";
const char* password = "123456";
const char* PARAM_INPUT_1 = "input1";

// HTML web page to handle Pass Phrase input)
const char index_html[] = R"rawliteral(
<!DOCTYPE HTML><html><head>
<div style="text-align:center">
<body style="background-color:powderblue;">
  <title>The Lock V1</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h1>The Lock Controller</h1><br><br>
  <h2>Passphrase</h2><br><br>
  <form action="/get">
    Unlock Code: <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
    // Initalize Display
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  //Set Screen Output
  delay(2000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  // Generate Random Password
  randomSeed(analogRead(1));
  randNumber = random(100, 999);
  Serial.print('\n');
  Serial.print('\n');
  Serial.print("Decrypted PW: ");
  Serial.println(randNumber);
  String toEncode = (randNumber);
  String encoded = base64::encode(toEncode);
  Serial.print("Encrypted PW: ");
  Serial.println(encoded);

// Display static text
  display.clearDisplay();
  display.println("  " + lockstate);
  display.println("##########");
  display.print(" ID: ");
  display.print(encoded);
  display.display();

// Enable WiFi
  //WiFi.mode(WIFI_AP);
  WiFi.softAP("THE-LOCK","");
  Serial.print("Create WiFi ... ");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

//Servo 
//  Serial.println("Verbinde Servo.");
//  Servo_SG90.attach(2); // 2 = D4
//  Serial.println("Servo auf 0 Grad stellen");
//  Servo_SG90.write(0);

 // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });


  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
     // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
      if (inputMessage ==encoded) {
      lockstate = "unlocked";
      Serial.println(lockstate);
      // Display static text
      display.clearDisplay();
      display.println("  " + lockstate);
      display.println("##########");
      display.print(" ID: ");
      display.print(encoded);
      display.display();
      }
  });
  
  server.onNotFound(notFound);
  server.begin();



}

void loop() {

}