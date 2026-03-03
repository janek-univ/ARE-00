/*
 * ==========================================================
 *  DHT22 – Meranie teploty a vlhkosti
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Senzor:   DHT22 (AM2302)
 *  Pin:      D3 (digitálny)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 2 sekundy (non-blocking, millis())
 * 
 *  Zapojenie:
 *    DHT22 VCC  -> 5V
 *    DHT22 GND  -> GND
 *    DHT22 DATA -> D3 (+ 10kΩ pull-up rezistor medzi DATA a VCC)
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
#define DHT_PIN       3           // Dátový pin senzora
#define DHT_TYP       DHT22      // Typ senzora
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

  Serial.println(F("{\"stav\":\"inicializacia\",\"senzor\":\"DHT22\",\"pin\":\"D3\"}"));
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
    Serial.println(F("{\"senzor\":\"DHT22\",\"chyba\":\"Nepodarilo sa precitat data zo senzora\"}"));
    return;
  }

  // JSON výstup
  Serial.print(F("{\"senzor\":\"DHT22\",\"teplota\":"));
  Serial.print(teplota, 2);
  Serial.print(F(",\"vlhkost\":"));
  Serial.print(vlhkost, 2);
  Serial.println(F(",\"jednotka\":\"°C\"}"));
}
