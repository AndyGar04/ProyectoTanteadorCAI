const byte ledPin = 6;          // Which pin on the Arduino is connected to the NeoPixels?
const byte numDigits = 2;       // How many digits (numbers) are available on each display
const byte pixelPerDigit = 63;  // all pixels, including decimal point pixels if available at each digit
const byte addPixels = 0;       // unregular additional pixels to be added to the strip
const byte startPixelMM = 0;    // start pixel of display A
const byte startPixelSS = 126;  // start pixel of display B (assumption: 2 x 14 used by displayMM + 4 additional Pixels)
const byte identificacion[6] = "00001";

const int varv = 0;

int Minutos = 00;
int Segundos = 00;
int bandera = 0;
int confi = 0;

int boton_pausar = 7;
int boton_iniciar = 8;
int boton_sumar = 9;
int boton_restar = 10;
int boton_reset = 11;

const uint64_t ledCount(pixelPerDigit* numDigits * 2 + addPixels);
/*
   Segments are named and orded like this

          SEG_A
   SEG_F         SEG_B
          SEG_G
   SEG_E         SEG_C
          SEG_D          SEG_DP

  in the following constant array you have to define
  which pixels belong to which segment
*/

typedef uint64_t segsize_t;  // fit variable size to your needed pixels. uint16_t --> max 16 Pixel per digit
const segsize_t segment[7]{
  0b000000000000000000000000000000000000000000000000000000111111111,  // SEG_A
  0b000000000000000000000000000000000000000000000111111111000000000,  // SEG_B
  0b000000000000000000000000000000000000111111111000000000000000000,  // SEG_C
  0b000000000000000000000000000111111111000000000000000000000000000,  // SEG_D
  0b000000000000000000111111111000000000000000000000000000000000000,  // SEG_E
  0b000000000111111111000000000000000000000000000000000000000000000,  // SEG_F
  0b111111111000000000000000000000000000000000000000000000000000000,  // SEG_G
};

#include <Adafruit_NeoPixel.h>                                    // install Adafruit library from library manager
Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRB + NEO_KHZ800);  // create neopixel object like you commonly used with Adafruit

#include <Noiasca_NeopixelDisplay.h>                                                   // download library from: http://werner.rothschopf.net/202005_arduino_neopixel_display.htm
Noiasca_NeopixelDisplay display(strip, segment, numDigits, pixelPerDigit, addPixels);  // create display object, handover the name of your strip as first parameter!

Noiasca_NeopixelDisplay displayMM(strip, segment, numDigits, pixelPerDigit, startPixelMM);
Noiasca_NeopixelDisplay displaySS(strip, segment, numDigits, pixelPerDigit, startPixelSS);

void setup() {
  Serial.begin(115200);

  strip.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();             // Turn OFF all pixels ASAP
  strip.setBrightness(50);  // Set BRIGHTNESS to about 1/5 (max = 255)
  strip.clear();
  
  pinMode(boton_pausar, INPUT);
  pinMode(boton_iniciar, INPUT);
  pinMode(boton_sumar, INPUT);
  pinMode(boton_restar, INPUT);
  pinMode(boton_reset, INPUT);
}

void loop() {
 
  Sub12_1(); 
  Sub12_2(); 
  Primera_1(); 
  
  if (Minutos < 10) {
    displayMM.print(varv);
    displayMM.print(Minutos);
  } else {
    displayMM.print(Minutos);
  }
 
  if (Segundos < 10) {
    displaySS.print(varv);
    displaySS.print(Segundos);
  } else {
    displaySS.print(Segundos);
  }

  if (bandera == 1) {
    if (digitalRead(boton_pausar) == HIGH) {
      bandera = 0;
      displayMM.setColorFont(0xAA0000);
      displaySS.setColorFont(0xAA0000);
    }
    Contador();
    delay(950);
    
    displayMM.clear();
    displaySS.clear();
  }
  if (bandera == 0) {
    if (digitalRead(boton_iniciar) == HIGH) {
      bandera = 1;
      displayMM.setColorFont(0x00FFFF); // Establecer color en celeste
      displaySS.setColorFont(0x00FFFF); // Establecer color en celeste
    }
    delay(950);
  }


  if (digitalRead(boton_sumar) == HIGH) {
    Minutos++;}
  if (digitalRead(boton_restar) == HIGH and not Minutos==0) {
    Minutos--;} 
  if (digitalRead(boton_reset) == HIGH) {
    Minutos = 0; 
    Segundos = 0; 
    bandera = 0;
    confi=0;}
}

void Sub12_1(){
  
  if(Minutos==2 and Segundos==0 and confi==0 and digitalRead(boton_iniciar) == HIGH){
    Minutos=10;
    confi=12;}
    
  if (Minutos==0 && Segundos==0 && confi==12){
    Minutos=Minutos+10;
    bandera = 0;
  }
}

void Sub12_2(){
  
  if(Minutos==3 and Segundos==0 and confi==0 and digitalRead(boton_iniciar) == HIGH){
    Minutos=25;
    confi=25;}
    
  if (Minutos==0 && Segundos==0 && confi==25){
    Minutos=Minutos+25;
    bandera = 0;
  }
}

void Primera_1(){
  
  if(Minutos==1 and Segundos==0 and confi==0 and digitalRead(boton_iniciar) == HIGH){
    Minutos=15;
    confi=15;}
    
  if (Minutos==0 && Segundos==0 && confi==15){
    Minutos=Minutos+15;
    bandera = 0;
  }
}

void Contador() {
  Segundos--;

  if (Segundos < 0) {
    Minutos--;
    Segundos += 60;
  }
}
