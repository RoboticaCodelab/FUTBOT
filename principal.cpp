/*
  ============================================================================
  ROBÓTICA CODELAB SAS.

  Proyecto: FUTBOT
  Autor: Javier Ronaldo Gil Ayala - Juan Sebastian Carreño Patiño
  Fecha: Mayo de 2025

  Características:
  - Animación de espera mientras no hay conexión Bluetooth
  - Animación de inicio al conectar Bluetooth
  - Compensación de velocidad entre motores
  - PWM nativo en pines de dirección (sin ENABLE)
  - COMANDOS ADICIONALES:
    - 'C1XXX': Establece velocidad para Motor 1 (Izquierdo), XXX = 000-255
    - 'C2XXX': Establece velocidad para Motor 2 (Derecho), XXX = 000-255
    - 'CGXXX': Establece velocidad GENERAL para movimientos U,D,L,R, XXX = 000-255
    - 'F1': Motor Derecho Adelante (usa motor1Speed)
    - 'B1': Motor Derecho Atrás (usa motor1Speed)
    - 'S1': Motor Derecho Detener
    - 'F2': Motor Izquierdo Adelante (usa motor2Speed)
    - 'B2': Motor Izquierdo Atrás (usa motor2Speed)
    - 'S2': Motor Izquierdo Detener
    - 'U', 'D', 'L', 'R': Movimientos generales (usan generalSpeed)
    - 'S': Detener ambos motores
    - 'X': Prueba de Pantalla OLED
  ============================================================================
*/

#include <BluetoothSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
BluetoothSerial SerialBT;

// Configuración de motores (PWM en pines de dirección)

#define M1_IN1_CHANNEL 4  // GPIO4 - Motor Derecho IN1
#define M1_IN2_CHANNEL 2  // GPIO2 - Motor Derecho IN2
#define M2_IN1_CHANNEL 27 // GPIO27 - Motor Izquierdo IN3
#define M2_IN2_CHANNEL 26 // GPIO26 - Motor Izquierdo IN4

// Parámetros PWM
const uint32_t pwmFreq = 30000; 
const uint8_t pwmResolution = 8; // 0-255

// Variables de velocidad
uint8_t motor1Speed = 250;      // Velocidad Motor Derecho (para control individual F1/B1)
uint8_t motor2Speed = 250;      // Velocidad Motor Izquierdo (para control individual F2/B2)
uint8_t generalSpeed = 250;     // Velocidad para movimientos generales (U, D, L, R)
const int8_t compensation = 0; // Compensación para el motor derecho. Si M1 es más lento, usa un valor positivo. Si es más rápido, negativo.

bool connectedBefore = false;
unsigned long lastCommandTime = 0;
const unsigned long commandTimeout = 700; // ms - Detener motores si no hay comandos en este tiempo

void setup() {
    Serial.begin(115200);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("Error inicializando pantalla OLED");
        while(1);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Esperando BT...");
    display.display();

    ledcAttach(M1_IN1_CHANNEL, pwmFreq, pwmResolution);
    ledcAttach(M1_IN2_CHANNEL, pwmFreq, pwmResolution);
    ledcAttach(M2_IN1_CHANNEL, pwmFreq, pwmResolution);
    ledcAttach(M2_IN2_CHANNEL, pwmFreq, pwmResolution);
    

    SerialBT.begin("RoboCodelabCar");
    Serial.println("ESP32 Bluetooth Car Listo. Esperando conexión...");
}

