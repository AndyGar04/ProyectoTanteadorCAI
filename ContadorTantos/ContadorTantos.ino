const byte ledPin = 6;                // Which pin on the Arduino is connected to the NeoPixels?
const byte numDigits = 2; 
const byte numDigitsB = 1;  // How many digits (numbers) are available on each display
const byte pixelPerDigit = 63;         // all pixels, including decimal point pixels if available at each digit
const byte addPixels = 63;              // unregular additional pixels to be added to the strip

const byte startPixelA = 0;            // start pixel of display A
const byte startPixelB = 126;
const byte startPixelC = 252;          // start pixel of display B (assumption: 2 x 14 used by displayA + 4 additional Pixels)

int Var=0;
int varv=0;

int PuntosCai=0; //Puntos Cai
int PuntosVis=0; //Puntos Vis
int Per=0;       //Suma/Res periodo

int SumVis=7;
int ResVis=8;
int SumLoc=9;
int ResLoc=10;
int BPeriodo=11;

const uint64_t ledCount(pixelPerDigit * numDigits * 2 + addPixels);
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

typedef uint64_t segsize_t;                               // fit variable size to your needed pixels. uint16_t --> max 16 Pixel per digit
const segsize_t segment[7] {
  0b000000000000000000000000000000000000000000000000000000111111111,  // SEG_A
  0b000000000000000000000000000000000000000000000111111111000000000,  // SEG_B
  0b000000000000000000000000000000000000111111111000000000000000000,  // SEG_C
  0b000000000000000000000000000111111111000000000000000000000000000,  // SEG_D
  0b000000000000000000111111111000000000000000000000000000000000000,  // SEG_E
  0b000000000111111111000000000000000000000000000000000000000000000,  // SEG_F
  0b111111111000000000000000000000000000000000000000000000000000000,  // SEG_G
};

#include <Adafruit_NeoPixel.h>                                       // install Adafruit library from library manager
Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRB + NEO_KHZ800);     // create neopixel object like you commonly used with Adafruit

#include <Noiasca_NeopixelDisplay.h>                                      // download library from: http://werner.rothschopf.net/202005_arduino_neopixel_display.htm
// in this sketch we handle displayA and displayB as two individual displays:
Noiasca_NeopixelDisplay displayA(strip, segment, numDigits, pixelPerDigit, startPixelA);  // create display object, handover the name of your strip as first parameter!
Noiasca_NeopixelDisplay displayB(strip, segment, numDigits, pixelPerDigit, startPixelB);  // create display object, handover the name of your strip as first parameter!
Noiasca_NeopixelDisplay displayC(strip, segment, numDigits-1, pixelPerDigit, startPixelC);

void setup() {
  Serial.begin(9600);
  strip.begin();                       // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();                        // Turn OFF all pixels ASAP
  strip.setBrightness(125);             // Set BRIGHTNESS to about 1/5 (max = 255)
  strip.clear();

  displayA.setColorFont(0xAA0000);     // each display gets its own color, e.g. corresponding to the button color
  displayB.setColorFont(0x00AA00);  
  displayC.setColorFont(0xAAAAAA);
    
  pinMode(SumVis,INPUT);
  pinMode(SumLoc,INPUT);
  pinMode(ResVis,INPUT);
  pinMode(ResLoc,INPUT);
  pinMode(BPeriodo,INPUT);
}

void loop() {
  
 Limitaciones();
 ColorPer();
 Periodo();}


void Limitaciones(){
if (Per==5){
  Per=0;}
  
if (PuntosCai>99){
     PuntosCai=99;
}else{ 
      sumaLoc();}
  
if (PuntosCai<=0){
   PuntosCai=0;
   }else{
     restaLoc();}
 
if (varv>9 and PuntosVis!=9 and (digitalRead(SumVis)==HIGH)){
   varv=9;
   PuntosVis=9;
   }else {
   funcionMostrarVis();} 
 
if (varv==0 and PuntosVis==0 and (digitalRead(ResVis)==HIGH)){
    varv=0;
    PuntosVis=0;
    }else{
      restavis();}
}

void sumaLoc(){
if(PuntosCai<10){
  displayA.print(Var);}
  displayA.print(PuntosCai);
if (digitalRead(SumLoc)==HIGH){
  PuntosCai=PuntosCai+1;
  displayA.clear();
  delay(250);}
}
 
 void restaLoc(){
  if (digitalRead(ResLoc)==HIGH){
  displayA.clear();
  PuntosCai=PuntosCai-1;
  delay(250);}
}

void funcionMostrarVis(){
   displayB.print(varv);
   displayB.print(PuntosVis);
   
 if (digitalRead(SumVis)==HIGH){
    delay(250);
    PuntosVis=PuntosVis+1;
    displayB.clear();}
    
 if (PuntosVis>=10){
    varv=varv+1;
    PuntosVis=0;}
}

void restavis(){
   if (digitalRead(ResVis)==HIGH){
     PuntosVis=PuntosVis-1;
     delay(250);     
     displayB.clear();}
    if (PuntosVis<0){
      PuntosVis=9;
      varv=varv-1;}}

void Reset(){
  if (digitalRead(ResVis)==HIGH){
     PuntosCai=PuntosCai-PuntosCai;
     varv=varv-varv;
     PuntosVis=PuntosVis-PuntosVis;}}

void Periodo(){
  if (digitalRead(BPeriodo)==HIGH){
    Per=Per+1;
    delay(500);}}


void ColorPer(){
  displayC.print(Per);
}
