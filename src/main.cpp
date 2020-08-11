#include <Arduino.h>
#include <base64.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>


String randNumber;
 
void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(1));
  randNumber = random(1000, 9999);
  Serial.println(randNumber);
  String toEncode = (randNumber);
  String encoded = base64::encode(toEncode);
  Serial.println(encoded);
}

void loop() {
  // put your main code here, to run repeatedly:
}