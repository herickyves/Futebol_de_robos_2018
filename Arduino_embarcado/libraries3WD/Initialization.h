#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <math.h>

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
RF24 radio(9, 53);
char info;
int num=0;
#define alarm A0

#define m1a 8
#define m1b 7
#define pwm1 6
#define encoder1A 21
#define m2a 28
#define m2b 26
#define pwm2 5
#define encoder2A 20
#define m3a 24
#define m3b 4
#define pwm3 3
#define encoder3A 19

int pwm[3] = {6,5,3};

void Initialize_Motors();
void Initialize_Encoders();
void Initialize_Radio();
void interruptFunction();

void Initialize_Motors()
{
	pinMode(m1a, OUTPUT);
	pinMode(m1b, OUTPUT);
	pinMode(m2a, OUTPUT);
	pinMode(m2b, OUTPUT);
	pinMode(m3a, OUTPUT);
	pinMode(m3b, OUTPUT);
	pinMode(pwm1, OUTPUT);
	pinMode(pwm2, OUTPUT);
	pinMode(pwm3, OUTPUT);
	analogWrite(pwm[0],0);
	analogWrite(pwm[1],0);
	analogWrite(pwm[2],0);
}

void Initialize_Encoders()
{
	pinMode(encoder1A, INPUT);   //motor 60
	pinMode(encoder2A, INPUT);   //motor 180
	pinMode(encoder3A, INPUT);   //motor 300
	attachInterrupt(digitalPinToInterrupt(encoder1A), speedmotor1 , RISING);   
	attachInterrupt(digitalPinToInterrupt(encoder2A), speedmotor2 , RISING);    
	attachInterrupt(digitalPinToInterrupt(encoder3A), speedmotor3 , RISING);
}

void Initialize_Radio()
{
	pinMode(alarm, OUTPUT);
	digitalWrite(alarm,1);
	radio.begin();
	radio.setDataRate( RF24_2MBPS );
	radio.setPALevel( RF24_PA_HIGH);
	radio.setChannel(0);
	radio.openReadingPipe(1,pipes[0]);
	radio.startListening();
	//attachInterrupt(0, interruptFunction, FALLING);
}
