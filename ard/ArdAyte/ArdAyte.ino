#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            12
#define NUMPIXELS      64
#include <SPI.h>
#include <WiFi101.h>
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
WiFiClient client;
 
static char ssid[] = "2WIRE648";  
static char pass[] = "3519464889";  
int keyIndex = 0;           
int status = WL_IDLE_STATUS;
const char server[] = "pubsub.pubnub.com";    // name address for openweathermap (using DNS)

String oldAyte = "initial";
String displayedAyte;
String oldGal = "initial";
String galAyte;
String text;
String timeStamp = "0";
long int missedUpdateCheck;
long int action;
long int delayA = 10000;
int endResponse = 0;
int cnt = 0;
boolean led6State = true;
boolean checking = false;
boolean startJson = false;
boolean recentAction = true;
boolean blank = true;
boolean galCheck = true;

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

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting connect to SSID..."); Serial.println(ssid); status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  printWifiStatus(); 
  httpRequest("history"); //TURN BACK INTO ONE!!!
  delay(50);
  parseAyte("live");
  missedUpdateCheck = millis();
  action = millis();

}

//----------VOID LOOP----------VOID LOOP----------VOID LOOP----------VOID LOOP----------VOID LOOP----------//
void loop (){

  if (!checking) {
    if (oldAyte != displayedAyte){ Serial.println("---!!!   NEW LIVE AYTE   !!!---"); 
      Serial.print("oldAyte:       "); Serial.println(oldAyte); 
      Serial.print("displayedAyte: "); Serial.println(displayedAyte);
      recentAction = true;
      missedUpdateCheck = millis();
      delayA = 10000;
      action = millis();
    } 
    digitalWrite(6, led6State); led6State = true ? !led6State : false;
    httpRequest("0"); 
    if (timeStamp == ""){ Serial.println("____no TimeStamp___");
      httpRequest("0"); //delay(50); 
    }
    httpRequest(timeStamp); //delay(50);
    oldAyte = displayedAyte; displayedAyte = ""; 
  }

  parseAyte("live"); 
   
  if (millis() - action > 5000 && galCheck) { Serial.println("Most Recent Gallery Post"); 
    httpRequest("gal"); 
    recentAction = true;
    long int galDelay = millis();
    while(millis() - galDelay < 500){ //50 didn't work...check with cnt count.
      parseAyte("gal");
    }
    if (oldGal != galAyte){ pixels.show(); }
    Serial.print("galAyte:       "); Serial.println(galAyte); 
    Serial.print("oldGal:       "); Serial.println(oldGal); 
    oldGal = galAyte; galAyte = "";
    galCheck = false;
  }

  if (millis() - action > delayA) { Serial.println("10 Seconds of no action"); 
    action = millis();
    galCheck = true;
  }

  if (millis() - missedUpdateCheck > 2500) { Serial.println("Live History Check"); 
    httpRequest("history"); 
    parseAyte("live");
    missedUpdateCheck = millis();
  }
  
}
// ------------------------------------------------------------------------------------------------------------
// 
// delay's to remove and test...1(inside new timestamp grab)...2()...3()...
//
//

void parseAyte(String ayte){ char c = 0; 
  if (client.available()) {
    c = client.read();
    if (c == '{') { startJson = true; }
    if (startJson == true) {
      text += c;  
      int x = c-65; //65-to-90 is the upperCase Alphabit ASCII
      if (c+0 >= 64 && c+0 <= 90){ 
        if(ayte == "live"){ displayedAyte += c; }
        if(ayte == "gal"){ galAyte += c; }
        if (recentAction){ pixels.setPixelColor(cnt, pixels.Color(r[x],g[x],b[x])); }
        cnt+=1;
        if (cnt >= 64){
          client.flush();
          if (recentAction){ 
            if (ayte != "gal") { pixels.show(); }
            recentAction = false; blank = false; 
          }
          text = ""; startJson = false; checking = false; cnt=0;
        }
      }   
    }
  }
}  

