# ARE-00 – Arduino Zbierka Sketchov

Zbierka Arduino sketchov pre rôzne senzory, moduly a periférie. Každý modul má samostatný sketch optimalizovaný pre **Arduino Nano** (ATmega328P) s JSON výstupom na Serial Monitor.

## Prehľad modulov

### Senzory

| Modul | Teplota | Vlhkosť | Tlak | Svetlo | Rozhranie |
|-------|:-------:|:-------:|:----:|:------:|-----------|
| DHT11 | ✅ | ✅ | — | — | Digitálny (D2) |
| DHT22 | ✅ | ✅ | — | — | Digitálny (D3) |
| LM35 | ✅ | — | — | — | Analog (A0) |
| DS18B20 | ✅ | — | — | — | 1-Wire (D4) |
| BME280 | ✅ | ✅ | ✅ | — | I²C (0x76) |
| BMP180 | ✅ | — | ✅ | — | I²C (0x77) |
| VEML7700 | — | — | — | ✅ | I²C (0x10) |
| MPU-6050 | ✅* | — | — | — | I²C (0x68) |

> *MPU-6050 meria aj teplotu (interný senzor), primárne je to akcelerometer + gyroskop.

### Aktuátory a moduly

| Modul | Funkcia | Rozhranie |
|-------|---------|-----------|
| Relay Module | Spínanie záťaže | Digitálny (D5) |
| WS2812B | Adresovateľné LED | Digitálny (D6) |
| DC Motor (L298N) | Ovládanie DC motora | PWM (D10) + Digitálny (D11, D12) |
| HC-SR04 | Ultrazvuková vzdialenosť | Digitálny (D7, D8) |
| PIR (HC-SR501) | Detekcia pohybu | Digitálny (D9) |

### Komunikácia a displeje

| Modul | Funkcia | Rozhranie |
|-------|---------|-----------|
| DS3231 RTC | Hodiny reálneho času | I²C (0x68) |
| TCA9548A | I²C Multiplexer (8 kanálov) | I²C (0x70) |
| Nokia 5110 | LCD displej 84x48 px | SPI (D3-D7) |
| 7-Segment (74HC595) | 4-miestny číselný displej | Shift Register (D2-D8) |

## Štruktúra projektu

```
ARE-00/
├── README.md
├── LICENSE
├── .gitignore
├── DHT11_Teplota/
│   └── DHT11_Teplota.ino
├── DHT22_Teplota/
│   └── DHT22_Teplota.ino
├── LM35_Teplota/
│   └── LM35_Teplota.ino
├── DS18B20_Teplota/
│   └── DS18B20_Teplota.ino
├── BME280_Teplota/
│   └── BME280_Teplota.ino
├── BMP180_Teplota/
│   └── BMP180_Teplota.ino
├── VEML7700_Svetlo/
│   └── VEML7700_Svetlo.ino
├── Relay_Modul/
│   └── Relay_Modul.ino
├── WS2812B_LED/
│   └── WS2812B_LED.ino
├── Ultrasonic_HC-SR04/
│   └── Ultrasonic_HC-SR04.ino
├── PIR_Senzor/
│   └── PIR_Senzor.ino
├── DC_Motor_L298N/
│   └── DC_Motor_L298N.ino
├── RTC_DS3231/
│   └── RTC_DS3231.ino
├── MPU6050_IMU/
│   └── MPU6050_IMU.ino
├── I2C_Multiplexer_TCA9548A/
│   └── I2C_Multiplexer_TCA9548A.ino
├── Nokia5110_Displej/
│   └── Nokia5110_Displej.ino
└── SedemSegment_74HC595/
    └── SedemSegment_74HC595.ino
```

## Zapojenie

### DHT11

| Pin DHT11 | Arduino Nano |
|-----------|-------------|
| VCC | 5V |
| GND | GND |
| DATA | D2 |

> ⚡ Vyžaduje **10kΩ pull-up rezistor** medzi DATA a VCC.

### DHT22

| Pin DHT22 | Arduino Nano |
|-----------|-------------|
| VCC | 5V |
| GND | GND |
| DATA | D3 |

> ⚡ Vyžaduje **10kΩ pull-up rezistor** medzi DATA a VCC.

### LM35

| Pin LM35 | Arduino Nano |
|----------|-------------|
| VCC | 5V |
| GND | GND |
| VOUT | A0 |

> Žiadny pull-up rezistor nie je potrebný. Pre dlhšie káble odporúčam 0.1µF kondenzátor pri senzore.

### DS18B20

| Pin DS18B20 | Arduino Nano |
|-------------|-------------|
| VCC | 5V |
| GND | GND |
| DATA | D4 |

> ⚡ Vyžaduje **4.7kΩ pull-up rezistor** medzi DATA a VCC. Podporuje viacero senzorov na jednej zbernici.

### BME280 (I²C)

| Pin BME280 | Arduino Nano |
|------------|-------------|
| VIN | 5V (alebo 3.3V) |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

> I²C adresa: `0x76` (predvolená). Väčšina breakout dosiek má zabudovaný regulátor napätia a level shifter.

### BMP180 (I²C)

