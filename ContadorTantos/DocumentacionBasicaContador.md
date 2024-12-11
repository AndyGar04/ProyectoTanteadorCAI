# Funcionamiento Básico del Código para el Sistema de Conteo de Puntos

Este documento explica el funcionamiento básico del código diseñado para un sistema de conteo de puntos utilizando displays NeoPixel y botones de control. Este sistema se compone de tres displays:

- **Display A**: Muestra los puntos del equipo local.
- **Display B**: Muestra los puntos del equipo visitante.
- **Display C**: Muestra el periodo actual del juego.

El código está implementado utilizando las librerías **Adafruit_NeoPixel** y **Noiasca_NeopixelDisplay** para controlar los LEDs NeoPixel y manejar los displays.

## Configuración Inicial

1. **Pines y Parámetros**:
   - El pin 6 del Arduino está conectado a los NeoPixels.
   - Cada display tiene un número definido de dígitos:
     - Display A y B tienen 2 dígitos.
     - Display C tiene 1 dígito.
   - Se definen segmentos y posiciones iniciales para cada display.

2. **Inicialización de Objetos**:
   - Se crean objetos `Adafruit_NeoPixel` y `Noiasca_NeopixelDisplay` para controlar los LEDs y displays.
   - Cada display se inicializa con un color diferente:
     - Rojo para el equipo local.
     - Verde para el equipo visitante.
     - Blanco para el periodo.

## Funcionalidades Principales

### 1. **Control de Puntos del Equipo Local**
- **Incremento**:
  - El botón conectado al pin 9 permite incrementar los puntos del equipo local.
  - El límite superior es 99 puntos.
- **Decremento**:
  - El botón conectado al pin 10 permite reducir los puntos.
  - El límite inferior es 0 puntos.
- Los cambios se reflejan en el **Display A**.

### 2. **Control de Puntos del Equipo Visitante**
- **Incremento**:
  - El botón conectado al pin 7 permite incrementar los puntos.
  - Cada 10 puntos, los dígitos se reinician y se incrementa una variable auxiliar (`varv`) para contabilizar los "decenas".
  - El límite superior es 99 puntos.
- **Decremento**:
  - El botón conectado al pin 8 permite reducir los puntos.
  - Si los puntos llegan a un valor negativo, se ajustan a 9 puntos y se decrementa la variable auxiliar (`varv`).
- Los cambios se reflejan en el **Display B**.

### 3. **Control del Periodo**
- El botón conectado al pin 11 permite incrementar el periodo.
- Si el periodo alcanza el valor 5, se reinicia a 0.
- El periodo se muestra en el **Display C**.

### 4. **Reinicio General**
- El sistema puede reiniciar todos los valores (puntos locales, puntos visitantes y periodo) a cero.
- Esta funcionalidad se activa al presionar el botón de restablecimiento de puntos visitantes (pin 8).

## Lógica del Sistema

1. **Limitaciones**:
   - Verifica que los puntos y el periodo se mantengan dentro de los rangos permitidos.
   - Gestiona las acciones de los botones de manera ordenada para evitar inconsistencias.

2. **Visualización**:
   - Cada cambio en los valores se actualiza y muestra en el display correspondiente.
   - Los colores ayudan a diferenciar los equipos y el periodo.

3. **Retrasos**:
   - Se utiliza `delay(250)` para evitar lecturas rápidas y erróneas de los botones.

## Requisitos del Sistema

1. **Hardware**:
   - Arduino con soporte para NeoPixels.
   - Tira de LEDs NeoPixel compatible.
   - Botones pulsadores conectados a los pines configurados.

2. **Software**:
   - Librería **Adafruit_NeoPixel** instalada desde el gestor de librerías de Arduino.
   - Librería **Noiasca_NeopixelDisplay** descargada del sitio oficial.

Con esta documentación, se facilita el entendimiento del sistema y su funcionalidad básica, permitiendo a futuros desarrolladores extender y adaptar el código según sea necesario.