void httpRequest(String timeState) { // this method makes a HTTP connection to the server:
  checking = true;
  client.stop(); 
  if (client.connect(server, 80)) {
    if(timeState == "gal"){ //Serial.println("gallery request");
      client.println("GET /history/sub-c-f0907bae-1ab6-11e6-9f24-02ee2ddab7fe/gallery1/0/1 HTTP/1.1"); }
    else if (timeState == "history"){ //Serial.println("Live History request");
      client.println("GET /history/sub-c-b3fbc6fa-0bf5-11e6-a8fd-02ee2ddab7fe/1b/0/1 HTTP/1.1"); }
    else { //Serial.println("else 0 or timeStamp request"); 
      client.println("GET /subscribe/sub-c-b3fbc6fa-0bf5-11e6-a8fd-02ee2ddab7fe/1b/0/" + timeState + " HTTP/1.1"); }
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
    for (int j = 0; j < 1000; j++){//this isn't tieing up because you put a break in....
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
    missedUpdateCheck = millis();
    checking = false;      
  }
}

void printWifiStatus() { Serial.print("SSID: ");Serial.println(WiFi.SSID());IPAddress ip = WiFi.localIP();
Serial.print("IP Address: "); Serial.println(ip);long rssi = WiFi.RSSI();
Serial.print("signal strength (RSSI):");Serial.print(rssi); Serial.println(" dBm");}

void solid(int r, int g, int b){
  for (int i = 0; i < 64; i++){
    pixels.setPixelColor(i, pixels.Color(r,g,b)); 
  }
  pixels.show();
}

//---------------------------------------------------------------------------------------------------------//
//----------          ANIMATIONS                                                                 ----------//
//---------------------------------------------------------------------------------------------------------//
void rainbowWipe(int d){
  for (int i = 0; i < 77; i++){
      pixels.setPixelColor(i, pixels.Color(100,100,100)); 
      pixels.setPixelColor(i-1, pixels.Color(64,0,0)); pixels.setPixelColor(i-2, pixels.Color(45,15,0)); 
      pixels.setPixelColor(i-3, pixels.Color(28,28,0)); pixels.setPixelColor(i-4, pixels.Color(14,38,0));
      pixels.setPixelColor(i-5, pixels.Color(0,48,0)); pixels.setPixelColor(i-6, pixels.Color(0,32,12));   
      pixels.setPixelColor(i-7, pixels.Color(0,20,20)); pixels.setPixelColor(i-8, pixels.Color(0,9,27));
      pixels.setPixelColor(i-9, pixels.Color(0,0,32)); pixels.setPixelColor(i-10, pixels.Color(7,0,21));
      pixels.setPixelColor(i-11, pixels.Color(12,0,12)); pixels.setPixelColor(i-12, pixels.Color(15,0,5));
      pixels.setPixelColor(i-13, pixels.Color(0,0,0)); 
      pixels.show();
      delay(d);
  }    
}

void randomPixelDanceHalfCourt(int d){ //Serial.println("Playing: Random Pixel Dance Half Court");
  for (int i = 0; i < 3; i++){
    int x = random(0,63); 
    int r = random(0,90); 
    int g = random(0,90); 
    int b = random(0,90);
    pixels.setPixelColor(x, pixels.Color(r,g,b)); pixels.show();
    x = random(0,63); pixels.setPixelColor(x, pixels.Color(0,0,0)); pixels.show();
  }
  delay(d);
}

//void rainbowWormHole(){
//  Serial.println("Playing: Rainbow Wormhole");
//  
//  void loop() { 
//    r+=4*rd; g+=3*gd; b+=2*bd; rg+=1*rgd;
//    
//    if(r>=80){rd=rd*-1;}
//    if(g>=80){gd=gd*-1;}
//    if(b>=80){bd=bd*-1;}
//    if(rg>=80){rgd=rgd*-1;}
//  
//    if(r<=0){rd=rd*-1;}
//    if(g<=0){gd=gd*-1; g=0;}
//    if(b<=0){bd=bd*-1;}
//    if(rg<=0){rgd=rgd*-1;}
//    
//    
//    for (int i = 0; i < 64; i++){
//      if (valIn(level1, i, sizeof(level1)/sizeof(int))){ 
//        pixels.setPixelColor(i, pixels.Color(r,0,0));
//      }
//    }
//    for (int i = 0; i < 64; i++){
//      if (valIn(level2, i, sizeof(level2)/sizeof(int))){ 
//        pixels.setPixelColor(i, pixels.Color(0,g,0));
//      }
//    }
//    for (int i = 0; i < 64; i++){
//      if (valIn(level3, i, sizeof(level3)/sizeof(int))){ 
//        pixels.setPixelColor(i, pixels.Color(0,0,b));
//      }
//    }
//    for (int i = 0; i < 64; i++){
//      if (valIn(level4, i, sizeof(level4)/sizeof(int))){ 
//        pixels.setPixelColor(i, pixels.Color(rg,rg,0));
//      }
//    }
//    pixels.show();
//    delay(30);
//  }
//  
//}
//
//boolean valIn(int arr[], int x, int rng){
//  for (int i = 0; i < rng; i++){if (arr[i] == x){return true;}}return false;}

void bluePixelWondersThroughHell(){
  Serial.println("Playing: Blue Soul Wonders Through Hell");
}
