/*
 * ==========================================================
 *  DHT11 – Meranie teploty a vlhkosti
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Senzor:   DHT11
 *  Pin:      D2 (digitálny)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 2 sekundy (non-blocking, millis())
 * 
 *  Zapojenie:
 *    DHT11 VCC  -> 5V
 *    DHT11 GND  -> GND
 *    DHT11 DATA -> D2 (+ 10kΩ pull-up rezistor medzi DATA a VCC)
 * 
 *  Potrebné knižnice (Arduino Library Manager):
 *    - DHT sensor library (Adafruit)
 *    - Adafruit Unified Sensor
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

#include <DHT.h>

// ---- Konfigurácia ----
#define DHT_PIN       2           // Dátový pin senzora
#define DHT_TYP       DHT11      // Typ senzora
#define INTERVAL_MS   2000       // Interval merania (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie

// ---- Globálne premenné ----
DHT dht(DHT_PIN, DHT_TYP);
unsigned long predchadzajuciCas = 0;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);
  dht.begin();

  Serial.println(F("{\"stav\":\"inicializacia\",\"senzor\":\"DHT11\",\"pin\":\"D2\"}"));
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  unsigned long aktualyCas = millis();

  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;
    merajTeplotu();
  }
}

// ==========================================================
//  Funkcia na meranie a výpis JSON
// ==========================================================
void merajTeplotu() {
  float teplota = dht.readTemperature();     // °C
  float vlhkost = dht.readHumidity();        // %

  // Kontrola chýb
  if (isnan(teplota) || isnan(vlhkost)) {
    Serial.println(F("{\"senzor\":\"DHT11\",\"chyba\":\"Nepodarilo sa precitat data zo senzora\"}"));
    return;
  }

  // JSON výstup
  Serial.print(F("{\"senzor\":\"DHT11\",\"teplota\":"));
  Serial.print(teplota, 2);
  Serial.print(F(",\"vlhkost\":"));
  Serial.print(vlhkost, 2);
  Serial.println(F(",\"jednotka\":\"°C\"}"));
}
