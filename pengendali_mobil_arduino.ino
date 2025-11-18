// === PROGRAM PENGENDALI MOBIL (HAND) ===
// ESP32 + MPU6050 + Bluetooth Classic

#include <Wire.h>
#include <BluetoothSerial.h>
#include <MPU6050_light.h>

BluetoothSerial SerialBT;
MPU6050 mpu(Wire);

const char* targetName = "mobil kece"; // Ganti dengan nama Bluetooth mobil kamu

float threshold = 15.0;
unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL = 300;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Inisialisasi MPU6050...");
  mpu.begin();
  mpu.calcOffsets(); // kalibrasi awal

  // Nyalakan Bluetooth dan sambungkan ke mobil
  SerialBT.begin("ESP32_TANGAN", true); // true = mode master
  Serial.println("Menyambungkan ke Bluetooth mobil...");

  bool connected = SerialBT.connect("mobil kece");

  if (connected) {
    Serial.println("Bluetooth tersambung ke mobil ✅");
  } else {
    Serial.println("Gagal tersambung. Pastikan mobil menyala dan namanya sesuai!");
  }
}

void loop() {
  mpu.update();

  // Baca sudut rotasi dari MPU
  float angleX = mpu.getAngleX(); // miring depan/belakang
  float angleY = mpu.getAngleY(); // miring kiri/kanan

  char command = 'S'; // default stop

  if (angleX > threshold) command = 'M';        // Maju
  else if (angleX < -threshold) command = 'B';  // Mundur
  else if (angleY > threshold) command = 'R';   // Belok kanan
  else if (angleY < -threshold) command = 'L';  // Belok kiri

  // Kirim perintah tiap 300ms
  if (millis() - lastSend > SEND_INTERVAL) {
    SerialBT.write(command);
    Serial.print("Kirim perintah: ");
    Serial.println(command);
    lastSend = millis();
  }

  delay(10);
}
