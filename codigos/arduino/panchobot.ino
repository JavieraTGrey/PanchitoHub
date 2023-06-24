#include <ESP8266WiFi.h>
#include <Servo.h>

const char* ssid = "PanchoWifi";
const char* password = "12345";
WiFiServer server(80);

const int PWMA = 15;
const int AIN1 = 14;
const int AIN2 = 12;
const int PWMB = 13;
const int BIN1 = 4;
const int BIN2 = 5;
const int STBY = 0;

Servo servoMotor;
int servoAngle = 90;

void setup() {
  Serial.begin(9600);
  delay(10);

  servoMotor.attach(2);

  server.begin();
  WiFi.mode(WIFI_AP);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);
}

void loop() {
  int estado = HIGH;

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while (!client.available()) {
    delay(1);
  }

  String peticion = client.readStringUntil('\r');
  client.flush();

  if (peticion.indexOf("/on") != -1) {
    estado = LOW;
  }
  if (peticion.indexOf("/off") != -1) {
    estado = HIGH;
  }

  if (estado == LOW) {
    digitalWrite(2, LOW); 
    servoAngle = 10;
    servoMotor.write(servoAngle);
  }
  else {
    digitalWrite(2, HIGH); // Reemplaza el número de pin con el correspondiente al LED
    servoAngle = 90;
    servoMotor.write(servoAngle);
  }

  if (peticion.indexOf("/Izquierda") != -1) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, 200);

    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, 200);

    delay(100);
  } else if (peticion.indexOf("/Derecha") != -1) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, 200);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, 200);

    delay(100);
  } else if (peticion.indexOf("/Atras") != -1) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, 250);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, 250);

    delay(300);
  } else if (peticion.indexOf("/Adelante") != -1) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, 200);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, 200);

    delay(200);
  } else if (peticion.indexOf("/Detener") != -1) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, LOW);

    delay(200);
  }

  digitalWrite(PWMA, LOW);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);

  digitalWrite(PWMB, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);

  delay(100);

  Serial.println("Client disconnected");
}

