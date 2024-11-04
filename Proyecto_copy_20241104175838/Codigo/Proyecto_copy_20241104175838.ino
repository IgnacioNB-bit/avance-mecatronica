#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 rfid(SS_PIN, RST_PIN); // Crear objeto MFRC522

const int in1 = 4;
const int in2 = 5;
const int in3 = 6;
const int in4 = 7;
const int ledPin = 2; // Pin del LED

const int stepsPerRevolution = 2048; // Pasos por revolución para el 28BYJ-48
const int stepsFor90Degrees = stepsPerRevolution / 14; // Pasos para 90 grados (2048/4 = 512)
int readCount = 0; // Contador de lecturas

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
  
  Serial.begin(9600);
  SPI.begin(); // Inicializar el bus SPI
  rfid.PCD_Init(); // Inicializar el RFID
  Serial.println("Esperando lectura de RFID...");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    // Si se detecta una tarjeta
    Serial.println("Tarjeta leída!");
    
    // Mensaje para depuración
    Serial.print("Contador de lecturas: ");
    Serial.println(readCount);

    // Alternar el sentido de giro
    if (readCount % 2 == 0) {
      Serial.println("Gira 90 grados en sentido antihorario");
      moveMotor(stepsFor90Degrees, false); // Gira 90 grados en sentido antihorario
      digitalWrite(ledPin, HIGH); // Enciende el LED
    } else {
      Serial.println("Gira 90 grados en sentido horario");
      moveMotor(stepsFor90Degrees, true); // Gira 90 grados en sentido horario
      digitalWrite(ledPin, LOW); // Apaga el LED
    }
    
    readCount++; // Incrementar contador de lecturas
    rfid.PICC_HaltA(); // Detener lectura del RFID
  }
}

void moveMotor(int steps, bool clockwise) {
  for (int i = 0; i < steps; i++) {
    stepMotor(clockwise); // Gira en la dirección correspondiente
  }
}

void stepMotor(bool clockwise) {
  if (clockwise) { // Sentido horario
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    delay(5); // Aumentar el retardo
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    delay(5);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    delay(5);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(5);
    digitalWrite(in4, LOW);
  } else { // Sentido antihorario
    digitalWrite(in4, HIGH);
    delay(5); // Aumentar el retardo
    digitalWrite(in4, LOW);
    digitalWrite(in3, HIGH);
    delay(5);
    digitalWrite(in3, LOW);
    digitalWrite(in2, HIGH);
    delay(5);
    digitalWrite(in2, LOW);
    digitalWrite(in1, HIGH);
    delay(5);
    digitalWrite(in1, LOW);
  }
}