void animacionEsperaBT() {
    static int angulo = 0;
    display.clearDisplay();
    float angle_rad = angulo * PI / 180.0;
    int x = SCREEN_WIDTH / 2 + cos(angle_rad) * (SCREEN_HEIGHT / 2 - 12);
    int y = SCREEN_HEIGHT / 2 + sin(angle_rad) * (SCREEN_HEIGHT / 2 - 12);
    display.fillCircle(x, y, 5, SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(10, SCREEN_HEIGHT - 10);
    display.print("Esperando BT...");
    display.display();
    angulo = (angulo + 12) % 360;
}

void animacionInicio() {
    display.clearDisplay();
    for(int i=0; i<360; i+=25) {
        display.clearDisplay();
        float angle_rad = i * PI / 180.0;
        int x = SCREEN_WIDTH / 2 + cos(angle_rad) * 20;
        int y = SCREEN_HEIGHT / 2 + sin(angle_rad) * 20;
        display.fillCircle(x, y, 3, SSD1306_WHITE);
        display.setTextSize(1);
        display.setCursor(35, SCREEN_HEIGHT - 10); // Ajustado para centrar mejor
        display.print("Conectado!");
        display.display();
        delay(25);
    }
    for(int i=0; i < SCREEN_WIDTH; i+=5) {
        display.fillRect(0, SCREEN_HEIGHT - 20, i, 8, SSD1306_WHITE);
        display.display();
        delay(8);
    }
    display.clearDisplay();
    display.setTextSize(2);
    // Centrar Texto "LISTO!"
    int16_t x1, y1;
    uint16_t w, h;
    String listoStr = "LISTO!";
    display.getTextBounds(listoStr, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
    display.print(listoStr);
    display.display();
    delay(500);
    display.clearDisplay(); 
    display.display();
}

void drawArrow(const String& direction) {
    display.clearDisplay();
    int16_t x1, y1; uint16_t w, h; // Para centrar texto

    // Mapeo invertido para corresponder con movimiento físico del carro en el display
    // Comando 'D' (Atrás general) -> Flecha Arriba en display
    if(direction == "D") { 
        display.fillTriangle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 15, SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2, SCREEN_WIDTH/2 + 10, SCREEN_HEIGHT/2, SSD1306_WHITE);
        display.fillRect(SCREEN_WIDTH/2 - 4, SCREEN_HEIGHT/2, 8, 15, SSD1306_WHITE); 
    }
    // Comando 'U' (Adelante general) -> Flecha Abajo en display
    else if(direction == "U") { 
        display.fillTriangle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 15, SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2, SCREEN_WIDTH/2 + 10, SCREEN_HEIGHT/2, SSD1306_WHITE);
        display.fillRect(SCREEN_WIDTH/2 - 4, SCREEN_HEIGHT/2 - 15, 8, 15, SSD1306_WHITE);
    }
    // Comando 'R' (Giro Derecha físico) -> Flecha Izquierda en display
    else if(direction == "R") { 
        display.fillTriangle(SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 10, SSD1306_WHITE);
        display.fillRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 4, 15, 8, SSD1306_WHITE);
    }
    // Comando 'L' (Giro Izquierda físico) -> Flecha Derecha en display
    else if(direction == "L") { 
        display.fillTriangle(SCREEN_WIDTH/2 + 15, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 10, SSD1306_WHITE);
        display.fillRect(SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2 - 4, 15, 8, SSD1306_WHITE);
    }
    else if(direction == "X") {
        display.setTextSize(1); display.setCursor(0,0); display.println("Test Pantalla:");
        display.drawRect(0, 10, SCREEN_WIDTH-1, SCREEN_HEIGHT-11, SSD1306_WHITE);
        for(int i=0; i<60; i++) { 
            display.drawPixel(random(1,SCREEN_WIDTH-2), random(11,SCREEN_HEIGHT-12), SSD1306_WHITE);
        }
        display.display(); delay(1000);
        display.clearDisplay(); display.setTextSize(2);
        String oledOkStr = "OLED OK!";
        display.getTextBounds(oledOkStr, 0, 0, &x1, &y1, &w, &h);
        display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
        display.println(oledOkStr);
        display.display(); delay(800); display.clearDisplay();
    }
    else if(direction == "F1") { display.setTextSize(1); display.setCursor(10, 28); display.print("Motor DER: Adelante"); }
    else if(direction == "B1") { display.setTextSize(1); display.setCursor(10, 28); display.print("Motor DER: Atras"); }
    else if(direction == "S1") { display.setTextSize(1); display.setCursor(10, 28); display.print("Motor DER: STOP"); }
    else if(direction == "F2") { display.setTextSize(1); display.setCursor(10, 28); display.print("Motor IZQ: Adelante"); }
    else if(direction == "B2") { display.setTextSize(1); display.setCursor(10, 28); display.print("Motor IZQ: Atras"); }
    else if(direction == "S2") { display.setTextSize(1); display.setCursor(10, 28); display.print("Motor IZQ: STOP"); }
    else { 
        display.setTextSize(2);
        String stopStr = "STOP";
        display.getTextBounds(stopStr, 0, 0, &x1, &y1, &w, &h);
        display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
        display.print(stopStr);
    }
    display.display();
}

void controlMotor1(char direction, uint8_t speedVal) {
    uint16_t compensatedSpeed = speedVal; // Usar uint16_t temporalmente por si la compensación es grande
    if (speedVal > 0) {
        compensatedSpeed = speedVal + compensation;
        if (compensatedSpeed > 255) compensatedSpeed = 255;
        if (compensatedSpeed < 0) compensatedSpeed = 0; // Aunque uint no será negativo, por si acaso.
    }
    uint8_t finalSpeed = (uint8_t)compensatedSpeed;

    if (direction == 'F') { ledcWrite(M1_IN1_CHANNEL, finalSpeed); ledcWrite(M1_IN2_CHANNEL, 0); }
    else if (direction == 'B') { ledcWrite(M1_IN1_CHANNEL, 0); ledcWrite(M1_IN2_CHANNEL, finalSpeed); }
    else if (direction == 'S') { ledcWrite(M1_IN1_CHANNEL, 0); ledcWrite(M1_IN2_CHANNEL, 0); }
}

