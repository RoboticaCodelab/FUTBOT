/*
  ============================================================================
  ROBÓTICA CODELAB SAS. 
  
  Proyecto: Codigo de prueba para pantalla OLED SSD1306
  Autor: Javier Ronaldo Gil Ayala - Juan Sebastian Carreño Patiño
  Fecha de desarrollo: Mayo de 2025

  Descripción:
  Este código fue desarrollado como parte de un proyecto educativo para probar y visualizar flechas animadas y gruesas en una pantalla OLED SSD1306 utilizando un ESP32. 
  El objetivo es mostrar diferentes direcciones (arriba, derecha, abajo, izquierda) mediante gráficos claros y estéticos, facilitando el desarrollo de interfaces visuales para proyectos de robótica o control de vehículos.

  Uso:
  - Conecta la pantalla SSD1306 al ESP32 mediante I2C.
  - Carga este código.
  - Observa la secuencia animada de flechas en la pantalla, útil para indicar direcciones de movimiento en futuros proyectos.

  Este proyecto sirve como base visual para sistemas de control y monitoreo en tiempo real.

  www.roboticacodelab.com - Pagina principal 
  www.roboticacodelab.online - página educativa 
  www.instagram.com/robotica_codelab - Instagram
  ============================================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Flecha hacia arriba
void drawArrowUp(int offsetY) {
  display.clearDisplay();
  // Punta de la flecha (más ancha)
  display.fillTriangle(54, 22 + offsetY, 74, 22 + offsetY, 64, 4 + offsetY, SSD1306_WHITE);
  // Cuerpo de la flecha (más grueso)
  display.fillRect(58, 22 + offsetY, 12, 32, SSD1306_WHITE);
  display.display();
}

// Flecha hacia la derecha
void drawArrowRight(int offsetX) {
  display.clearDisplay();
  // Punta de la flecha (más ancha)
  display.fillTriangle(108 + offsetX, 20, 108 + offsetX, 44, 124 + offsetX, 32, SSD1306_WHITE);
  // Cuerpo de la flecha (más grueso)
  display.fillRect(60 + offsetX, 26, 48, 12, SSD1306_WHITE);
  display.display();
}

// Flecha hacia abajo
void drawArrowDown(int offsetY) {
  display.clearDisplay();
  // Punta de la flecha (más ancha)
  display.fillTriangle(54, 42 + offsetY, 74, 42 + offsetY, 64, 60 + offsetY, SSD1306_WHITE);
  // Cuerpo de la flecha (más grueso)
  display.fillRect(58, 10 + offsetY, 12, 32, SSD1306_WHITE);
  display.display();
}

// Flecha hacia la izquierda
void drawArrowLeft(int offsetX) {
  display.clearDisplay();
  // Punta de la flecha (más ancha)
  display.fillTriangle(20 + offsetX, 20, 20 + offsetX, 44, 4 + offsetX, 32, SSD1306_WHITE);
  // Cuerpo de la flecha (más grueso)
  display.fillRect(20 + offsetX, 26, 48, 12, SSD1306_WHITE);
  display.display();
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop() {
  // Flecha arriba animada
  for (int i = 0; i < 3; i++) {
    drawArrowUp(i*2);
    delay(120);
    drawArrowUp(0);
    delay(120);
  }
  // Flecha derecha animada
  for (int i = 0; i < 3; i++) {
    drawArrowRight(i*2);
    delay(120);
    drawArrowRight(0);
    delay(120);
  }
  // Flecha abajo animada (dos veces)
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < 3; i++) {
      drawArrowDown(i*2);
      delay(120);
      drawArrowDown(0);
      delay(120);
    }
  }
  // Flecha izquierda animada
  for (int i = 0; i < 3; i++) {
    drawArrowLeft(i*2);
    delay(120);
    drawArrowLeft(0);
    delay(120);
  }
}
