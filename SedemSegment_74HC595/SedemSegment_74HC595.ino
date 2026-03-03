/*
 * ==========================================================
 *  7-Segmentový displej – Ovládanie cez 74HC595
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Displej:  4-miestny 7-segmentový displej (spoločná katóda)
 *  Driver:   74HC595 posuvný register (alebo priame zapojenie)
 *  Piny:     D2(DATA), D3(LATCH), D4(CLOCK) + D5-D8 (digit select)
 *  Výstup:   JSON cez Serial (115200 baud)
 * 
 *  Zapojenie (s 74HC595):
 *    74HC595 VCC   -> 5V
 *    74HC595 GND   -> GND
 *    74HC595 DS    -> D2  (Data / SER)
 *    74HC595 STCP  -> D3  (Latch / RCLK)
 *    74HC595 SHCP  -> D4  (Clock / SRCLK)
 *    74HC595 Q0-Q7 -> Segmenty a-g + dp (cez 220Ω rezistory)
 * 
 *    Digit výber (spoločná katóda):
 *    Digit 1 -> D5
 *    Digit 2 -> D6
 *    Digit 3 -> D7
 *    Digit 4 -> D8
 * 
 *  Poznámka:
 *    Používa multiplexovanie - rýchle prepínanie medzi číslicami.
 *    Žiadne externé knižnice nie sú potrebné.
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

// ---- Konfigurácia pinov (74HC595) ----
#define DATA_PIN      2           // DS (Serial Data)
#define LATCH_PIN     3           // STCP (Latch / Storage Clock)
#define CLOCK_PIN     4           // SHCP (Shift Clock)

// ---- Digit piny (spoločná katóda) ----
#define DIGIT1_PIN    5
#define DIGIT2_PIN    6
#define DIGIT3_PIN    7
#define DIGIT4_PIN    8

#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie
#define INTERVAL_MS   1000       // Interval JSON výstupu (ms)

// ---- Globálne premenné ----
unsigned long predchadzajuciCas = 0;
unsigned long predchadzajuciMultiplex = 0;
int aktualnyDigit = 0;
int zobrazenaHodnota = 0;        // Hodnota na zobrazenie (0-9999)

// Segmentové vzory pre číslice 0-9 (a,b,c,d,e,f,g,dp)
// Bit poradie: dp g f e d c b a
const byte cisla[] = {
  0b00111111,  // 0
  0b00000110,  // 1
  0b01011011,  // 2
  0b01001111,  // 3
  0b01100110,  // 4
  0b01101101,  // 5
  0b01111101,  // 6
  0b00000111,  // 7
  0b01111111,  // 8
  0b01101111   // 9
};

const int digitPiny[] = {DIGIT1_PIN, DIGIT2_PIN, DIGIT3_PIN, DIGIT4_PIN};

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);

  // 74HC595 piny
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  // Digit piny
  for (int i = 0; i < 4; i++) {
    pinMode(digitPiny[i], OUTPUT);
    digitalWrite(digitPiny[i], HIGH);  // Vypnúť (aktívny LOW)
  }

  Serial.println(F("{\"stav\":\"inicializacia\",\"modul\":\"7Segment\",\"driver\":\"74HC595\",\"digity\":4}"));
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  unsigned long aktualyCas = millis();

  // Multiplexovanie displeja (~5ms per digit = ~200Hz refresh)
  if (aktualyCas - predchadzajuciMultiplex >= 5) {
    predchadzajuciMultiplex = aktualyCas;
    zobrazDigit();
  }

  // Aktualizácia hodnoty a JSON výstup
  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;

    // Demo: počítadlo 0-9999
    zobrazenaHodnota = (zobrazenaHodnota + 1) % 10000;

    Serial.print(F("{\"modul\":\"7Segment\",\"hodnota\":"));
    Serial.print(zobrazenaHodnota);
    Serial.println(F("}"));
  }

  // Sériový vstup: pošlite číslo 0-9999
  if (Serial.available() > 0) {
    int novaHodnota = Serial.parseInt();
    if (novaHodnota >= 0 && novaHodnota <= 9999) {
      zobrazenaHodnota = novaHodnota;
      Serial.print(F("{\"modul\":\"7Segment\",\"nastavene\":"));
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
  for (int i = 0; i < 4; i++) {
    digitalWrite(digitPiny[i], HIGH);
  }

  // Extrahovať číslicu pre aktuálny digit
  int hodnota = zobrazenaHodnota;
  int cislica;

  switch (aktualnyDigit) {
    case 0: cislica = (hodnota / 1000) % 10; break;  // Tisícky
    case 1: cislica = (hodnota / 100) % 10;  break;  // Stovky
    case 2: cislica = (hodnota / 10) % 10;   break;  // Desiatky
    case 3: cislica = hodnota % 10;          break;  // Jednotky
    default: cislica = 0;
  }

  // Poslať vzor do 74HC595
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, cisla[cislica]);
  digitalWrite(LATCH_PIN, HIGH);

  // Zapnúť aktuálny digit
  digitalWrite(digitPiny[aktualnyDigit], LOW);

  // Ďalší digit
  aktualnyDigit = (aktualnyDigit + 1) % 4;
}
