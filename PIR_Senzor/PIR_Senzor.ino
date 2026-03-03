/*
 * ==========================================================
 *  PIR – Pasívny infračervený senzor pohybu
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Senzor:   HC-SR501 (PIR)
 *  Pin:      D9 (digitálny)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 200 ms (non-blocking, millis())
 * 
 *  Zapojenie:
 *    PIR VCC  -> 5V
 *    PIR GND  -> GND
 *    PIR OUT  -> D9
 * 
 *  Poznámka:
 *    Po zapnutí senzor potrebuje ~60 sekúnd na kalibráciu.
 *    Potenciometre na module: citlivosť a doba držania.
 *    Žiadne externé knižnice nie sú potrebné.
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

// ---- Konfigurácia ----
#define PIR_PIN       9           // Výstupný pin PIR senzora
#define INTERVAL_MS   200        // Interval kontroly (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie

// ---- Globálne premenné ----
unsigned long predchadzajuciCas = 0;
bool poslednyStav = false;
unsigned long pocetDetekcii = 0;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(PIR_PIN, INPUT);

  Serial.println(F("{\"stav\":\"inicializacia\",\"senzor\":\"PIR\",\"pin\":\"D9\",\"poznamka\":\"Kalibracia 60s\"}"));
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  unsigned long aktualyCas = millis();

  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;
    kontrolujPohyb();
  }
}

// ==========================================================
//  Kontrola pohybu a výpis JSON
// ==========================================================
void kontrolujPohyb() {
  bool aktualnyStav = digitalRead(PIR_PIN);

  // Výpis len pri zmene stavu (hrana)
  if (aktualnyStav != poslednyStav) {
    poslednyStav = aktualnyStav;

    if (aktualnyStav) {
      pocetDetekcii++;
    }

    Serial.print(F("{\"senzor\":\"PIR\",\"pohyb\":"));
    Serial.print(aktualnyStav ? F("true") : F("false"));
    Serial.print(F(",\"pocet_detekcii\":"));
    Serial.print(pocetDetekcii);
    Serial.print(F(",\"cas_ms\":"));
    Serial.print(millis());
    Serial.println(F("}"));
  }
}
