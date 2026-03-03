/*
 * ==========================================================
 *  HC-SR04 – Ultrazvukový senzor vzdialenosti
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Senzor:   HC-SR04
 *  Piny:     D7 (TRIG), D8 (ECHO)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 500 ms (non-blocking, millis())
 * 
 *  Zapojenie:
 *    HC-SR04 VCC  -> 5V
 *    HC-SR04 GND  -> GND
 *    HC-SR04 TRIG -> D7
 *    HC-SR04 ECHO -> D8
 * 
 *  Poznámka:
 *    Rozsah: 2 cm – 400 cm
 *    Žiadne externé knižnice nie sú potrebné.
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

// ---- Konfigurácia ----
#define TRIG_PIN      7           // Trigger pin
#define ECHO_PIN      8           // Echo pin
#define INTERVAL_MS   500        // Interval merania (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie
#define RYCHLOST_ZVUKU 0.0343   // cm/µs (pri ~20°C)

// ---- Globálne premenné ----
unsigned long predchadzajuciCas = 0;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println(F("{\"stav\":\"inicializacia\",\"senzor\":\"HC-SR04\",\"trig\":\"D7\",\"echo\":\"D8\"}"));
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  unsigned long aktualyCas = millis();

  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;
    merajVzdialenost();
  }
}

// ==========================================================
//  Meranie vzdialenosti a výpis JSON
// ==========================================================
void merajVzdialenost() {
  // Vyslanie ultrazvukového pulzu
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Meranie doby odrazu
  long trvanie = pulseIn(ECHO_PIN, HIGH, 30000);  // Timeout 30ms (~5m)

  if (trvanie == 0) {
    Serial.println(F("{\"senzor\":\"HC-SR04\",\"chyba\":\"Ziaden odraz - objekt mimo dosahu\"}"));
    return;
  }

  // Výpočet vzdialenosti
  float vzdialenostCm = (trvanie * RYCHLOST_ZVUKU) / 2.0;

  // Kontrola rozsahu
  if (vzdialenostCm < 2.0 || vzdialenostCm > 400.0) {
    Serial.println(F("{\"senzor\":\"HC-SR04\",\"chyba\":\"Hodnota mimo rozsahu (2-400 cm)\"}"));
    return;
  }

  // JSON výstup
  Serial.print(F("{\"senzor\":\"HC-SR04\",\"vzdialenost\":"));
  Serial.print(vzdialenostCm, 2);
  Serial.println(F(",\"jednotka\":\"cm\"}"));
}
