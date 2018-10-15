#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(9, 10);
int i=0;
char nome;
char vai = 90;

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup(){
  radio.begin();
  radio.setDataRate( RF24_2MBPS );
  radio.setPALevel( RF24_PA_HIGH);
  radio.setChannel(0);
  radio.openWritingPipe(pipes[0]);
  radio.stopListening();
  Serial.begin(9600);
}

void loop()
{
  while(0){
    radio.write(&vai, sizeof(vai));
    delay(100);}
  }

