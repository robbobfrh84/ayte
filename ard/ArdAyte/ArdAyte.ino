#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            12
#define NUMPIXELS      64
#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoJson.h>
#define JSON_BUFF_DIMENSION 2500
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


static char ssid[] = "2WIRE648";  
static char pass[] = "3519464889";  
int keyIndex = 0;           
int status = WL_IDLE_STATUS;

const char server[] = "pubsub.pubnub.com";    // name address for openweathermap (using DNS)
String text;
