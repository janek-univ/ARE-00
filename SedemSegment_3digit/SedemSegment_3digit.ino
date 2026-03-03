/*
 * ==========================================================
 *  7-Segmentový displej – 3-miestny (priame zapojenie)
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Displej:  3-miestny 7-segmentový displej (spoločná katóda)
 *  Piny:     D2-D8 (segmenty a-g), D9-D11 (digit select)
 *  Výstup:   JSON cez Serial (115200 baud)
 * 
 *  Zapojenie (priame, bez shift registra):
 *    Segment a -> D2 (cez 220Ω rezistor)
 *    Segment b -> D3 (cez 220Ω rezistor)
 *    Segment c -> D4 (cez 220Ω rezistor)
 *    Segment d -> D5 (cez 220Ω rezistor)
 *    Segment e -> D6 (cez 220Ω rezistor)
 *    Segment f -> D7 (cez 220Ω rezistor)
 *    Segment g -> D8 (cez 220Ω rezistor)
 * 
 *    Digit 1 (stovky)  -> D9
 *    Digit 2 (desiatky) -> D10
 *    Digit 3 (jednotky) -> D11
 * 
 *  Poznámka:
 *    Spoločná katóda: digit pin LOW = zapnutý
 *    Segment HIGH = svietiaci segment
 *    Každý segment cez 220Ω rezistor!
 *    Žiadne externé knižnice nie sú potrebné.
 *    Rozsah zobrazenia: 0-999
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

// ---- Konfigurácia pinov (segmenty a-g) ----
const int segmentPiny[] = {2, 3, 4, 5, 6, 7, 8};  // a, b, c, d, e, f, g
#define POCET_SEGMENTOV 7

// ---- Digit piny (spoločná katóda) ----
const int digitPiny[] = {9, 10, 11};  // Digit 1 (stovky), 2 (desiatky), 3 (jednotky)
#define POCET_DIGITOV   3

#define BAUD_RATE       115200     // Rýchlosť sériovej komunikácie
#define INTERVAL_MS     1000       // Interval JSON výstupu (ms)

// ---- Globálne premenné ----
unsigned long predchadzajuciCas = 0;
unsigned long predchadzajuciMultiplex = 0;
int aktualnyDigit = 0;
int zobrazenaHodnota = 0;           // Hodnota na zobrazenie (0-999)

// Segmentové vzory pre číslice 0-9
//                              a  b  c  d  e  f  g
const bool cisla[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1},  // 2
  {1, 1, 1, 1, 0, 0, 1},  // 3
  {0, 1, 1, 0, 0, 1, 1},  // 4
  {1, 0, 1, 1, 0, 1, 1},  // 5
  {1, 0, 1, 1, 1, 1, 1},  // 6
  {1, 1, 1, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1},  // 8
  {1, 1, 1, 1, 0, 1, 1}   // 9
};

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);

  // Nastavenie pinov segmentov ako výstup
  for (int i = 0; i < POCET_SEGMENTOV; i++) {
    pinMode(segmentPiny[i], OUTPUT);
    digitalWrite(segmentPiny[i], LOW);
  }

  // Nastavenie pinov digitov ako výstup
  for (int i = 0; i < POCET_DIGITOV; i++) {
    pinMode(digitPiny[i], OUTPUT);
    digitalWrite(digitPiny[i], HIGH);  // Vypnúť (aktívny LOW pre spol. katódu)
  }

  Serial.println(F("{\"stav\":\"inicializacia\",\"modul\":\"7Segment_3digit\",\"digity\":3,\"rozsah\":\"0-999\"}"));
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  unsigned long aktualyCas = millis();

  // Multiplexovanie displeja (~5ms per digit = ~66Hz refresh)
  if (aktualyCas - predchadzajuciMultiplex >= 5) {
    predchadzajuciMultiplex = aktualyCas;
    zobrazDigit();
  }

  // Aktualizácia hodnoty a JSON výstup
  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;

    // Demo: počítadlo 0-999
    zobrazenaHodnota = (zobrazenaHodnota + 1) % 1000;

    Serial.print(F("{\"modul\":\"7Segment_3digit\",\"hodnota\":"));
    Serial.print(zobrazenaHodnota);
    Serial.println(F("}"));
  }

  // Sériový vstup: pošlite číslo 0-999
  if (Serial.available() > 0) {
    int novaHodnota = Serial.parseInt();
    if (novaHodnota >= 0 && novaHodnota <= 999) {
      zobrazenaHodnota = novaHodnota;
      Serial.print(F("{\"modul\":\"7Segment_3digit\",\"nastavene\":"));
      Serial.print(zobrazenaHodnota);
      Serial.println(F("}"));
    }
  }
}

// ==========================================================
//  Zobrazenie jedného digitu (multiplexovanie)
// ==========================================================
void zobrazDigit() {
  // Vypnúť všetky digity
  for (int i = 0; i < POCET_DIGITOV; i++) {
    digitalWrite(digitPiny[i], HIGH);
  }

  // Extrahovať číslicu pre aktuálny digit
  int cislica;
  switch (aktualnyDigit) {
    case 0: cislica = (zobrazenaHodnota / 100) % 10; break;  // Stovky
    case 1: cislica = (zobrazenaHodnota / 10) % 10;  break;  // Desiatky
    case 2: cislica = zobrazenaHodnota % 10;          break;  // Jednotky
    default: cislica = 0;
  }

  // Nastaviť segmenty
  for (int i = 0; i < POCET_SEGMENTOV; i++) {
    digitalWrite(segmentPiny[i], cisla[cislica][i] ? HIGH : LOW);
  }

  // Zapnúť aktuálny digit (LOW pre spoločnú katódu)
  digitalWrite(digitPiny[aktualnyDigit], LOW);

  // Ďalší digit
  aktualnyDigit = (aktualnyDigit + 1) % POCET_DIGITOV;
}
