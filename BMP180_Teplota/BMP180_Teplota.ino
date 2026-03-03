/*
 * ==========================================================
 *  BMP180 – Meranie teploty a atmosférického tlaku
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Senzor:   BMP180 (I²C)
 *  Piny:     A4 (SDA), A5 (SCL)
 *  Adresa:   0x77 (fixná)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 2 sekundy (non-blocking, millis())
 * 
 *  Zapojenie:
 *    BMP180 VIN -> 3.3V (alebo 5V ak má doska regulátor)
 *    BMP180 GND -> GND
 *    BMP180 SDA -> A4
 *    BMP180 SCL -> A5
 * 
 *  Potrebné knižnice (Arduino Library Manager):
 *    - Adafruit BMP085 Library (pokrýva aj BMP180)
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

#include <Wire.h>
#include <Adafruit_BMP085.h>

// ---- Konfigurácia ----
#define INTERVAL_MS   2000       // Interval merania (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie

// ---- Globálne premenné ----
Adafruit_BMP085 bmp;
unsigned long predchadzajuciCas = 0;
bool senzorOk = false;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);

  // Inicializácia BMP180
  senzorOk = bmp.begin();

  if (senzorOk) {
    Serial.println(F("{\"stav\":\"inicializacia\",\"senzor\":\"BMP180\",\"adresa\":\"0x77\",\"piny\":\"SDA=A4, SCL=A5\"}"));
  } else {
    Serial.println(F("{\"senzor\":\"BMP180\",\"chyba\":\"Senzor nebol najdeny. Skontrolujte zapojenie.\"}"));
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
  float teplota = bmp.readTemperature();       // °C
  float tlak    = bmp.readPressure() / 100.0;  // hPa (Pa -> hPa)

  // JSON výstup
  Serial.print(F("{\"senzor\":\"BMP180\",\"teplota\":"));
  Serial.print(teplota, 2);
  Serial.print(F(",\"tlak\":"));
  Serial.print(tlak, 2);
  Serial.println(F(",\"jednotka\":\"°C\"}"));
}