void controlMotor2(char direction, uint8_t speedVal) {
    if (direction == 'F') { ledcWrite(M2_IN1_CHANNEL, speedVal); ledcWrite(M2_IN2_CHANNEL, 0); }
    else if (direction == 'B') { ledcWrite(M2_IN1_CHANNEL, 0); ledcWrite(M2_IN2_CHANNEL, speedVal); }
    else if (direction == 'S') { ledcWrite(M2_IN1_CHANNEL, 0); ledcWrite(M2_IN2_CHANNEL, 0); }
}

void moveMotorsGeneral(char command) {
    switch(command) {
        case 'U': controlMotor1('F', generalSpeed); controlMotor2('F', generalSpeed); break;
        case 'D': controlMotor1('B', generalSpeed); controlMotor2('B', generalSpeed); break;
        case 'L': controlMotor1('B', generalSpeed); controlMotor2('F', generalSpeed); break; // Izquierda físico
        case 'R': controlMotor1('F', generalSpeed); controlMotor2('B', generalSpeed); break; // Derecha físico
        case 'S': controlMotor1('S', 0); controlMotor2('S', 0); break;
    }
}

void loop() {
    if (SerialBT.connected()) {
        if (!connectedBefore) {
            animacionInicio();
            connectedBefore = true;
            lastCommandTime = millis(); 
        }

        if (SerialBT.available()) {
            String readData = SerialBT.readStringUntil('\n');
            readData.trim();
            lastCommandTime = millis(); 

            if (readData.length() > 0) {
                char firstChar = readData.charAt(0);
                Serial.print("RX: "); Serial.println(readData);

                if (firstChar == 'C') { 
                    if (readData.length() >= 3) { 
                        char target = readData.charAt(1); 
                        // Asegurar que haya suficientes caracteres para el valor de velocidad
                        // CGXXX (5 chars), C1XXX (5 chars)
                        int required_len = (target == 'G' ? 5 : 5); 
                        if (readData.length() >= required_len) {
                           String speedStr = readData.substring(2, 5); // Siempre toma 3 dígitos para XXX
                           int speedValue = speedStr.toInt();

                            if (speedValue >= 0 && speedValue <= 255) {
                                if (target == '1') {
                                    motor1Speed = speedValue;
                                    Serial.print("M1(Ind) Vel: "); Serial.println(motor1Speed);
                                } else if (target == '2') {
                                    motor2Speed = speedValue;
                                    Serial.print("M2(Ind) Vel: "); Serial.println(motor2Speed);
                                } else if (target == 'G') {
                                    generalSpeed = speedValue;
                                    Serial.print("Vel. General: "); Serial.println(generalSpeed);
                                    display.clearDisplay(); display.setTextSize(1);
                                    display.setCursor(5,20); display.print("Vel. General:");
                                    display.setTextSize(2);
                                    // Centrar el valor de la velocidad
                                    int16_t x1, y1; uint16_t w, h;
                                    String speedValStr = String(generalSpeed);
                                    display.getTextBounds(speedValStr, 0, 0, &x1, &y1, &w, &h);
                                    display.setCursor((SCREEN_WIDTH - w) / 2, 35);
                                    display.print(speedValStr);
                                    display.display();
                                    // No redibujar flecha, solo mostrar actualización de velocidad
                                }
                            } else { Serial.println("Vel. fuera de rango (0-255)."); }
                        } else { Serial.println("Comando C incompleto."); }
                    }
                } else if (firstChar == 'F' || firstChar == 'B' || firstChar == 'S') { 
                    if (readData.length() == 2) { // F1, B1, S1, F2, B2, S2
                        char motorId = readData.charAt(1);
                        if (motorId == '1') { controlMotor1(firstChar, motor1Speed); }
                        else if (motorId == '2') { controlMotor2(firstChar, motor2Speed); }
                        drawArrow(readData);
                    } else if (firstChar == 'S' && readData.length() == 1) { // Comando 'S' general
                        moveMotorsGeneral('S');
                        drawArrow("S");
                    } else { Serial.println("Comando F/B/S malformado."); }
                } else if (firstChar == 'U' || firstChar == 'D' || firstChar == 'L' || firstChar == 'R' || firstChar == 'X') {
                    if (firstChar == 'X') { drawArrow("X"); }
                    else { moveMotorsGeneral(firstChar); drawArrow(String(firstChar)); }
                } else { Serial.print("Comando Desconocido: "); Serial.println(readData); }
            }
        } else { // No hay datos BT disponibles, verificar timeout
            if (millis() - lastCommandTime > commandTimeout) {
                if (connectedBefore) { 
                    Serial.println("Timeout BT -> STOP");
                    moveMotorsGeneral('S');
                    drawArrow("S"); 
                    lastCommandTime = millis(); // Resetear para no enviar 'S' continuamente hasta nuevo comando
                }
            }
        }

    } else { // Bluetooth no conectado
        if (connectedBefore) {
            display.clearDisplay(); display.setTextSize(1);
            display.setCursor(15, 28); display.print("Desconectado!");
            display.display(); delay(1000);
            connectedBefore = false;
            moveMotorsGeneral('S'); 
        }
        animacionEsperaBT();
        delay(40); // Reducido para animación más fluida
    }
}
