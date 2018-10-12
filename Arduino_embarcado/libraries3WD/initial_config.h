//define em quantas direcoes diferentes o robo podera ir = 360/PossibleWays
#define Possible_Ways 32
//Define bateria utilizada para que o PWM ajuste e não passe da tensao nominal do motor
#define Battery 12
//Define primeiro caracter da faixa que o robo ira receber
#define Initial_CHAR_Convert_to_INT 65

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <math.h>
#define inicio 85

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
RF24 radio(9, 53);
char info;
int num=0;

//Motor_1 ou 60
#define M1_ENA 8
#define M1_ENB 7
#define M1_PWM_port 6
#define encoder60 19

//motor_2 ou 300
#define M2_ENA A4
#define M2_ENB 4
#define M2_PWM_port 3
#define encoder300 21

//Motor_3 ou 180
#define M3_ENA A2
#define M3_ENB A3
#define M3_PWM_port 5
#define encoder180 20

#define alarm A0

int Received_INT = 0;
int M1_PWM = 0;
int M2_PWM = 0;
int M3_PWM = 0;
int M1_S = 0;
int M2_S = 0;
int M3_S = 0;

char Received_CHAR;

volatile long pulses1 = 0;
volatile long pulses2 = 0;
volatile long pulses3 = 0;

volatile long time1 = 0;
volatile long time2 = 0;
volatile long time3 = 0;
volatile long timeF1 = 0;
volatile long timeF2 = 0;
volatile long timeF3 = 0;

volatile float speed1 = 0;
volatile float speed2 = 0;
volatile float speed3 = 0;

float v60, v180, v300;

void interruptFunction();
void speedmotor1();
void speedmotor2();
void speedmotor3();
