# Documentación del Código: Cronómetro para Tablero con Arduino y NeoPixel

Este documento describe el funcionamiento y la estructura del código para un cronómetro con NeoPixels controlado por un microcontrolador Arduino. El cronómetro incluye funcionalidades como iniciar, pausar, ajustar minutos y reiniciar.

---

## Introducción
El proyecto implementa un cronómetro visual basado en displays NeoPixel que permite mostrar el tiempo restante en minutos y segundos. Se utiliza la librería Adafruit NeoPixel para gestionar los LEDs y la librería Noiasca Neopixel Display para crear los displays.

### Características principales
- Control del cronómetro mediante botones:
  - **Iniciar/Pausar**.
  - **Sumar/Restar minutos**.
  - **Reiniciar el cronómetro**.
- Visualización clara de los minutos y segundos en displays.
- Configuración para manejar tiempos especiales predeterminados.

---

## Configuración del Hardware

### Pines utilizados
- **Pin 6**: Conectado al NeoPixel.
- **Pines 7-11**: Botones para control del cronómetro.

### Componentes necesarios
- Tira LED NeoPixel.
- Botones de control.
- Fuente de alimentación adecuada para los LEDs.
- Arduino (compatible con las librerías utilizadas).

---

## Descripción del Código

### 1. Inicialización
Se configuran las variables globales, los pines y los objetos necesarios para el control del cronómetro y los displays.

```cpp
const byte ledPin = 6;          // Pin conectado al NeoPixel
const byte numDigits = 2;       // Número de dígitos en cada display
const byte pixelPerDigit = 63;  // Píxeles por dígito
...
Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRB + NEO_KHZ800);
Noiasca_NeopixelDisplay displayMM(strip, segment, numDigits, pixelPerDigit, startPixelMM);
Noiasca_NeopixelDisplay displaySS(strip, segment, numDigits, pixelPerDigit, startPixelSS);
```

Los displays para minutos y segundos se inicializan con los segmentos y los píxeles correspondientes.

---

### 2. Control del Cronómetro
El cronómetro puede:

#### a. **Iniciar y Pausar**
El estado del cronómetro se controla con la variable `bandera`:
- **1**: El cronómetro está en marcha.
- **0**: El cronómetro está pausado.

```cpp
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
```

#### b. **Ajustar Minutos**
Botones dedicados para sumar y restar minutos.
```cpp
if (digitalRead(boton_sumar) == HIGH) {
  Minutos++;
}
if (digitalRead(boton_restar) == HIGH && Minutos > 0) {
  Minutos--;
}
```

#### c. **Reiniciar**
Establece los minutos y segundos a cero y reinicia las configuraciones:
```cpp
if (digitalRead(boton_reset) == HIGH) {
  Minutos = 0;
  Segundos = 0;
  bandera = 0;
  confi = 0;
}
```

---

### 3. Visualización del Tiempo
El tiempo se muestra en los displays NeoPixel para minutos (`displayMM`) y segundos (`displaySS`). Se gestiona la correcta visualización de ceros iniciales.

```cpp
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
```

---

### 4. Configuración de Tiempos Especiales
Se ajustan tiempos predeterminados según condiciones específicas.
```cpp
if (Minutos == 2 && Segundos == 0 && confi == 0 && digitalRead(boton_iniciar) == HIGH) {
  Minutos = 10;
  confi = 12;
}
...
```

---

### 5. Cuenta Atrás
El método `Contador()` decrementa segundos y ajusta minutos cuando es necesario.
```cpp
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
```

---

## Consideraciones Finales
- **Brillo**: El brillo del NeoPixel está configurado al 50%. Puede ajustarse mediante `strip.setBrightness()`.
- **Tiempo Especial**: Los tiempos predeterminados se reinician cuando el cronómetro llega a cero.
- **Colores**: Los colores de los displays indican el estado:
  - **Rojo**: Pausa.
  - **Celeste**: En marcha.

---

## Recursos Adicionales
- [Librería Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
- [Librería Noiasca Neopixel Display](http://werner.rothschopf.net/202005_arduino_neopixel_display.htm)

