/*
 * ==========================================================
 *  DS18B20 – Meranie teploty (1-Wire digitálny senzor)
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Senzor:   DS18B20
 *  Pin:      D4 (digitálny, 1-Wire)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 2 sekundy (non-blocking, millis())
 * 
 *  Zapojenie:
 *    DS18B20 VCC  -> 5V
 *    DS18B20 GND  -> GND
 *    DS18B20 DATA -> D4 (+ 4.7kΩ pull-up rezistor medzi DATA a VCC)
 * 
 *  Potrebné knižnice (Arduino Library Manager):
 *    - OneWire
 *    - DallasTemperature
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

#include <OneWire.h>
#include <DallasTemperature.h>

// ---- Konfigurácia ----
#define ONEWIRE_PIN   4           // Dátový pin 1-Wire zbernice
#define INTERVAL_MS   2000       // Interval merania (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie

// ---- Globálne premenné ----
OneWire oneWire(ONEWIRE_PIN);
DallasTemperature senzory(&oneWire);
unsigned long predchadzajuciCas = 0;
int pocetSenzorov = 0;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);
  senzory.begin();

  pocetSenzorov = senzory.getDeviceCount();

  Serial.print(F("{\"stav\":\"inicializacia\",\"senzor\":\"DS18B20\",\"pin\":\"D4\",\"najdene_senzory\":"));
  Serial.print(pocetSenzorov);
  Serial.println(F("}"));

  if (pocetSenzorov == 0) {
    Serial.println(F("{\"senzor\":\"DS18B20\",\"chyba\":\"Ziaden senzor nebol najdeny na zbernici\"}"));
  }
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
  senzory.requestTemperatures();  // Požiadavka na meranie

  for (int i = 0; i < pocetSenzorov; i++) {
    float teplota = senzory.getTempCByIndex(i);

    // Kontrola chýb (-127 = chyba komunikácie)
    if (teplota == DEVICE_DISCONNECTED_C) {
      Serial.print(F("{\"senzor\":\"DS18B20\",\"index\":"));
      Serial.print(i);
      Serial.println(F(",\"chyba\":\"Senzor odpojeny alebo chyba komunikacie\"}"));
      continue;
    }

    // Získanie adresy senzora
    DeviceAddress adresa;
    senzory.getAddress(adresa, i);

    // JSON výstup
    Serial.print(F("{\"senzor\":\"DS18B20\",\"index\":"));
    Serial.print(i);
    Serial.print(F(",\"adresa\":\""));
    for (uint8_t j = 0; j < 8; j++) {
      if (adresa[j] < 16) Serial.print('0');
      Serial.print(adresa[j], HEX);
    }
    Serial.print(F("\",\"teplota\":"));
    Serial.print(teplota, 2);
    Serial.println(F(",\"jednotka\":\"°C\"}"));
  }
}
