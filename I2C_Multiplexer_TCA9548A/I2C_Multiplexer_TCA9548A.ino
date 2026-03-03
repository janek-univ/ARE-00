/*
 * ==========================================================
 *  TCA9548A – I²C Multiplexer (8 kanálov)
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Modul:    TCA9548A I²C Multiplexer
 *  Piny:     A4 (SDA), A5 (SCL)
 *  Adresa:   0x70 (predvolená, nastaviteľná 0x70-0x77)
 *  Výstup:   JSON cez Serial (115200 baud)
 * 
 *  Zapojenie:
 *    TCA9548A VIN -> 5V (alebo 3.3V)
 *    TCA9548A GND -> GND
 *    TCA9548A SDA -> A4
 *    TCA9548A SCL -> A5
 *    TCA9548A A0  -> GND (adresa 0x70)
 *    TCA9548A A1  -> GND
 *    TCA9548A A2  -> GND
 * 
 *    Kanál 0-7: Pripojte I²C senzory na SD0/SC0 až SD7/SC7
 * 
 *  Poznámka:
 *    Umožňuje pripojiť viacero I²C zariadení s rovnakou adresou.
 *    Príklad: 8x BME280 (všetky na 0x76) na jednej zbernici.
 *    Žiadne externé knižnice nie sú potrebné (len Wire.h).
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

#include <Wire.h>

// ---- Konfigurácia ----
#define TCA_ADRESA    0x70       // I²C adresa multiplexera
#define INTERVAL_MS   2000       // Interval skenovania (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie
#define POCET_KANALOV 8          // Počet I²C kanálov

// ---- Globálne premenné ----
unsigned long predchadzajuciCas = 0;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);
  Wire.begin();

  // Overenie pripojenia TCA9548A
  Wire.beginTransmission(TCA_ADRESA);
  uint8_t chyba = Wire.endTransmission();

  if (chyba == 0) {
    Serial.println(F("{\"stav\":\"inicializacia\",\"modul\":\"TCA9548A\",\"adresa\":\"0x70\",\"kanaly\":8}"));
  } else {
    Serial.println(F("{\"modul\":\"TCA9548A\",\"chyba\":\"Multiplexer nebol najdeny na adrese 0x70\"}"));
  }

  // Prvotný sken
  skenujVsetkyKanaly();
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  unsigned long aktualyCas = millis();

  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;
    skenujVsetkyKanaly();
  }
}

// ==========================================================
//  Výber kanálu na multiplexeri
// ==========================================================
void vyberKanal(uint8_t kanal) {
  if (kanal > 7) return;
  Wire.beginTransmission(TCA_ADRESA);
  Wire.write(1 << kanal);
  Wire.endTransmission();
}

// ==========================================================
//  Vypnutie všetkých kanálov
// ==========================================================
void vypniVsetkyKanaly() {
  Wire.beginTransmission(TCA_ADRESA);
  Wire.write(0);
  Wire.endTransmission();
}

// ==========================================================
//  Sken I²C zariadení na všetkých kanáloch
// ==========================================================
void skenujVsetkyKanaly() {
  Serial.println(F("{\"modul\":\"TCA9548A\",\"akcia\":\"sken_start\"}"));

  for (uint8_t kanal = 0; kanal < POCET_KANALOV; kanal++) {
    vyberKanal(kanal);
    delay(10);  // Krátka pauza na prepnutie kanálu

    Serial.print(F("{\"modul\":\"TCA9548A\",\"kanal\":"));
    Serial.print(kanal);
    Serial.print(F(",\"zariadenia\":["));

    bool prveZariadenie = true;

    for (uint8_t adresa = 1; adresa < 127; adresa++) {
      if (adresa == TCA_ADRESA) continue;  // Preskočiť adresu multiplexera

      Wire.beginTransmission(adresa);
      uint8_t chyba = Wire.endTransmission();

      if (chyba == 0) {
        if (!prveZariadenie) Serial.print(',');
        Serial.print(F("\"0x"));
        if (adresa < 16) Serial.print('0');
        Serial.print(adresa, HEX);
        Serial.print('"');
        prveZariadenie = false;
      }
    }

    Serial.println(F("]}"));
  }

  vypniVsetkyKanaly();
  Serial.println(F("{\"modul\":\"TCA9548A\",\"akcia\":\"sken_koniec\"}"));
}
