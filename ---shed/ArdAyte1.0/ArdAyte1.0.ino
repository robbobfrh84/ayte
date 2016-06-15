#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            12
#define NUMPIXELS      64
#include <SPI.h>
#include <WiFi101.h>
//#include <ArduinoJson.h>
//#define JSON_BUFF_DIMENSION 2500
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
WiFiClient client;

static char ssid[] = "2WIRE648";  
static char pass[] = "3519464889";  
int keyIndex = 0;           
int status = WL_IDLE_STATUS;
const char server[] = "pubsub.pubnub.com";    // name address for openweathermap (using DNS)
String timeStamp = "0";

String text;
long int lastConnection;
long int action;

int endResponse = 0;
int cnt = 0;
boolean led6State = true;
boolean checking = false;
boolean startJson = false;

//web rgb *0.4, rounded down...
const char r[]={  15,200, 20, 85,  0,   40, 70, 40,130, 100,  10, 40,100, 25, 95,  0 };
const char g[]={  15,200, 40, 75, 70,   40, 20, 20, 0,  40,   70, 25, 70, 25, 120, 0 };
const char b[]={  15,200,100, 11,  0,   40, 15, 70, 0,  0,    70, 15, 80, 40, 25,  0 };

void setup(){
  Serial.begin(115200);
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  pinMode(6, OUTPUT);
  pixels.begin();
  if (WiFi.status() == WL_NO_SHIELD) { Serial.println("WiFi shield not present"); while (true);}
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting connect to SSID..."); Serial.println(ssid); status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  printWifiStatus(); 
  
  httpRequest("history"); //TURN BACK INTO ONE!!!
  delay(50);
  parseAyte();
  lastConnection = millis();
  action = millis();
}

//----------VOID LOOP----------VOID LOOP----------VOID LOOP----------VOID LOOP----------VOID LOOP----------//
void loop (){

  if (!checking) {
    digitalWrite(6, led6State); led6State = true ? !led6State : false;
    httpRequest("0"); Serial.print("!checking: "); Serial.print(timeStamp);
    if (timeStamp == ""){ Serial.println("____no TimeStamp___");
      delay(50); 
      httpRequest("0");
      delay(50); 
    }
    httpRequest(timeStamp); //Serial.print("OutCheck?");
  }
  parseAyte();

  if (millis() - lastConnection > 1000) {
    httpRequest("history"); 
    delay(50);
    parseAyte();
    lastConnection = millis();
    Serial.println("***");
  }

//  if (millis() - action > 15000) {
//    rainbowWormHole();
//    httpRequest("history"); 
//    delay(50);
//    parseAyte();
//    delay(500);
//  }
  

}
void parseAyte(){  
  char c = 0;
  if (client.available()) {
    c = client.read();
    if (endResponse == 0 && startJson == true) { Serial.println(text); 
      pixels.show();
      cnt = 0;
      text = "";  
      startJson = false;  
      checking = false;
    }
    if (c == '{') { startJson = true; endResponse++;}
    if (c == '}') { endResponse--; }
    if (startJson == true) {
      text += c;  
      int x = c-65; //65-to-90 is the upperCase Alphabit ASCII
      if (c+0 >= 64 && c+0 <= 90){ 
        { pixels.setPixelColor(cnt, pixels.Color(r[x],g[x],b[x])); }
        cnt+=1;
      }   
    }
  }
}  

void httpRequest(String timeState) { // this method makes a HTTP connection to the server:
  checking = true;
  client.stop(); 
  if (client.connect(server, 80)) {
    if(timeState == "history"){ client.println("GET /history/sub-c-b3fbc6fa-0bf5-11e6-a8fd-02ee2ddab7fe/1b/0/1 HTTP/1.1");
    } else {client.println("GET /subscribe/sub-c-b3fbc6fa-0bf5-11e6-a8fd-02ee2ddab7fe/1b/0/" + timeState + " HTTP/1.1"); }
    client.println("Host: pubsub.pubnub.com");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");
  }
  
  if(timeState == "0"){
    char c = 0;
    timeStamp = "";
    delay(50); 
    Serial.println("-0-");
    for (int j = 0; j < 1000; j++){
      c = client.read();
      if (c == '['){
        for (int i = 0; i < 4; i++){ c = client.read(); }
        for (int i = 0; i < 17; i++){
          c = client.read();
          timeStamp += c;
          if (c == ']'){ break;}
        }
      }  
    }
    lastConnection = millis();
    checking = false;      
  }
}

void printWifiStatus() { Serial.print("SSID: ");Serial.println(WiFi.SSID());IPAddress ip = WiFi.localIP();
Serial.print("IP Address: "); Serial.println(ip);long rssi = WiFi.RSSI();
Serial.print("signal strength (RSSI):");Serial.print(rssi); Serial.println(" dBm");}

//---------------------------------------------------------------------------------------------------------//
//----------          ANIMATIONS                                                                 ----------//
//---------------------------------------------------------------------------------------------------------//

void rainbowWormHole(){
  //Serial.println("Playing Rainbow Worm-hole");
  for (int i = 0; i < 3; i++){
    int x = random(0,63); 
    int r = random(0,90); 
    int g = random(0,90); 
    int b = random(0,90);
    pixels.setPixelColor(x, pixels.Color(r,g,b)); pixels.show();
    x = random(0,63); pixels.setPixelColor(x, pixels.Color(0,0,0)); pixels.show();
  }
  delay(200);
}

void randomPixelDanceHalfBlack(){
  Serial.println("Playing RandomPixelDanceHalfBlack");
}

void bluePixelWondersThroughHell(){
  Serial.println("Playing RandomPixelDanceHalfBlack");
}

//---------------------------------------------------------------------------------------------------------//
//----------          NOTES                                                                 ----------//
//---------------------------------------------------------------------------------------------------------//

//--create function parseTimeStamp()
