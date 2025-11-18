#
// === PROGRAM (MOBIL) ===
// ESP32 + L298N + Bluetooth Classic
#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

// Definisi pin L298N
const int L_ENA = 17; // PWM untuk motor kiri
const int L_IN1 = 18;
const int L_IN2 = 19;

const int R_ENB = 16; // PWM untuk motor kanan
const int R_IN3 = 25;
const int R_IN4 = 26;

// PWM settings
const int PWM_FREQ = 20000;
const int PWM_CHANNEL_L = 0;
const int PWM_CHANNEL_R = 1;
const int PWM_RES = 8; // 8-bit (0–255)

void stopMotor();
void forward(int spd);
void backward(int spd);
void left(int spd);
void right(int spd);
void handleCommand(char c);

void setup() {
  Serial.begin(115200);
  SerialBT.begin("mobil kece"); // Bluetooth server
  Serial.println("🚗 Mobil siap. Nama Bluetooth: mobil kece");

  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);
  pinMode(R_IN3, OUTPUT);
  pinMode(R_IN4, OUTPUT);

  ledcSetup(PWM_CHANNEL_L, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CHANNEL_R, PWM_FREQ, PWM_RES);
  ledcAttachPin(L_ENA, PWM_CHANNEL_L);
  ledcAttachPin(R_ENB, PWM_CHANNEL_R);

  stopMotor();
}

void loop() {
  if (SerialBT.available()) {
    char c = (char)SerialBT.read();
    Serial.print("📩 Perintah diterima: ");
    Serial.println(c);
    handleCommand(c);
  }
}

void handleCommand(char c) {
  switch (c) {
    case 'F': forward(255); break;
    case 'B': backward(255); break;
    case 'L': left(255); break;
    case 'R': right(255); break;
    case 'S': stopMotor(); break;
  }
}

void stopMotor() {
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN3, LOW);
  digitalWrite(R_IN4, LOW);
  ledcWrite(PWM_CHANNEL_L, 0);
  ledcWrite(PWM_CHANNEL_R, 0);
}

void forward(int spd) {
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN3, HIGH);
  digitalWrite(R_IN4, LOW);
  ledcWrite(PWM_CHANNEL_L, spd);
  ledcWrite(PWM_CHANNEL_R, spd);
}

void backward(int spd) {
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);
  digitalWrite(R_IN3, LOW);
  digitalWrite(R_IN4, HIGH);
  ledcWrite(PWM_CHANNEL_L, spd);
  ledcWrite(PWM_CHANNEL_R, spd);
}

void left(int spd) {
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);
  digitalWrite(R_IN3, HIGH);
  digitalWrite(R_IN4, LOW);
  ledcWrite(PWM_CHANNEL_L, spd);
  ledcWrite(PWM_CHANNEL_R, spd);
}

void right(int spd) {
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN3, LOW);
  digitalWrite(R_IN4, HIGH);
  ledcWrite(PWM_CHANNEL_L, spd);
  ledcWrite(PWM_CHANNEL_R, spd);
}
