/*
 * ==========================================================
 *  WS2812B – Ovládanie adresovateľného LED pásu (NeoPixel)
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Modul:    WS2812B LED pás / ring
 *  Pin:      D6 (digitálny)
 *  Výstup:   JSON cez Serial (115200 baud)
 * 
 *  Zapojenie:
 *    WS2812B VCC  -> 5V (externý zdroj pre viac LED)
 *    WS2812B GND  -> GND (spoločná zem s Arduino)
 *    WS2812B DIN  -> D6
 * 
 *  Potrebné knižnice (Arduino Library Manager):
 *    - Adafruit NeoPixel
 * 
 *  Poznámka:
 *    Pre viac ako 8 LED odporúčame externý 5V zdroj.
 *    Pridajte 330Ω rezistor medzi D6 a DIN.
 *    Pridajte 1000µF kondenzátor medzi VCC a GND pri LED páse.
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

#include <Adafruit_NeoPixel.h>

// ---- Konfigurácia ----
#define LED_PIN       6           // Dátový pin
#define POCET_LED     8           // Počet LED na páse
#define INTERVAL_MS   100        // Interval animácie (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie

// ---- Globálne premenné ----
Adafruit_NeoPixel strip(POCET_LED, LED_PIN, NEO_GRB + NEO_KHZ800);
unsigned long predchadzajuciCas = 0;
int aktualnaLed = 0;
uint8_t jas = 50;               // Jas (0-255)

// Farby pre demo efekt (dúha)
uint32_t farby[] = {
  0xFF0000,  // Červená
  0xFF7F00,  // Oranžová
  0xFFFF00,  // Žltá
  0x00FF00,  // Zelená
  0x0000FF,  // Modrá
  0x4B0082,  // Indigo
  0x9400D3,  // Fialová
  0xFF1493   // Ružová
};

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);

  strip.begin();
  strip.setBrightness(jas);
  strip.show();  // Vypnúť všetky LED

  Serial.print(F("{\"stav\":\"inicializacia\",\"modul\":\"WS2812B\",\"pin\":\"D6\",\"pocet_led\":"));
  Serial.print(POCET_LED);
  Serial.println(F("}"));
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  unsigned long aktualyCas = millis();

  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;
    animaciaDuha();
  }
}

// ==========================================================
//  Dúhová animácia (posúvajúce sa farby)
// ==========================================================
void animaciaDuha() {
  for (int i = 0; i < POCET_LED; i++) {
    int farbaIndex = (i + aktualnaLed) % 8;
    strip.setPixelColor(i, farby[farbaIndex]);
  }
  strip.show();

  // JSON výstup
  Serial.print(F("{\"modul\":\"WS2812B\",\"efekt\":\"duha\",\"krok\":"));
  Serial.print(aktualnaLed);
  Serial.print(F(",\"jas\":"));
  Serial.print(jas);
  Serial.println(F("}"));

  aktualnaLed = (aktualnaLed + 1) % 8;
}
