/*
 * ==========================================================
 *  LM35 – Meranie teploty (analógový senzor)
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Senzor:   LM35
 *  Pin:      A0 (analógový)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 2 sekundy (non-blocking, millis())
 * 
 *  Zapojenie:
 *    LM35 VCC  -> 5V
 *    LM35 GND  -> GND
 *    LM35 VOUT -> A0
 * 
 *  Poznámka:
 *    LM35 výstup = 10 mV/°C
 *    Formula: teplota = (analogRead * 5.0 / 1024.0) * 100.0
 *    Žiadne externé knižnice nie sú potrebné.
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

// ---- Konfigurácia ----
#define LM35_PIN      A0          // Analógový pin senzora
#define INTERVAL_MS   2000       // Interval merania (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie
#define NAPATIE_REF   5.0        // Referenčné napätie (V)
#define ADC_MAX       1024.0     // Rozlíšenie ADC

// ---- Globálne premenné ----
unsigned long predchadzajuciCas = 0;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);

  Serial.println(F("{\"stav\":\"inicializacia\",\"senzor\":\"LM35\",\"pin\":\"A0\"}"));
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
  // Priemerovanie 10 vzoriek pre presnejší výsledok
  float sucet = 0.0;
  for (int i = 0; i < 10; i++) {
    sucet += analogRead(LM35_PIN);
    delay(10);  // Krátka pauza medzi vzorkami ADC
  }
  float priemer = sucet / 10.0;

  // Prepočet na °C: LM35 dáva 10 mV/°C
  float napatie = (priemer * NAPATIE_REF) / ADC_MAX;
  float teplota = napatie * 100.0;

  // JSON výstup
  Serial.print(F("{\"senzor\":\"LM35\",\"teplota\":"));
  Serial.print(teplota, 2);
  Serial.println(F(",\"jednotka\":\"°C\"}"));
}
