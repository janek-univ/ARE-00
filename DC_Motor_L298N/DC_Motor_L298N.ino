/*
 * ==========================================================
 *  DC Motor – Ovládanie DC motora cez L298N / L293D
 * ==========================================================
 *  Doska:    Arduino Nano (ATmega328P)
 *  Driver:   L298N (alebo L293D)
 *  Piny:     D10 (ENA/PWM), D11 (IN1), D12 (IN2)
 *  Výstup:   JSON cez Serial (115200 baud)
 * 
 *  Zapojenie (L298N):
 *    L298N VCC   -> Externý zdroj 6-12V
 *    L298N GND   -> GND (spoločná zem s Arduino)
 *    L298N 5V    -> Môže napájať Arduino (ak jumper je zapojený)
 *    L298N ENA   -> D10 (PWM pre riadenie rýchlosti)
 *    L298N IN1   -> D11
 *    L298N IN2   -> D12
 *    Motor A     -> OUT1, OUT2
 * 
 *  Smer otáčania:
 *    IN1=HIGH, IN2=LOW  -> Dopredu
 *    IN1=LOW,  IN2=HIGH -> Dozadu
 *    IN1=LOW,  IN2=LOW  -> Stop
 * 
 *  Poznámka:
 *    Žiadne externé knižnice nie sú potrebné.
 * 
 *  Autor:  Marian Janek
 *  Dátum:  2026-03-03
 * ==========================================================
 */

// ---- Konfigurácia ----
#define ENA_PIN       10          // PWM pin (rýchlosť)
#define IN1_PIN       11          // Smer 1
#define IN2_PIN       12          // Smer 2
#define INTERVAL_MS   3000       // Interval zmeny smeru (ms)
#define BAUD_RATE     115200     // Rýchlosť sériovej komunikácie

// ---- Globálne premenné ----
unsigned long predchadzajuciCas = 0;
int faza = 0;                    // 0=dopredu, 1=stop, 2=dozadu, 3=stop
uint8_t rychlost = 150;         // PWM rýchlosť (0-255)

// ==========================================================
//  SETUP
// ==========================================================
void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  // Počiatočný stav: stop
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, 0);

  Serial.println(F("{\"stav\":\"inicializacia\",\"modul\":\"DC_Motor\",\"driver\":\"L298N\",\"piny\":\"ENA=D10,IN1=D11,IN2=D12\"}"));
}

// ==========================================================
//  LOOP (non-blocking)
// ==========================================================
void loop() {
  unsigned long aktualyCas = millis();

  if (aktualyCas - predchadzajuciCas >= INTERVAL_MS) {
    predchadzajuciCas = aktualyCas;
    dalsiKrok();
  }

  // Sériový príkaz: 'f'=dopredu, 'b'=dozadu, 's'=stop, '0'-'9'=rýchlosť
  if (Serial.available() > 0) {
    char prikaz = Serial.read();
    switch (prikaz) {
      case 'f': motorDopredu(rychlost); break;
      case 'b': motorDozadu(rychlost);  break;
      case 's': motorStop();            break;
      default:
        if (prikaz >= '0' && prikaz <= '9') {
          rychlost = map(prikaz - '0', 0, 9, 0, 255);
          analogWrite(ENA_PIN, rychlost);
          Serial.print(F("{\"modul\":\"DC_Motor\",\"rychlost\":"));
          Serial.print(rychlost);
          Serial.println(F("}"));
        }
        break;
    }
  }
}

// ==========================================================
//  Demo sekvencia
// ==========================================================
void dalsiKrok() {
  switch (faza) {
    case 0: motorDopredu(rychlost); break;
    case 1: motorStop();            break;
    case 2: motorDozadu(rychlost);  break;
    case 3: motorStop();            break;
  }
  faza = (faza + 1) % 4;
}

// ==========================================================
//  Funkcie motora
// ==========================================================
void motorDopredu(uint8_t pwm) {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, pwm);

  Serial.print(F("{\"modul\":\"DC_Motor\",\"smer\":\"dopredu\",\"rychlost\":"));
  Serial.print(pwm);
  Serial.println(F("}"));
}

void motorDozadu(uint8_t pwm) {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  analogWrite(ENA_PIN, pwm);

  Serial.print(F("{\"modul\":\"DC_Motor\",\"smer\":\"dozadu\",\"rychlost\":"));
  Serial.print(pwm);
  Serial.println(F("}"));
}

void motorStop() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, 0);

  Serial.println(F("{\"modul\":\"DC_Motor\",\"smer\":\"stop\",\"rychlost\":0}"));
}
