/*
   Autores:
   Javier Ronaldo Gil Ayala
   Juan Sebastián Carreño Patiño
   
   Año: 2025
   Proyecto: Futbot - Prueba de Motores
   Descripción:
   - Activa los motores en diferentes direcciones con pausas entre cada acción.
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
