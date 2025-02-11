#include <SPI.h>
#include <LoRa.h>

#ifdef ARDUINO_SAMD_MKRWAN1300
#error "This example is not compatible with the Arduino MKR WAN 1300 board!"
#endif

int packetReceived = 0;  // keeps track of packetReceived for onReceiveCallback

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver Callback");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Uncomment the next line to disable the default AGC and set LNA gain, values between 1 - 6 are supported
  // LoRa.setGain(6);
  
  // register the receive callback
  // onReceive calls interrupt internally. should run only tiny amount of code
  // any heavylifting should handled in the loop
  // also, should not contain any timer based delay calls
  LoRa.onReceive([](int packetLength) { packetReceived = packetLength; });

  // put the radio into receive mode
  LoRa.receive();
}

void loop() {
  if (packetReceived) onReceive();
}

void onReceive() {
  // received a packet
  Serial.print("Received packet '");

  // read packet
  for (int i = 0; i < packetReceived; i++) {
    Serial.print((char)LoRa.read());
  }

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());

  // reset packetReceived so that onReceive only runs once per packet
  packetReceived = 0;
}
