#include <SPI.h>
#include "nRF24L01.h"
#include <RF24.h>
#include <math.h>

//#define inicio 85

#define Possible_Ways 4
//Define bateria utilizada para que o PWM ajuste e não passe da tensao nominal do motor
//Define primeiro caracter da faixa que o robo ira receber
#define Initial_CHAR_Convert_to_INT 65

 const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
RF24 radio(9, 53);
char info;
int num=0;
int A[3] = {60,180,300};
float M[3] = {0,0,0};
int pwm[3] = {6,5,3};

volatile long pulses1 = 0;
volatile long pulses2 = 0;
volatile long pulses3 = 0;

volatile long time1 = 0;
volatile long time2 = 0;
volatile long time3 = 0;

volatile long timeF1 = 0;
volatile long timeF2 = 0;
volatile long timeF3 = 0;

volatile float speedEncoder[3] = {0,0,0};
 
char Received_CHAR;
int Received_INT;

float Angle=360;
float Angle_new=0;

#define m1a 8
#define m1b 7
#define pwm1 6
#define encoder1A 21
#define encoder1B 42

#define m2a 28
#define m2b 26
#define pwm2 5
#define encoder2A 20
#define encoder2B 38

#define m3a 24
#define m3b 4
#define pwm3 3
#define encoder3A 19
#define encoder3B 40

#define alarm A0
int c;
float teta,valor[3]={0,0,0};
int vmax = 100;
int x=0, timex;
int velocidade_total;

void setup() {
  Serial.begin(9600);
  pinMode(alarm, OUTPUT);
  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT);
  pinMode(m2b, OUTPUT);
  pinMode(m3a, OUTPUT);
  pinMode(m3b, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(encoder1B, INPUT);
  pinMode(encoder2B, INPUT);
  pinMode(encoder3B, INPUT);
  pinMode(53,OUTPUT);
  radio.begin();
  radio.setDataRate( RF24_2MBPS );
  radio.setPALevel( RF24_PA_HIGH);
  radio.setChannel(0);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
  //attachInterrupt(0, interruptFunction, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder1A), speedmotor1 , RISING);   
  attachInterrupt(digitalPinToInterrupt(encoder2A), speedmotor2 , RISING);    
  attachInterrupt(digitalPinToInterrupt(encoder3A), speedmotor3 , RISING);  

  analogWrite(pwm[0],0);
  analogWrite(pwm[1],0);
  analogWrite(pwm[2],0);

}

void loop() {
       Angle_new = 180;
       if(Angle_new!=Angle)
        {
          Angle=Angle_new;
          move_o(Angle,210);
  
          valor[0] = M[0];
          valor[1] = M[1];
          valor[2] = M[2];
          Serial.println("Loop:");
          Serial.println(Angle);
          //speedEncoder[0]= 0 ;
          //speedEncoder[1]= 0 ;
          //speedEncoder[2]= 0 ;
        }
 
        verifica(0,m1a,m1b,valor[0]);
        verifica(1,m2a,m2b,valor[1]);
        verifica(2,m3a,m3b,valor[2]);

       
   
}

void verifica(int id_motor,int pa,int pb, float valor)
{
    if(abs((M[id_motor]-speedEncoder[id_motor])/M[id_motor])>0)
    { 
      if(speedEncoder[id_motor] >= 0)
      {
        valor = valor * (abs(M[id_motor])/speedEncoder[id_motor]);
        aciona_motor(id_motor,pa,pb, valor);
      }
      else
      {
        aciona_motor(id_motor,pa,pb,105);
      }
    }
           

}
void move_o(int o,int v)
{    
    for(int i =0; i < 3;i++)
    {
      teta = A[i] - o;
      teta = teta*0.0174533;
      M[i] = v*sin(teta);  
    }
    
}

void aciona_motor(int id_motor,int pa,int pb, int pwm_motor)
{
   float valor_pwm_final;
   if(pwm_motor <210 && pwm_motor > -210)
   {
     if(M[id_motor]<0)
     {
       digitalWrite(pa,0);
       digitalWrite(pb,1);
       valor_pwm_final = map(pwm_motor*(-1),0,210,0,255);  
     }
     else
     {
       digitalWrite(pa,1);
       digitalWrite(pb,0);
       valor_pwm_final = map(pwm_motor,0,210,0,255);
     }
     if(valor_pwm_final>255)valor_pwm_final=255;
     analogWrite(pwm[id_motor],valor_pwm_final);
   }
}
void speedmotor1 ()
{

   pulses1++;
   if (abs(pulses1)>=10)
   {
      timeF1 = micros() - time1;
      speedEncoder[0] = (float(pulses1)/float(time1))*175438.5965;
      pulses1 = 0;
      time1 = micros();
   }
   
}

void speedmotor2 ()
{
  
   pulses2++;
   if (abs(pulses2)>=10){
      timeF2 = micros() - time2;
      speedEncoder[1] = (float(pulses2)/float(time2))*175438.5965;
      pulses2 = 0;
      time2 = micros();    // Initialize initial time again
   }
}

void speedmotor3 (){
   pulses3++;
   if (abs(pulses3)>=10){
      timeF3 = micros() - time3;
      speedEncoder[2] = (float(pulses3)/float(time3))*175438.5965;
      //speedEncoder[2] = ((float)pulses3*0.002924) / ((float)(timeF3) / (60000000.0));
      pulses3 = 0;
      time3 = micros();
   }
}

void parada()
{
    digitalWrite(m1a, 0);
    digitalWrite(m1b, 0);
    digitalWrite(m2a, 0);
    digitalWrite(m2b, 0);
    digitalWrite(m3a, 0);
    digitalWrite(m3b, 0);
    analogWrite(pwm1, 0);
    analogWrite(pwm2, 0);
    analogWrite(pwm3, 0);
}

/*void interruptFunction()
{
  if(radio.available()){
        radio.read(&Received_CHAR, sizeof(Received_CHAR));
        //Received_INT = int(Received_CHAR);
        //Angle_new = ( Received_INT - Initial_CHAR_Convert_to_INT ) * (360 / Possible_Ways);//converte CHAR em INT
        //Serial.print("Recebido: ");
        //Serial.println(Received_CHAR);
        Angle_new = 90;
        if(Angle_new!=Angle)
        {

          Angle=Angle_new;
          move_o(Angle,150);
          valor[0] = M[0];
          valor[1] = M[1];
          valor[2] = M[2];
          // speedEncoder[0]= 0 ;
          //speedEncoder[1]= 0 ;
          //speedEncoder[2]= 0 ;
        }
        
        
        verifica(0,m1a,m1b,valor[0]);
        verifica(1,m2a,m2b,valor[1]);
        verifica(2,m3a,m3b,valor[2]);
   
        
        Serial.print("Velocidade Real 1: ");
        Serial.println(speedEncoder[0]);
        Serial.print("Velocidade Desejada 1: ");
        Serial.println(M[0]); 
        Serial.print("Velocidade Real 2: ");
        Serial.println(speedEncoder[1]);
        Serial.print("Velocidade Desejada 2: ");
        Serial.println(M[1]); 
        Serial.print("Velocidade Real 3: ");
        Serial.println(speedEncoder[2]);
        Serial.print("Velocidade Desejada 3: ");
        Serial.println(M[2]);


      }
    

}*/
