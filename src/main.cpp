#include <Arduino.h>
#include <ELMduino.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

#define SERIAL_BAUD 115200
#define ELM_PORT    SerialBT
#define DEBUG_PORT  Serial
#define LED_PIN     2

const bool DEBUG_MODE     = true;
const int  ELM_TIMEOUT_MS = 1000;
const bool HALT_ON_ERROR  = false;

ELM327 elm;

uint32_t rpm;

void setup() {
  DEBUG_PORT.begin(SERIAL_BAUD);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  ELM_PORT.begin("obdScanner", true);
  DEBUG_PORT.println("Bluetooth device started: obdScanner");

  if(!ELM_PORT.connect("OBDII"))
  {
    DEBUG_PORT.println("Failed to connect to OBDII device - Phase 1");
    while (1);
  }

  if(!elm.begin(ELM_PORT, DEBUG_MODE, ELM_TIMEOUT_MS))
  {
    DEBUG_PORT.println("Failed to connect to OBDII device - Phase 2");
    while (1);
  }

  DEBUG_PORT.println("Connected to OBDII device");
}

void loop() {
  float tempRPM = elm.rpm();

  if (elm.nb_rx_state == ELM_SUCCESS)
  {
    rpm = (uint32_t)tempRPM;
    Serial.print("RPM: "); Serial.println(rpm);
  }
  else if (elm.nb_rx_state != ELM_GETTING_MSG)
    elm.printError();
}