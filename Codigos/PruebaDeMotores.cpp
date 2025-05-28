/*
  ============================================================================
  ROBÓTICA CODELAB SAS. 
  
  Proyecto: Código de prueba para motores con ESP32 y Driver L293D
  Autor: Javier Ronaldo Gil Ayala - Juan Sebastian Carreño Patiño
  Fecha de desarrollo: Mayo de 2025

  Descripción:
  Este código fue desarrollado como parte de un proyecto educativo para probar el control básico de dos motores DC utilizando un ESP32 y el driver L293D. 
  El objetivo es verificar el funcionamiento de los motores, permitiendo movimientos hacia adelante, atrás y paradas, mediante la activación directa de los pines de control desde el microcontrolador.

  Uso:
  - Conecta el ESP32 al driver L293D y a los motores según el esquema de pines definido en el código.
  - Alimenta adecuadamente el driver y los motores.
  - Carga este código en el ESP32.
  - Observa la secuencia de movimientos de los motores (adelante, parada, atrás, parada) como prueba de funcionamiento.

  Este proyecto sirve como base para el desarrollo de sistemas de robótica móvil y control de actuadores en tiempo real.

  www.roboticacodelab.com - Página principal 
  www.roboticacodelab.online - Página educativa 
  www.instagram.com/robotica_codelab - Instagram
  ============================================================================
*/

#define MOTOR_IZQ_A 27
#define MOTOR_IZQ_B 26
#define MOTOR_DER_A 2
#define MOTOR_DER_B 4

void setup() {
    pinMode(MOTOR_IZQ_A, OUTPUT);
    pinMode(MOTOR_IZQ_B, OUTPUT);
    pinMode(MOTOR_DER_A, OUTPUT);
    pinMode(MOTOR_DER_B, OUTPUT);
}

void loop() {
    // Avanzar
    digitalWrite(MOTOR_IZQ_A, HIGH);
    digitalWrite(MOTOR_IZQ_B, LOW);
    digitalWrite(MOTOR_DER_A, HIGH);
    digitalWrite(MOTOR_DER_B, LOW);
    delay(2000);

    // Retroceder
    digitalWrite(MOTOR_IZQ_A, LOW);
    digitalWrite(MOTOR_IZQ_B, HIGH);
    digitalWrite(MOTOR_DER_A, LOW);
    digitalWrite(MOTOR_DER_B, HIGH);
    delay(2000);

    // Girar izquierda
    digitalWrite(MOTOR_IZQ_A, LOW);
    digitalWrite(MOTOR_IZQ_B, HIGH);
    digitalWrite(MOTOR_DER_A, HIGH);
    digitalWrite(MOTOR_DER_B, LOW);
    delay(2000);

    // Girar derecha
    digitalWrite(MOTOR_IZQ_A, HIGH);
    digitalWrite(MOTOR_IZQ_B, LOW);
    digitalWrite(MOTOR_DER_A, LOW);
    digitalWrite(MOTOR_DER_B, HIGH);
    delay(2000);

    // Detenerse
    digitalWrite(MOTOR_IZQ_A, LOW);
    digitalWrite(MOTOR_IZQ_B, LOW);
    digitalWrite(MOTOR_DER_A, LOW);
    digitalWrite(MOTOR_DER_B, LOW);
    delay(2000);
}
