/*
 * ==========================================================
 *  VEML7700 – Meranie intenzity okolitého svetla (lux)
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Senzor:   VEML7700 (I²C)
 *  Piny:     A4 (SDA), A5 (SCL)
 *  Adresa:   0x10 (fixná)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 2 sekundy (non-blocking, millis())
 * 
 *  Zapojenie:
 *    VEML7700 VIN -> 3.3V (alebo 5V ak má doska regulátor)
 *    VEML7700 GND -> GND
 *    VEML7700 SDA -> A4
 *    VEML7700 SCL -> A5
 * 
 *  Potrebné knižnice (Arduino Library Manager):
 *    - Adafruit VEML7700 Library
 *    - Adafruit BusIO (automatická závislosť)
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

#include <Wire.h>
#include <Adafruit_VEML7700.h>

// ---- Konfigurácia ----
#define INTERVAL_MS   2000       // Interval merania (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie

// ---- Globálne premenné ----
Adafruit_VEML7700 veml = Adafruit_VEML7700();
unsigned long predchadzajuciCas = 0;
bool senzorOk = false;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);

  // Inicializácia VEML7700
  senzorOk = veml.begin();

  if (senzorOk) {
    // Nastavenie zosilnenia a integračného času
    veml.setGain(VEML7700_GAIN_1);
    veml.setIntegrationTime(VEML7700_IT_100MS);

    Serial.println(F("{\"stav\":\"inicializacia\",\"senzor\":\"VEML7700\",\"adresa\":\"0x10\",\"piny\":\"SDA=A4, SCL=A5\"}"));
  } else {
    Serial.println(F("{\"senzor\":\"VEML7700\",\"chyba\":\"Senzor nebol najdeny. Skontrolujte zapojenie.\"}"));
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
    merajSvetlo();
  }
}

// ==========================================================
//  Funkcia na meranie a výpis JSON
// ==========================================================
void merajSvetlo() {
  float lux       = veml.readLux();        // Intenzita svetla (lux)
  float bieleSvetlo = veml.readWhite();     // Biely kanál (raw)
  float als        = veml.readALS();        // ALS kanál (raw)

  // JSON výstup
  Serial.print(F("{\"senzor\":\"VEML7700\",\"lux\":"));
  Serial.print(lux, 2);
  Serial.print(F(",\"biele_svetlo\":"));
  Serial.print(bieleSvetlo, 2);
  Serial.print(F(",\"als\":"));
  Serial.print(als, 2);
  Serial.println(F(",\"jednotka\":\"lx\"}"));
}
