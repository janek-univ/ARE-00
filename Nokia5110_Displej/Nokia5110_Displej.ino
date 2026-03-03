/*
 * ==========================================================
 *  Nokia 5110 – PCD8544 LCD displej (84x48 pixelov)
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Displej:  Nokia 5110 (PCD8544)
 *  Piny:     D3(CLK), D4(DIN), D5(DC), D6(CE), D7(RST)
 *  Výstup:   JSON cez Serial (115200 baud)
 * 
 *  Zapojenie:
 *    Nokia 5110 VCC  -> 3.3V
 *    Nokia 5110 GND  -> GND
 *    Nokia 5110 CLK  -> D3 (SCLK)
 *    Nokia 5110 DIN  -> D4 (MOSI)
 *    Nokia 5110 DC   -> D5 (Data/Command)
 *    Nokia 5110 CE   -> D6 (Chip Enable)
 *    Nokia 5110 RST  -> D7 (Reset)
 *    Nokia 5110 BL   -> 3.3V cez 330Ω rezistor (podsvietenie)
 * 
 *  Potrebné knižnice (Arduino Library Manager):
 *    - Adafruit PCD8544 Nokia 5110 LCD library
 *    - Adafruit GFX Library
 * 
 *  Poznámka:
 *    Displej pracuje na 3.3V logike! Ak používate 5V Arduino,
 *    použite level shifter alebo napäťové deliče na dátových pinoch.
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// ---- Konfigurácia pinov ----
#define CLK_PIN       3           // SCLK
#define DIN_PIN       4           // MOSI (Data In)
#define DC_PIN        5           // Data/Command
#define CE_PIN        6           // Chip Enable
#define RST_PIN       7           // Reset

#define INTERVAL_MS   1000       // Interval aktualizácie (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie
#define KONTRAST      50         // Kontrast displeja (0-127)

// ---- Globálne premenné ----
Adafruit_PCD8544 displej = Adafruit_PCD8544(CLK_PIN, DIN_PIN, DC_PIN, CE_PIN, RST_PIN);
unsigned long predchadzajuciCas = 0;
unsigned long pocitadlo = 0;

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);

  displej.begin();
  displej.setContrast(KONTRAST);
  displej.clearDisplay();

  // Uvítacia obrazovka
  displej.setTextSize(1);
  displej.setTextColor(BLACK);
  displej.setCursor(0, 0);
  displej.println(F("ARE-00"));
  displej.println(F("Nokia 5110"));
  displej.println(F("----------"));
  displej.println(F("84x48 px"));
  displej.println(F("PCD8544"));
  displej.display();

  Serial.println(F("{\"stav\":\"inicializacia\",\"modul\":\"Nokia_5110\",\"rozlisenie\":\"84x48\",\"kontrast\":50}"));
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  unsigned long aktualyCas = millis();

  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;
    aktualizujDisplej();
  }
}

// ==========================================================
//  Aktualizácia displeja
// ==========================================================
void aktualizujDisplej() {
  pocitadlo++;

  displej.clearDisplay();
  displej.setTextSize(1);
  displej.setTextColor(BLACK);

  // Riadok 1: Názov
  displej.setCursor(0, 0);
  displej.println(F("ARE-00 Demo"));

  // Riadok 2: Čítač
  displej.print(F("Citac: "));
  displej.println(pocitadlo);

  // Riadok 3: Uptime
  unsigned long sekundy = millis() / 1000;
  displej.print(F("Uptime: "));
  displej.print(sekundy);
  displej.println(F("s"));

  // Riadok 4: Voľná RAM
  displej.print(F("RAM: "));
  displej.print(volnaRAM());
  displej.println(F(" B"));

  displej.display();

  // JSON výstup
  Serial.print(F("{\"modul\":\"Nokia_5110\",\"citac\":"));
  Serial.print(pocitadlo);
  Serial.print(F(",\"uptime_s\":"));
  Serial.print(sekundy);
  Serial.print(F(",\"volna_ram\":"));
  Serial.print(volnaRAM());
  Serial.println(F("}"));
}

// ==========================================================
//  Výpočet voľnej RAM
// ==========================================================
int volnaRAM() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}
