/*
 * ==========================================================
 *  BME280 – Meranie teploty, vlhkosti a tlaku
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Senzor:   BME280 (I²C)
 *  Piny:     A4 (SDA), A5 (SCL)
 *  Adresa:   0x76 (predvolená)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 2 sekundy (non-blocking, millis())
 * 
 *  Zapojenie:
 *    BME280 VIN -> 5V (alebo 3.3V)
 *    BME280 GND -> GND
 *    BME280 SDA -> A4
 *    BME280 SCL -> A5
 * 
 *  Potrebné knižnice (Arduino Library Manager):
 *    - Adafruit BME280 Library
 *    - Adafruit Unified Sensor
 *    - Adafruit BusIO (automatická závislosť)
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// ---- Konfigurácia ----
#define BME280_ADRESA 0x76       // I²C adresa (0x76 alebo 0x77)
#define INTERVAL_MS   2000       // Interval merania (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie

// ---- Globálne premenné ----
Adafruit_BME280 bme;
unsigned long predchadzajuciCas = 0;
bool senzorOk = false;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);

  // Inicializácia BME280
  senzorOk = bme.begin(BME280_ADRESA);

  if (senzorOk) {
    Serial.println(F("{\"stav\":\"inicializacia\",\"senzor\":\"BME280\",\"adresa\":\"0x76\",\"piny\":\"SDA=A4, SCL=A5\"}"));
  } else {
    Serial.println(F("{\"senzor\":\"BME280\",\"chyba\":\"Senzor nebol najdeny. Skontrolujte zapojenie a I2C adresu.\"}"));
  }
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  if (!senzorOk) return;  // Ak senzor nie je pripojený, nerob nič

  unsigned long aktualyCas = millis();

  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;
    merajData();
  }
}

// ==========================================================
//  Funkcia na meranie a výpis JSON
// ==========================================================
void merajData() {
  float teplota = bme.readTemperature();       // °C
  float vlhkost = bme.readHumidity();          // %
  float tlak    = bme.readPressure() / 100.0F; // hPa (Pa -> hPa)

  // JSON výstup
  Serial.print(F("{\"senzor\":\"BME280\",\"teplota\":"));
  Serial.print(teplota, 2);
  Serial.print(F(",\"vlhkost\":"));
  Serial.print(vlhkost, 2);
  Serial.print(F(",\"tlak\":"));
  Serial.print(tlak, 2);
  Serial.println(F(",\"jednotka\":\"°C\"}"));
}
