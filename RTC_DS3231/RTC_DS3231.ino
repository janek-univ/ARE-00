/*
 * ==========================================================
 *  DS3231 RTC – Hodiny reálneho času
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Modul:    DS3231 RTC (I²C)
 *  Piny:     A4 (SDA), A5 (SCL)
 *  Adresa:   0x68 (fixná)
 *  Výstup:   JSON cez Serial (115200 baud)
 *  Interval: 1 sekunda (non-blocking, millis())
 * 
 *  Zapojenie:
 *    DS3231 VCC -> 5V (alebo 3.3V)
 *    DS3231 GND -> GND
 *    DS3231 SDA -> A4
 *    DS3231 SCL -> A5
 * 
 *  Potrebné knižnice (Arduino Library Manager):
 *    - RTClib (Adafruit)
 * 
 *  Poznámka:
 *    DS3231 má zabudovaný teplotne kompenzovaný oscilátor.
 *    Záložná batéria CR2032 udržuje čas aj bez napájania.
 *    Pri prvom spustení nastavte čas odkomentovaním riadku v setup().
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

#include <Wire.h>
#include <RTClib.h>

// ---- Konfigurácia ----
#define INTERVAL_MS   1000       // Interval výpisu (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie

// ---- Globálne premenné ----
RTC_DS3231 rtc;
unsigned long predchadzajuciCas = 0;
bool rtcOk = false;

// Názvy dní v slovenčine
const char* dni[] = {"Nedela", "Pondelok", "Utorok", "Streda", "Stvrtok", "Piatok", "Sobota"};

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);

  rtcOk = rtc.begin();

  if (rtcOk) {
    Serial.println(F("{\"stav\":\"inicializacia\",\"modul\":\"DS3231_RTC\",\"adresa\":\"0x68\"}"));

    // Ak RTC stratil napájanie, nastavte čas na čas kompilácie:
    if (rtc.lostPower()) {
      Serial.println(F("{\"modul\":\"DS3231_RTC\",\"poznamka\":\"Stratene napajanie - nastavujem cas\"}"));
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // Manuálne nastavenie času (odkomentujte a upravte):
    // rtc.adjust(DateTime(2026, 3, 3, 12, 0, 0));
  } else {
    Serial.println(F("{\"modul\":\"DS3231_RTC\",\"chyba\":\"RTC modul nebol najdeny\"}"));
  }
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  if (!rtcOk) return;

  unsigned long aktualyCas = millis();

  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;
    vypisCas();
  }
}

// ==========================================================
//  Výpis času v JSON
// ==========================================================
void vypisCas() {
  DateTime teraz = rtc.now();
  float teplota = rtc.getTemperature();  // DS3231 má zabudovaný teplotný senzor

  // JSON výstup
  Serial.print(F("{\"modul\":\"DS3231_RTC\",\"datum\":\""));
  Serial.print(teraz.year());
  Serial.print('-');
  if (teraz.month() < 10) Serial.print('0');
  Serial.print(teraz.month());
  Serial.print('-');
  if (teraz.day() < 10) Serial.print('0');
  Serial.print(teraz.day());
  Serial.print(F("\",\"cas\":\""));
  if (teraz.hour() < 10) Serial.print('0');
  Serial.print(teraz.hour());
  Serial.print(':');
  if (teraz.minute() < 10) Serial.print('0');
  Serial.print(teraz.minute());
  Serial.print(':');
  if (teraz.second() < 10) Serial.print('0');
  Serial.print(teraz.second());
  Serial.print(F("\",\"den\":\""));
  Serial.print(dni[teraz.dayOfTheWeek()]);
  Serial.print(F("\",\"teplota\":"));
  Serial.print(teplota, 2);
  Serial.println(F("}"));
}
