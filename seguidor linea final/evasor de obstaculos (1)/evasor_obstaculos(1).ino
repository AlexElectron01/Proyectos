#include <Servo.h>

const int trigPin = 8;
const int echoPin = 9;
const int servoPin = 10;

const int in1 = 2;
const int in2 = 3;
const int in3 = 4;
const int in4 = 5;

Servo miServo;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  miServo.attach(servoPin);
  miServo.write(90);  // Posición inicial: frente
}

void loop() {
  // Mira al frente y mide
  miServo.write(90);
  delay(1000);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH, 25000);
  float distancia = duracion * 0.034 / 2;

  Serial.print("Distancia frente: ");
  Serial.println(distancia);

  if (distancia > 40) {
    // Avanzar
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else {
    // Retroceder un poco
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(700);

    // Detener
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);

    // Medir izquierda
    miServo.write(30);
    delay(1000);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duracion = pulseIn(echoPin, HIGH, 25000);
    float distanciaIzq = duracion * 0.034 / 2;

    // Medir derecha
    miServo.write(150);
    delay(1000);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duracion = pulseIn(echoPin, HIGH, 25000);
    float distanciaDer = duracion * 0.034 / 2;

    // Volver al centro
    miServo.write(90);
    delay(1000);

    // Elegir dirección
    if (distanciaIzq > distanciaDer) {
      // Girar izquierda
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    } else {
      // Girar derecha
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }

    delay(600);  // Tiempo de giro

    // Detener
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }

  delay(100);
}
