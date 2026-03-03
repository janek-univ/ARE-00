/*
 * ==========================================================
 *  NTC 10kΩ – Meranie teploty s napäťovým deličom
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Senzor:   NTC termistor 10kΩ (B=3950)
 *  Pin:      A1 (analógový)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 2 sekundy (non-blocking, millis())
 * 
 *  Zapojenie (napäťový delič):
 * 
 *    5V ──┬── R1 (10kΩ fixný) ──┬── NTC (10kΩ) ── GND
 *          │                     │
 *          │                     └── A1 (Arduino)
 *          │
 *          └── (Vcc)
 * 
 *    Schéma:
 *      5V → 10kΩ rezistor → [uzol A1] → NTC termistor → GND
 * 
 *  Výpočet teploty (Steinhart-Hart / Beta rovnica):
 *    1/T = 1/T0 + (1/B) * ln(R_ntc / R0)
 *    kde T0 = 25°C (298.15K), R0 = 10kΩ, B = 3950
 * 
 *  Poznámka:
 *    Žiadne externé knižnice nie sú potrebné.
 *    Hodnotu B konštanty (BETA) upravte podľa datasheetu vášho NTC.
 *    Bežné hodnoty: 3380, 3435, 3950, 4300.
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

// ---- Konfigurácia ----
#define NTC_PIN       A1          // Analógový pin
#define INTERVAL_MS   2000       // Interval merania (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie

// ---- Parametre NTC termistora ----
#define BETA          3950.0     // B konštanta (z datasheetu)
#define R_FIXNY       10000.0    // Fixný rezistor v deliči (Ω)
#define R_NTC_25      10000.0    // Odpor NTC pri 25°C (Ω)
#define T_REF         298.15     // Referenčná teplota (25°C v Kelvinoch)
#define NAPATIE_REF   5.0        // Referenčné napätie (V)
#define ADC_MAX       1023.0     // Maximálna hodnota ADC (10-bit)

// ---- Globálne premenné ----
unsigned long predchadzajuciCas = 0;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);

  Serial.println(F("{\"stav\":\"inicializacia\",\"senzor\":\"NTC_10k\",\"pin\":\"A1\",\"beta\":3950,\"r_fixny\":10000}"));
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
//  Meranie teploty a výpis JSON
// ==========================================================
void merajTeplotu() {
  // Priemerovanie 10 vzoriek pre presnejší výsledok
  float sucet = 0.0;
  for (int i = 0; i < 10; i++) {
    sucet += analogRead(NTC_PIN);
    delay(10);
  }
  float priemer = sucet / 10.0;

  // Ochrana proti deleniu nulou
  if (priemer <= 0 || priemer >= ADC_MAX) {
    Serial.println(F("{\"senzor\":\"NTC_10k\",\"chyba\":\"ADC hodnota mimo rozsahu - skontrolujte zapojenie\"}"));
    return;
  }

  // Výpočet odporu NTC z napäťového deliča
  // Vzorec: R_ntc = R_fixny * (ADC_MAX / ADC_hodnota - 1)
  // (NTC je na spodnej strane deliča, fixný rezistor navrchu)
  float r_ntc = R_FIXNY * (ADC_MAX / priemer - 1.0);

  // Výpočet teploty pomocou Beta rovnice (Steinhart-Hart zjednodušená)
  // 1/T = 1/T0 + (1/B) * ln(R_ntc / R0)
  float teplota_k = 1.0 / (1.0 / T_REF + (1.0 / BETA) * log(r_ntc / R_NTC_25));
  float teplota_c = teplota_k - 273.15;

  // JSON výstup
  Serial.print(F("{\"senzor\":\"NTC_10k\",\"teplota\":"));
  Serial.print(teplota_c, 2);
  Serial.print(F(",\"odpor_ohm\":"));
  Serial.print(r_ntc, 0);
  Serial.print(F(",\"adc\":"));
  Serial.print(priemer, 0);
  Serial.println(F(",\"jednotka\":\"°C\"}"));
}
