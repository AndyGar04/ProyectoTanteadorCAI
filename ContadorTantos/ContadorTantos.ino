// Este programa controla un sistema de conteo de puntos utilizando displays NeoPixel.
// Incluye tres displays: A, B y C para mostrar los puntos de los equipos locales, visitantes y el periodo del juego.
// El código utiliza la librería Adafruit_NeoPixel y Noiasca_NeopixelDisplay.

#include <Adafruit_NeoPixel.h>  // Librería para controlar NeoPixels
#include <Noiasca_NeopixelDisplay.h> // Librería para manejar displays NeoPixel

// Configuración de pines y parámetros
const byte ledPin = 6;                // Pin conectado al NeoPixel
const byte numDigits = 2;             // Cantidad de dígitos en los displays A y B
const byte numDigitsB = 1;            // Cantidad de dígitos en el display C
const byte pixelPerDigit = 63;        // Cantidad de píxeles por dígito
const byte addPixels = 63;            // Píxeles adicionales

// Posiciones de inicio para los displays
const byte startPixelA = 0;           // Inicio del display A
const byte startPixelB = 126;         // Inicio del display B
const byte startPixelC = 252;         // Inicio del display C

// Variables para almacenar los puntos y periodo
int PuntosCai = 0; // Puntos del equipo local
int PuntosVis = 0; // Puntos del equipo visitante
int Per = 0;       // Periodo actual

// Pines para botones de entrada
int SumVis = 7;    // Botón para sumar puntos al visitante
int ResVis = 8;    // Botón para restar puntos al visitante
int SumLoc = 9;    // Botón para sumar puntos al local
int ResLoc = 10;   // Botón para restar puntos al local
int BPeriodo = 11; // Botón para avanzar de periodo

// Cantidad total de LEDs
const uint64_t ledCount(pixelPerDigit * numDigits * 2 + addPixels);

// Segmentos de los displays definidos como bits
const uint64_t segment[7] {
  0b000000000000000000000000000000000000000000000000000000111111111,  // Segmento A
  0b000000000000000000000000000000000000000000000111111111000000000,  // Segmento B
  0b000000000000000000000000000000000000111111111000000000000000000,  // Segmento C
  0b000000000000000000000000000111111111000000000000000000000000000,  // Segmento D
  0b000000000000000000111111111000000000000000000000000000000000000,  // Segmento E
  0b000000000111111111000000000000000000000000000000000000000000000,  // Segmento F
  0b111111111000000000000000000000000000000000000000000000000000000   // Segmento G
};

// Inicialización de objetos NeoPixel y displays
Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRB + NEO_KHZ800);
Noiasca_NeopixelDisplay displayA(strip, segment, numDigits, pixelPerDigit, startPixelA);
Noiasca_NeopixelDisplay displayB(strip, segment, numDigits, pixelPerDigit, startPixelB);
Noiasca_NeopixelDisplay displayC(strip, segment, numDigitsB, pixelPerDigit, startPixelC);

void setup() {
  Serial.begin(9600);
  strip.begin();                       // Inicializa el objeto NeoPixel
  strip.show();                        // Apaga todos los LEDs
  strip.setBrightness(125);            // Establece el brillo
  strip.clear();

  // Configura los colores de los displays
  displayA.setColorFont(0xAA0000);     // Rojo para el equipo local
  displayB.setColorFont(0x00AA00);     // Verde para el equipo visitante
  displayC.setColorFont(0xAAAAAA);     // Blanco para el periodo

  // Configuración de los pines de entrada
  pinMode(SumVis, INPUT);
  pinMode(SumLoc, INPUT);
  pinMode(ResVis, INPUT);
  pinMode(ResLoc, INPUT);
  pinMode(BPeriodo, INPUT);
}

void loop() {
  Limitaciones();  // Verifica y ajusta límites
  ColorPer();      // Muestra el periodo en el display C
  Periodo();       // Controla el cambio de periodo
}

// Verifica los límites de los puntos y controla las funciones principales
void Limitaciones() {
  if (Per == 5) {
    Per = 0;
  }

  if (PuntosCai > 99) {
    PuntosCai = 99;
  } else {
    sumaLoc();
  }

  if (PuntosCai <= 0) {
    PuntosCai = 0;
  } else {
    restaLoc();
  }

  if (varv > 9 && PuntosVis != 9 && digitalRead(SumVis) == HIGH) {
    varv = 9;
    PuntosVis = 9;
  } else {
    funcionMostrarVis();
  }

  if (varv == 0 && PuntosVis == 0 && digitalRead(ResVis) == HIGH) {
    varv = 0;
    PuntosVis = 0;
  } else {
    restavis();
  }
}

// Incrementa los puntos del equipo local
void sumaLoc() {
  if (PuntosCai < 10) {
    displayA.print(Var);
  }
  displayA.print(PuntosCai);

  if (digitalRead(SumLoc) == HIGH) {
    PuntosCai++;
    displayA.clear();
    delay(250);
  }
}

// Decrementa los puntos del equipo local
void restaLoc() {
  if (digitalRead(ResLoc) == HIGH) {
    displayA.clear();
    PuntosCai--;
    delay(250);
  }
}

// Controla el incremento y visualización de puntos del visitante
void funcionMostrarVis() {
  displayB.print(varv);
  displayB.print(PuntosVis);

  if (digitalRead(SumVis) == HIGH) {
    delay(250);
    PuntosVis++;
    displayB.clear();
  }

  if (PuntosVis >= 10) {
    varv++;
    PuntosVis = 0;
  }
}

// Decrementa los puntos del equipo visitante
void restavis() {
  if (digitalRead(ResVis) == HIGH) {
    PuntosVis--;
    delay(250);
    displayB.clear();
  }

  if (PuntosVis < 0) {
    PuntosVis = 9;
    varv--;
  }
}

// Reinicia los puntos y variables a cero
void Reset() {
  if (digitalRead(ResVis) == HIGH) {
    PuntosCai = 0;
    varv = 0;
    PuntosVis = 0;
  }
}

// Controla el cambio de periodo
void Periodo() {
  if (digitalRead(BPeriodo) == HIGH) {
    Per++;
    delay(500);
  }
}

// Muestra el periodo en el display C
void ColorPer() {
  displayC.print(Per);
}
