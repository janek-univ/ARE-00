/*
 * ==========================================================
 *  Relay Module – Ovládanie relé modulu
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Modul:    1/2/4-kanálový relé modul (aktívny LOW)
 *  Pin:      D5 (digitálny)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 5 sekúnd (non-blocking, millis())
 * 
 *  Zapojenie:
 *    Relé VCC -> 5V
 *    Relé GND -> GND
 *    Relé IN1 -> D5
 * 
 *  Poznámka:
 *    Väčšina relé modulov je aktívna na LOW (LOW = zapnuté).
 *    Žiadne externé knižnice nie sú potrebné.
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

// ---- Konfigurácia ----
#define RELAY_PIN     5           // Pin relé modulu
#define INTERVAL_MS   5000       // Interval prepínania (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie

// ---- Globálne premenné ----
unsigned long predchadzajuciCas = 0;
bool releZapnute = false;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Relé vypnuté (aktívny LOW)

  Serial.println(F("{\"stav\":\"inicializacia\",\"modul\":\"Relay\",\"pin\":\"D5\",\"logika\":\"active_low\"}"));
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  unsigned long aktualyCas = millis();

  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;
    prepniRele();
  }

  // Sériový príkaz na ovládanie: pošlite '1' alebo '0'
  if (Serial.available() > 0) {
    char prikaz = Serial.read();
    if (prikaz == '1') {
      releZapnute = true;
      digitalWrite(RELAY_PIN, LOW);  // Zapnúť
      vypisStav();
    } else if (prikaz == '0') {
      releZapnute = false;
      digitalWrite(RELAY_PIN, HIGH); // Vypnúť
      vypisStav();
    }
  }
}

// ==========================================================
//  Prepnutie relé
// ==========================================================
void prepniRele() {
  releZapnute = !releZapnute;

  if (releZapnute) {
    digitalWrite(RELAY_PIN, LOW);   // Zapnúť (aktívny LOW)
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Vypnúť
  }

  vypisStav();
}

// ==========================================================
//  Výpis stavu v JSON
// ==========================================================
void vypisStav() {
  Serial.print(F("{\"modul\":\"Relay\",\"stav\":\""));
  Serial.print(releZapnute ? F("zapnute") : F("vypnute"));
  Serial.print(F("\",\"pin\":"));
  Serial.print(RELAY_PIN);
  Serial.println(F("}"));
}
