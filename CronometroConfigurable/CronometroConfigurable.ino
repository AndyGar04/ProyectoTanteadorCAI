// Programa para controlar un cronómetro utilizando displays NeoPixel.
// Este cronómetro incluye opciones para iniciar, pausar, sumar, restar y reiniciar el tiempo.

#include <Adafruit_NeoPixel.h> // Librería para manejar los LEDs NeoPixel
#include <Noiasca_NeopixelDisplay.h> // Librería para manejar displays NeoPixel

// Configuración de pines y parámetros
const byte ledPin = 6;          // Pin conectado al NeoPixel
const byte numDigits = 2;       // Número de dígitos en cada display
const byte pixelPerDigit = 63;  // Número de píxeles por dígito
const byte addPixels = 0;       // Píxeles adicionales
const byte startPixelMM = 0;    // Inicio del display de minutos
const byte startPixelSS = 126;  // Inicio del display de segundos

int Minutos = 0;
int Segundos = 0;
int bandera = 0; // Indica si el cronómetro está en pausa o en marcha
int confi = 0;   // Bandera de configuración para ajustes de tiempo

// Pines de los botones
const int boton_pausar = 7;
const int boton_iniciar = 8;
const int boton_sumar = 9;
const int boton_restar = 10;
const int boton_reset = 11;

const uint64_t ledCount(pixelPerDigit * numDigits * 2 + addPixels);

// Segmentos de los displays definidos como bits
const uint64_t segment[7] = {
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
Noiasca_NeopixelDisplay display(strip, segment, numDigits, pixelPerDigit, addPixels);
Noiasca_NeopixelDisplay displayMM(strip, segment, numDigits, pixelPerDigit, startPixelMM);
Noiasca_NeopixelDisplay displaySS(strip, segment, numDigits, pixelPerDigit, startPixelSS);

void setup() {
  Serial.begin(115200);

  strip.begin();            // Inicializa el objeto NeoPixel
  strip.show();             // Apaga todos los LEDs
  strip.setBrightness(50);  // Establece el brillo
  strip.clear();

  pinMode(boton_pausar, INPUT);
  pinMode(boton_iniciar, INPUT);
  pinMode(boton_sumar, INPUT);
  pinMode(boton_restar, INPUT);
  pinMode(boton_reset, INPUT);
}

void loop() {
  gestionarTiempoEspecial();

  mostrarTiempo();

  if (bandera == 1) {
    if (digitalRead(boton_pausar) == HIGH) {
      pausarCronometro();
    }
    Contador();
    delay(950);
  } else {
    if (digitalRead(boton_iniciar) == HIGH) {
      iniciarCronometro();
    }
    delay(950);
  }

  ajustarMinutos();
  reiniciarCronometro();
}

// Muestra el tiempo en los displays
void mostrarTiempo() {
  if (Minutos < 10) {
    displayMM.print(0);
    displayMM.print(Minutos);
  } else {
    displayMM.print(Minutos);
  }

  if (Segundos < 10) {
    displaySS.print(0);
    displaySS.print(Segundos);
  } else {
    displaySS.print(Segundos);
  }
}

// Incrementa o decrementa los minutos según los botones
void ajustarMinutos() {
  if (digitalRead(boton_sumar) == HIGH) {
    Minutos++;
  }
  if (digitalRead(boton_restar) == HIGH && Minutos > 0) {
    Minutos--;
  }
}

// Reinicia el cronómetro
void reiniciarCronometro() {
  if (digitalRead(boton_reset) == HIGH) {
    Minutos = 0;
    Segundos = 0;
    bandera = 0;
    confi = 0;
  }
}

// Pausa el cronómetro
void pausarCronometro() {
  bandera = 0;
  displayMM.setColorFont(0xAA0000); // Rojo para indicar pausa
  displaySS.setColorFont(0xAA0000);
}

// Inicia el cronómetro
void iniciarCronometro() {
  bandera = 1;
  displayMM.setColorFont(0x00FFFF); // Celeste para indicar en marcha
  displaySS.setColorFont(0x00FFFF);
}

// Cuenta hacia atrás el tiempo
void Contador() {
  Segundos--;

  if (Segundos < 0) {
    Minutos--;
    Segundos = 59;
  }

  if (Minutos < 0) {
    Minutos = 0;
    Segundos = 0;
    bandera = 0;
  }
}

// Configuración de tiempos especiales
void gestionarTiempoEspecial() {
  if (Minutos == 2 && Segundos == 0 && confi == 0 && digitalRead(boton_iniciar) == HIGH) {
    Minutos = 10;
    confi = 12;
  }

  if (Minutos == 3 && Segundos == 0 && confi == 0 && digitalRead(boton_iniciar) == HIGH) {
    Minutos = 25;
    confi = 25;
  }

  if (Minutos == 1 && Segundos == 0 && confi == 0 && digitalRead(boton_iniciar) == HIGH) {
    Minutos = 15;
    confi = 15;
  }

  if (Minutos == 0 && Segundos == 0) {
    if (confi == 12) {
      Minutos = 10;
    } else if (confi == 25) {
      Minutos = 25;
    } else if (confi == 15) {
      Minutos = 15;
    }
    bandera = 0;
  }
}
