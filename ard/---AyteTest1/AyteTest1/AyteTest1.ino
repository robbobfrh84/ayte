#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            12
#define NUMPIXELS      64
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
    
const int led13 = 13;
int r = 0; int g = 0; int b = 0;
int p = 28;
int soleDelay = 1;
int topBorder[] = {0,1,2,3,4,5,6,7};
int rightBorder[] = {7,17,23,31,39,47,55,63};
int bottomBorder[] = {56,57,58,59,60,61,62,63};
int leftBorder[] = {0,8,16,24,32,40,48,56};
int hell[] = {0,0,0,0,0,0,0,0,
              0,0,0,0,0,0,0,0,
              0,0,0,0,0,0,0,0,
              0,0,0,0,0,0,0,0,
              0,0,0,0,0,0,0,0,
              0,0,0,0,0,0,0,0,
              0,0,0,0,0,0,0,0,
              0,0,0,0,0,0,0,0};
              


void setup() {
  Serial.begin(115200);
  pinMode(led13,OUTPUT);
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  for (int i = 0; i < NUMPIXELS; i++){
    hell[i] = random(0,90);
  }
}

void solid(int r, int g, int b) {
  for (int i = 0; i < NUMPIXELS; i++){
     if (p != i){
       pixels.setPixelColor(i, pixels.Color(r,g,b));
     }
  }
  pixels.show();
}

boolean valIn(int arr[], int x, int rng){
  for (int i = 0; i < rng; i++){
    if (arr[i] == x){
      Serial.println(arr[i]);
      return true;
    }
  }
  return false;
}

void loop() { 
  
  //create hell effect
  for (int i = 0; i < NUMPIXELS; i++){
    hell[i] += random(0,5); 
    hell[i] -= random(0,5); 
    if (hell[i] > 100){ hell[i] = 90; hell[i] -= random(0,10); }
    if (hell[i] < 5){ hell[i] = 5; hell[i] += random(0,10); }
    if (p != i) {pixels.setPixelColor(i, pixels.Color(hell[i],g,b)); }
  }
  delay(20);
  soleDelay-=20;
  if (soleDelay <=0 ){
    //wonder blue pixel
    int m = random(1,5);
    if (valIn(topBorder, p, sizeof(topBorder)/sizeof(int))){ while (m == 4){ m = random(0,5);}}
    if (valIn(rightBorder, p, sizeof(rightBorder)/sizeof(int))){ while (m == 1){ m = random(0,5);}}
    if (valIn(bottomBorder, p, sizeof(bottomBorder)/sizeof(int))){ while (m == 3){ m = random(0,5);}}
    if (valIn(leftBorder, p, sizeof(leftBorder)/sizeof(int))){ while (m == 2){ m = random(0,5);}}
    int preP = p;
    if (m==0){p=p;} if (m==1){p+=1;} if (m==2){p-=1;} if (m==3){p+=8;} if (m==4){p-=8;}
    pixels.setPixelColor(p, pixels.Color(r,g,90));
    soleDelay = random(100,400);
  }
  pixels.show();
}






