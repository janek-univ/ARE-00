#include "LedControl.h"

// Define LedControl instance
LedControl lc = LedControl(12, 11, 10, 2); // Pins: DataIn, CLK, LOAD, Number of devices

// Delay time for updates
unsigned long delaytime = 1000; // in ms

const float VCC = 4.9;           // Supply voltage (V)
const float R_fixed = 20000; //19680;     // Fixed resistor value in ohms

// Steinhart-Hart coefficients for thermistor

//const float A = 0.2280094065E-03; 
//const float B = 2.822162716E-04;
//const float C = -0.6024439265E-07;

const float A = 0.0015096695;
const float B = 0.0000998886;
const float C = 0.0000004786;

// Define the number of iterations for averaging ADC readings
const unsigned long number_of_iterations = 32768;

void setup() {
  // Initialize the MAX7219 for both displays
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  lc.shutdown(1, false);
  lc.setIntensity(1, 8);
  lc.clearDisplay(1);

  // Initialize Serial communication for debugging
  Serial.begin(115200);
  Serial.println("Setup complete. Starting...");
}

void loop() {
  // Read the analog value from A0 and average over multiple readings
  unsigned long sumA0 = 0;
  for (unsigned long i = 0; i < number_of_iterations; i++) {
    sumA0 += analogRead(A0); // Accumulate readings
  }
  int analogValue1 = sumA0 / number_of_iterations; // Calculate average

  float voltage_on_diode = (analogValue1 * VCC / 1024.0) * 1000; // Convert to voltage in mV

  // Parse the digits from the analog value
  //int decimal1  = int(voltage_on_diode * 10) % 10; // Thousands digit
  int hundreds  = int(voltage_on_diode / 100) % 10;  // Hundreds digit
  int tens      = int(voltage_on_diode / 10) % 10;   // Tens digit
  int ones      = int(voltage_on_diode) % 10;        // Ones digit
  int decimal1  = int(voltage_on_diode * 10) % 10; // Thousands digit
  
  // Display the parsed digits on the 7-segment display
  lc.setDigit(0, 3, hundreds, false);
  lc.setDigit(0, 2, tens, false);
  lc.setDigit(0, 1, ones, true);
  lc.setDigit(0, 0, decimal1, false);

  // Print the voltage to the Serial Monitor with 4 decimal places
  Serial.print(voltage_on_diode / 1000., 4);

  // Read the analog value from A4 and average over multiple readings
  unsigned long sum = 0;
  for (unsigned long i = 0; i < number_of_iterations; i++) {
    sum += analogRead(A4); // Accumulate readings
    delayMicroseconds(10);
  }
  int analogValue2 = sum / number_of_iterations; // Calculate average

  float voltage_on_thermistor = (analogValue2 * VCC / 1024.0) * 1000; // Convert to voltage in mV

  // Parse the digits from the analog value of A4
  int thousandsA4 = int(voltage_on_thermistor / 1000) % 10; // Thousands digit
  int hundredsA4  = int(voltage_on_thermistor / 100) % 10;  // Hundreds digit
  int tensA4      = int(voltage_on_thermistor / 10) % 10;   // Tens digit
  int onesA4      = int(voltage_on_thermistor) % 10;        // Ones digit

  // Display parsed digits from A4 on the 7-segment display
  lc.setDigit(0, 7, thousandsA4, false);
  lc.setDigit(0, 6, hundredsA4, false);
  lc.setDigit(0, 5, tensA4, false);
  lc.setDigit(0, 4, onesA4, false);

  float V_out = voltage_on_thermistor / 1000.0;

  // Calculate the resistance of the thermistor
  float R_thermistor = (V_out * R_fixed) / (VCC - V_out);
  //float R_thermistor = ((R_fixed * VCC) / V_out) - R_fixed;//(V_out * R_fixed) / (VCC - V_out);

  // Calculate temperature using the Steinhart-Hart equation
  float lnR = log(R_thermistor);
  float T_inv = A + B * lnR + C * lnR * lnR * lnR;
  float T_kelvin = 1.0 / T_inv;
  float T_celsius = T_kelvin - 273.15;

  // Print the temperature to the Serial Monitor
  Serial.print(", ");
  Serial.print(V_out,4);
  Serial.print(", ");
  Serial.print(R_thermistor);
  Serial.print(", ");
  Serial.println(T_celsius);
  

  delay(delaytime);
}