| Pin BMP180 | Arduino Nano |
|------------|-------------|
| VIN | 3.3V (alebo 5V ak má doska regulátor) |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

> I²C adresa: `0x77` (fixná). BME280 a BMP180 môžu byť na rovnakej I²C zbernici, pretože majú rôzne adresy.

### VEML7700 (I²C)

| Pin VEML7700 | Arduino Nano |
|--------------|-------------|
| VIN | 3.3V (alebo 5V ak má doska regulátor) |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

> I²C adresa: `0x10` (fixná). Môže byť na rovnakej I²C zbernici s BME280 a BMP180.

### Relay Module

| Pin Relé | Arduino Nano |
|----------|-------------|
| VCC | 5V |
| GND | GND |
| IN1 | D5 |

> Väčšina relé modulov je **aktívna na LOW** (LOW = zapnuté, HIGH = vypnuté).

### WS2812B (NeoPixel)

| Pin WS2812B | Arduino Nano |
|-------------|-------------|
| VCC | 5V (externý zdroj pre viac LED) |
| GND | GND (spoločná zem s Arduino) |
| DIN | D6 |

> Pridajte **330Ω rezistor** medzi D6 a DIN. Pre viac ako 8 LED použite externý 5V zdroj. Pridajte **1000µF kondenzátor** medzi VCC a GND.

### HC-SR04 (Ultrazvukový senzor)

| Pin HC-SR04 | Arduino Nano |
|-------------|-------------|
| VCC | 5V |
| GND | GND |
| TRIG | D7 |
| ECHO | D8 |

> Rozsah: 2–400 cm. Žiadne externé knižnice nie sú potrebné.

### PIR (HC-SR501)

| Pin PIR | Arduino Nano |
|---------|-------------|
| VCC | 5V |
| GND | GND |
| OUT | D9 |

> Po zapnutí senzor potrebuje ~60 sekúnd na kalibráciu. Potenciometre na module: citlivosť a doba držania.

### DC Motor (L298N)

| Pin L298N | Arduino Nano |
|-----------|-------------|
| VCC | Externý zdroj 6-12V |
| GND | GND (spoločná zem) |
| ENA | D10 (PWM) |
| IN1 | D11 |
| IN2 | D12 |
| Motor A | OUT1, OUT2 |

> IN1=HIGH + IN2=LOW = Dopredu. IN1=LOW + IN2=HIGH = Dozadu. ENA riadi rýchlosť cez PWM.

### DS3231 RTC (I²C)

| Pin DS3231 | Arduino Nano |
|------------|-------------|
| VCC | 5V (alebo 3.3V) |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

> I²C adresa: `0x68` (fixná). Záložná batéria CR2032 udržuje čas aj bez napájania. Má zabudovaný teplotný senzor.

### MPU-6050 (I²C)

| Pin MPU-6050 | Arduino Nano |
|--------------|-------------|
| VCC | 5V (alebo 3.3V) |
| GND | GND |
| SDA | A4 |
| SCL | A5 |
| AD0 | GND (adresa 0x68) alebo VCC (adresa 0x69) |

> Predvolená I²C adresa: `0x68`. Ak používate aj DS3231 RTC, pripojte AD0 na VCC pre adresu `0x69`.

### TCA9548A I²C Multiplexer

| Pin TCA9548A | Arduino Nano |
|--------------|-------------|
| VIN | 5V (alebo 3.3V) |
| GND | GND |
| SDA | A4 |
| SCL | A5 |
| A0, A1, A2 | GND (adresa 0x70) |

> 8 nezávislých I²C kanálov. Umožňuje pripojiť viacero zariadení s rovnakou adresou.

### Nokia 5110 (PCD8544)

| Pin Nokia 5110 | Arduino Nano |
|----------------|-------------|
| VCC | 3.3V |
| GND | GND |
| CLK | D3 |
| DIN | D4 |
| DC | D5 |
| CE | D6 |
| RST | D7 |
| BL | 3.3V cez 330Ω |

> Displej pracuje na **3.3V logike**! Pre 5V Arduino použite level shifter. Rozlíšenie: 84×48 pixelov.

### 7-Segmentový displej (74HC595)

| Pin 74HC595 | Arduino Nano |
|-------------|-------------|
| VCC | 5V |
| GND | GND |
| DS (Data) | D2 |
| STCP (Latch) | D3 |
| SHCP (Clock) | D4 |
| Digit 1-4 | D5, D6, D7, D8 |

> 4-miestny displej so spoločnou katódou. Segmenty pripájané cez 220Ω rezistory. Používa multiplexovanie.

## Potrebné knižnice

Všetky knižnice sa dajú nainštalovať cez **Arduino Library Manager** (`Sketch` → `Include Library` → `Manage Libraries...`):

