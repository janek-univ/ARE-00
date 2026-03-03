/*
 * ==========================================================
 *  MPU-6050 – Akcelerometer a gyroskop (6-osový IMU)
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Senzor:   MPU-6050 (I²C)
 *  Piny:     A4 (SDA), A5 (SCL)
 *  Adresa:   0x68 (predvolená), 0x69 (ak AD0=HIGH)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 500 ms (non-blocking, millis())
 * 
 *  Zapojenie:
 *    MPU-6050 VCC -> 5V (alebo 3.3V)
 *    MPU-6050 GND -> GND
 *    MPU-6050 SDA -> A4
 *    MPU-6050 SCL -> A5
 *    MPU-6050 AD0 -> GND (adresa 0x68) alebo VCC (adresa 0x69)
 * 
 *  Potrebné knižnice (Arduino Library Manager):
 *    - Adafruit MPU6050
 *    - Adafruit Unified Sensor
 *    - Adafruit BusIO (automatická závislosť)
 * 
 *  Poznámka:
 *    Ak používate aj DS3231 RTC (tiež 0x68), pripojte AD0 na VCC
 *    pre zmenu adresy MPU-6050 na 0x69.
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// ---- Konfigurácia ----
#define INTERVAL_MS   500        // Interval merania (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie

// ---- Globálne premenné ----
Adafruit_MPU6050 mpu;
unsigned long predchadzajuciCas = 0;
bool senzorOk = false;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);

  senzorOk = mpu.begin();

  if (senzorOk) {
    // Nastavenie rozsahov
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println(F("{\"stav\":\"inicializacia\",\"senzor\":\"MPU-6050\",\"adresa\":\"0x68\",\"rozsah_g\":\"2G\",\"rozsah_gyro\":\"250deg/s\"}"));
  } else {
    Serial.println(F("{\"senzor\":\"MPU-6050\",\"chyba\":\"Senzor nebol najdeny. Skontrolujte zapojenie.\"}"));
  }
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  if (!senzorOk) return;

  unsigned long aktualyCas = millis();

  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;
    merajIMU();
  }
}

// ==========================================================
//  Meranie a výpis JSON
// ==========================================================
void merajIMU() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // JSON výstup
  Serial.print(F("{\"senzor\":\"MPU-6050\""));

  // Akcelerometer (m/s²)
  Serial.print(F(",\"accel\":{\"x\":"));
  Serial.print(a.acceleration.x, 2);
  Serial.print(F(",\"y\":"));
  Serial.print(a.acceleration.y, 2);
  Serial.print(F(",\"z\":"));
  Serial.print(a.acceleration.z, 2);
  Serial.print(F(",\"jednotka\":\"m/s2\"}"));

  // Gyroskop (rad/s)
  Serial.print(F(",\"gyro\":{\"x\":"));
  Serial.print(g.gyro.x, 2);
  Serial.print(F(",\"y\":"));
  Serial.print(g.gyro.y, 2);
  Serial.print(F(",\"z\":"));
  Serial.print(g.gyro.z, 2);
  Serial.print(F(",\"jednotka\":\"rad/s\"}"));

  // Teplota
  Serial.print(F(",\"teplota\":"));
  Serial.print(temp.temperature, 2);

  Serial.println(F("}"));
}
