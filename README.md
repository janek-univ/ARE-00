# ARE-00 – Arduino Meranie Teploty a Okolitých Veličín

Zbierka Arduino sketchov pre meranie teploty, vlhkosti, tlaku a intenzity svetla so 7 rôznymi senzormi. Každý senzor má samostatný sketch optimalizovaný pre **Arduino Nano** (ATmega328P).

## Prehľad senzorov

| Senzor | Teplota | Vlhkosť | Tlak | Svetlo | Typ | Rozhranie |
|--------|:-------:|:-------:|:----:|:------:|-----|-----------|
| DHT11 | ✅ | ✅ | — | — | Digitálny | 1-Wire (D2) |
| DHT22 | ✅ | ✅ | — | — | Digitálny | 1-Wire (D3) |
| LM35 | ✅ | — | — | — | Analógový | Analog (A0) |
| DS18B20 | ✅ | — | — | — | Digitálny | 1-Wire (D4) |
| BME280 | ✅ | ✅ | ✅ | — | Digitálny | I²C (A4/A5) |
| BMP180 | ✅ | — | ✅ | — | Digitálny | I²C (A4/A5) |
| VEML7700 | — | — | — | ✅ | Digitálny | I²C (A4/A5) |

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
└── VEML7700_Svetlo/
    └── VEML7700_Svetlo.ino
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

> **LM35** nevyžaduje žiadnu externú knižnicu – používa len `analogRead()`.

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