| Knižnica | Používa senzor | Autor |
|----------|----------------|-------|
| DHT sensor library | DHT11, DHT22 | Adafruit |
| Adafruit Unified Sensor | DHT11, DHT22, BME280 | Adafruit |
| OneWire | DS18B20 | Jim Studt |
| DallasTemperature | DS18B20 | Miles Burton |
| Adafruit BME280 Library | BME280 | Adafruit |
| Adafruit BusIO | BME280 (závislosť) | Adafruit |
| Adafruit BMP085 Library | BMP180 | Adafruit |
| Adafruit VEML7700 Library | VEML7700 | Adafruit |
| Adafruit NeoPixel | WS2812B | Adafruit |
| RTClib | DS3231 RTC | Adafruit |
| Adafruit MPU6050 | MPU-6050 | Adafruit |
| Adafruit PCD8544 Nokia 5110 LCD library | Nokia 5110 | Adafruit |
| Adafruit GFX Library | Nokia 5110 (závislosť) | Adafruit |

> **LM35**, **HC-SR04**, **PIR**, **Relay**, **DC Motor**, **TCA9548A** a **7-Segment** nevyžadujú externé knižnice.

## Návod na použitie

1. **Otvorte Arduino IDE**
2. **Nainštalujte potrebné knižnice** (viď tabuľka vyššie)
3. **Otvorte sketch** – `File` → `Open` → vyberte `.ino` súbor z príslušného priečinka
4. **Nastavte dosku** – `Tools` → `Board` → `Arduino Nano`
5. **Nastavte procesor** – `Tools` → `Processor` → `ATmega328P` (alebo `ATmega328P (Old Bootloader)`)
6. **Vyberte port** – `Tools` → `Port` → vyberte COM port vášho Arduina
7. **Nahrajte sketch** – kliknite na `Upload` (→)
8. **Otvorte Serial Monitor** – `Tools` → `Serial Monitor`, nastavte **115200 baud**

## Výstupný formát (JSON)

Každý sketch vypisuje dáta vo formáte JSON na sériový port (115200 baud) každé 2 sekundy.

### DHT11 / DHT22

```json
{"senzor":"DHT11","teplota":23.50,"vlhkost":45.20,"jednotka":"°C"}
```

### LM35

```json
{"senzor":"LM35","teplota":23.50,"jednotka":"°C"}
```

### DS18B20

```json
{"senzor":"DS18B20","index":0,"adresa":"28FF1A2B3C4D5E6F","teplota":23.50,"jednotka":"°C"}
```

### BME280

```json
{"senzor":"BME280","teplota":23.50,"vlhkost":45.20,"tlak":1013.25,"jednotka":"°C"}
```

### BMP180

```json
{"senzor":"BMP180","teplota":23.50,"tlak":1013.25,"jednotka":"°C"}
```

### VEML7700

```json
{"senzor":"VEML7700","lux":254.30,"biele_svetlo":312.00,"als":298.00,"jednotka":"lx"}
```

### Relay Module

```json
{"modul":"Relay","stav":"zapnute","pin":5}
```

### WS2812B

```json
{"modul":"WS2812B","efekt":"duha","krok":3,"jas":50}
```

### HC-SR04 (Ultrasonic)

```json
{"senzor":"HC-SR04","vzdialenost":25.43,"jednotka":"cm"}
```

### PIR

```json
{"senzor":"PIR","pohyb":true,"pocet_detekcii":5,"cas_ms":12345}
```

### DC Motor

```json
{"modul":"DC_Motor","smer":"dopredu","rychlost":150}
```

### DS3231 RTC

```json
{"modul":"DS3231_RTC","datum":"2026-03-03","cas":"14:30:25","den":"Utorok","teplota":23.50}
```

### MPU-6050

```json
{"senzor":"MPU-6050","accel":{"x":0.12,"y":-0.05,"z":9.78,"jednotka":"m/s2"},"gyro":{"x":0.01,"y":-0.02,"z":0.00,"jednotka":"rad/s"},"teplota":24.50}
```

### TCA9548A

```json
{"modul":"TCA9548A","kanal":0,"zariadenia":["0x76","0x77"]}
```

### Nokia 5110

```json
{"modul":"Nokia_5110","citac":42,"uptime_s":120,"volna_ram":1234}
```

### 7-Segment

```json
{"modul":"7Segment","hodnota":1234}
```

### Chybový výstup (príklad)

```json
{"senzor":"DHT11","chyba":"Nepodarilo sa precitat data zo senzora"}
{"senzor":"BME280","chyba":"Senzor nebol najdeny. Skontrolujte zapojenie a I2C adresu."}
```

## Hlavné vlastnosti kódu

- **Non-blocking** – použitie `millis()` namiesto `delay()` v hlavnej slučke
- **Kontrola chýb** – overenie pripojenia senzora a validity dát
- **JSON výstup** – štruktúrovaný výstup, ľahko parsovateľný ďalšími programami
- **Úspora pamäte** – použitie makra `F()` pre reťazce uložené vo Flash pamäti
- **Konfigurovateľné** – piny, interval a baud rate sú definované cez `#define`

## Hardvér

- **Doska:** Arduino Nano (ATmega328P) – kompatibilné aj s Arduino Uno a inými AVR doskami
- **Napájanie:** USB alebo externý zdroj 5V
- **Senzory:** Bežne dostupné moduly z e-shopov (AliExpress, GME, Laskakit, RPishop)

## Licencia

Tento projekt je licencovaný pod [MIT licenciou](LICENSE).
