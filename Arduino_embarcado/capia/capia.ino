#include <PID_v1.h>

#include <TimerOne.h>

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
double M[3] = {0,0,0};
double Mabs[3] = {0,0,0};
int pwm[3] = {6,5,3};
int i=0;

volatile long pulses[3] = {0,0,0};
volatile long timeE[3] = {0,0,0};
volatile long timeF[3] = {0,0,0};
double speedE[3] = {0,0,0};
double speedO[3] = {0,0,0};
 
char Received_CHAR;
int Received_INT;

float Angle=360;
float Angle_new=0;

double Kp = 1;                                // PID proportional control Gain
double Kd = 0; 
double Ki = 1; 



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
double teta,valor[3]={0,0,0};
float vmax = 210;
int modo=0, timex;
int velocidade_total;
double valor_pwm_final[3]={0,0,0};
double valor_pwm[3]={0,0,0};
float w=0;
float w_new=0;

PID PID60(&speedE[0], &valor[0], &Mabs[0],Kp,Ki,Kd, DIRECT);
PID PID180(&speedE[1], &valor[1], &Mabs[1],Kp,Ki,Kd, DIRECT);
PID PID300(&speedE[2], &valor[2], &Mabs[2],Kp,Ki,Kd, DIRECT);

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
  //radio.begin();
  //radio.setDataRate( RF24_2MBPS );
  //radio.setPALevel( RF24_PA_HIGH);
  //radio.setChannel(0);
  //radio.openReadingPipe(1,pipes[0]);
  //radio.startListening();
  //attachInterrupt(0, interruptFunction, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder1A), speedmotor1 , RISING);   
  attachInterrupt(digitalPinToInterrupt(encoder2A), speedmotor2 , RISING);    
  attachInterrupt(digitalPinToInterrupt(encoder3A), speedmotor3 , RISING); 

  PID60.SetMode(AUTOMATIC);
  PID180.SetMode(AUTOMATIC);
  PID300.SetMode(AUTOMATIC);

  PID60.SetOutputLimits(0, 210);
  PID180.SetOutputLimits(0, 210);
  PID300.SetOutputLimits(0, 210);

  Timer1.initialize(500000); // Inicializa o Timer1 e configura para um período de 0,50 segundo
  Timer1.attachInterrupt(zeravelocidade); // Configura a função zeravelocidade() como a função para ser chamada a cada interrupção do Timer1


  analogWrite(pwm[0],0);
  analogWrite(pwm[1],0);
  analogWrite(pwm[2],0);

}

void loop() {
     modo=1;
     if(modo==0){ //Modo parada
       aciona_motor(0,m1a,m1b,0);
       aciona_motor(1,m2a,m2b,0);
       aciona_motor(2,m3a,m3b,0);
     }
     else if(modo==1){ // Modo movimento linear
       Angle_new = 20;
       while(Angle_new!=Angle)//só executa uma vez quando o comando muda para evitar que o valor do PWM seja reescrito pelo valor teórico toda vez que o robô receba um comando
       {
          Angle=Angle_new;
          move_o(Angle,vmax);
  
          valor[0] = abs(M[0]);
          valor[1] = abs(M[1]);
          valor[2] = abs(M[2]);
         
          Serial.println("Loop:");
          Serial.println(Angle);
          
          while(speedE[0]!=0 || speedE[1]!=0 || speedE[2]!=0){
            aciona_motor(0,m1a,m1b,0);
            aciona_motor(1,m2a,m2b,0);
            aciona_motor(2,m3a,m3b,0);
           // velocidade(0);
           // velocidade(1);
           // velocidade(2);
            delay(500);
          }
        }
        PID60.Compute();
        PID180.Compute();
        PID300.Compute();
        verifica(0,m1a,m1b);
        verifica(1,m2a,m2b);
        verifica(2,m3a,m3b);
        //aciona_motor(0,m1a,m1b,valor[0]);
        //aciona_motor(1,m2a,m2b,valor[1]);
        //aciona_motor(2,m3a,m3b,valor[2]);
       // velocidade(0);
       // velocidade(1);
       // velocidade(2);
        
        Serial.print("V1 SpeedE:");
        Serial.println(speedE[0]);
        Serial.print("V1 M:");
        Serial.println(M[0]);
        Serial.print("V1 valor:");
        Serial.println(valor[0]);
        Serial.println(" ");
        Serial.print("V2 SpeedE:");
        Serial.println(speedE[1]);
        Serial.print("V2 M:");
        Serial.println(M[1]);
        Serial.print("V2 valor:");
        Serial.println(valor[1]);
        Serial.println(" ");
        Serial.print("V3 SpeedE:");
        Serial.println(speedE[2]);
        Serial.print("V3 M:");
        Serial.println(M[2]);
        Serial.print("V3 valor:");
        Serial.println(valor[2]);
        Serial.println(" ");

        
        //delay(100);
     }
     else if (modo==2){ //Modo movimento angular
       
       w_new = 0.25;
       while(w_new!=w)//só executa uma vez quando o comando muda para evitar que o valor do PWM seja reescrito pelo valor teórico toda vez que o robô receba um comando
       {
          w=w_new;
          turn_o(w,vmax);
  
          valor[0] = abs(M[0]);
          valor[1] = abs(M[1]);
          valor[2] = abs(M[2]);
         
          Serial.println("W:");
          Serial.println(w);
          
          while(speedE[0]!=0 || speedE[1]!=0 || speedE[2]!=0){
            aciona_motor(0,m1a,m1b,0);
            aciona_motor(1,m2a,m2b,0);
            aciona_motor(2,m3a,m3b,0);
           // velocidade(0);
           // velocidade(1);
           // velocidade(2);
            delay(500);
          }
       }
       
       verifica(0,m1a,m1b);
       verifica(1,m2a,m2b);
       verifica(2,m3a,m3b);

       delay(100);
     }
}

void verifica(int id_motor,int pa,int pb)//Verifica e corrigi a velocidade
{
  //if(abs((abs(M[id_motor])-speedE[id_motor])/M[id_motor])>0)//Checa a diferença entre a velocidade do encoder e a velocidade desejada
        //{ 
          //if(speedE[id_motor] != 0)
          //{ 
            //valor[id_motor] = valor[id_motor] * ((abs(M[id_motor])/speedE[id_motor]));//corrigi o pwm para alcançar a velocidade desejada 
            aciona_motor(id_motor,pa,pb, valor[id_motor]);
            //if(valor[id_motor]>210)valor[id_motor]=210;// Certifica-se que a velocidade corrigida não passe de 210rpm
          //}
          //else
          //{
            //if(modo==1)
              //aciona_motor(id_motor,pa,pb,150);//tranco se velocidade for 0 
            //else
              //aciona_motor(id_motor,pa,pb,100);
          //}
        //}
        //delay(1);
        //else{aciona_motor(id_motor,pa,pb,valor);}
}
void turn_o(float w,int vmax){
  for(i=0;i<3;i++){
         M[i]=w*vmax;
  }
}
void move_o(int o,int v)
{    
    for(int i =0; i < 3;i++)
    {
      teta = A[i] - o;
      teta = (teta*PI)/180;
      M[i] = v*sin(teta);
      Mabs[i] = abs(M[i]);  
      Serial.print(i);
      Serial.print(" : ");
      Serial.println(M[i]);
    }
    
}
void aciona_motor(int id_motor,int pa,int pb, int velocidade_motor)
{
   //if(velocidade_motor <210)
   //{
     if(M[id_motor]<0) // Gira o motor em um sentido
     {
       digitalWrite(pa,0);
       digitalWrite(pb,1);
       //valor_pwm_final[id_motor] = map(velocidade_motor,0,vmax,0,255);
       valor_pwm_final[id_motor]=(velocidade_motor*255)/210;  //faz regra de três
     }
     else //gira o motor no outro sentido
     {
       digitalWrite(pa,1);
       digitalWrite(pb,0);
       //valor_pwm_final[id_motor] = map(velocidade_motor,0,vmax,0,255);
       valor_pwm_final[id_motor]=(velocidade_motor*255)/210; 
     }
 

     
     Serial.print(id_motor);
     Serial.print(" : ");
     Serial.println(valor_pwm_final[id_motor]);
     if(valor_pwm_final[id_motor]>255)valor_pwm_final[id_motor]=255; // certifica-se que o valor pwm não passe de 255
     analogWrite(pwm[id_motor],valor_pwm_final[id_motor]);
     //analogWrite(pwm[id_motor],255);
   //}
}

////////////////////// FUNÇÕES DE INTERRUPÇÃO /////////////////////////////////////
void speedmotor1 (){
     pulses[0]++;
   if (abs(pulses[0])>=10 ||valor[0]==0){ //para não ter que recalcular a velocidade a cada pulso do encoder e para ter uma velocidade média mais homôgenea
      timeF[0] = micros() - timeE[0];
      speedE[0] = ((float)pulses[0]/342) / ((float)(timeF[0]) / (60000000.0));
      //if(M[0]<0)speed1=-speed1;
      pulses[0] = 0;
      timeE[0] = micros();// Initialize initial time again
   }
   
}

void speedmotor2 (){
    pulses[1]++;
   if (abs(pulses[1])>=10 || valor[1]==0){
      timeF[1] = micros() - timeE[1];
      speedE[1] = ((float)pulses[1]/342) / ((float)(timeF[1]) / (60000000.0));
      //if(M[1]<0)speed2=-speed2;
      pulses[1] = 0;
      timeE[1] = micros();    // Initialize initial time again
   }
}

void speedmotor3 (){
   pulses[2]++;
   if (abs(pulses[2])>=10 || valor[2]==0){
      timeF[2] = micros() - timeE[2];
      speedE[2] = ((float)pulses[2]/342) / ((float)(timeF[2]) / (60000000.0));
      //if(M[2]<0)speed3=-speed3;
      pulses[2] = 0;
      timeE[2] = micros();    // Initialize initial time again
   }
}
void zeravelocidade(){//zera a velocidade se passar 1 segundo sem ela mudar 
   for(i=0;i<3;i++){
    if(speedE[i]==speedO[i])
      speedE[i]=0;
    else
      speedO[i]=speedE[i];
   }
}
/*void interruptFunction()
{
  if(radio.available()){
        detachInterrupt(0);
        radio.read(&Received_CHAR, sizeof(Received_CHAR));
        Received_INT = int(Received_CHAR);
        Angle_new = ( Received_INT - Initial_CHAR_Convert_to_INT ) * (360 / Possible_Ways);//converte CHAR em INT
        Serial.print("Recebido: ");
        Serial.println(Received_CHAR);
        attachInterrupt(0, interruptFunction, FALLING);
        /*if(Received_INT == 98)
        {
          velocidade_total = 210;
          Angle_new = 0;
        }
        else
        {
          velocidade_total = 150;
          Angle_new = 180;
        }
        if(Angle_new!=Angle)
        {
          Angle=Angle_new;
          move_o(Angle,210);
          Serial.print("Angulo");
          Serial.println(Angle);
  
          valor[0] = M[0];
          valor[1] = M[1];
          valor[2] = M[2];
          Serial.print("M1 = ");
          Serial.println(M[0]);
          Serial.print("M2 = ");
          Serial.println(M[1]);
          Serial.print("M3 = ");
          Serial.println(M[2]);
          speedE[0]= 0 ;
          speedE[1]= 0 ;
          speedE[2]= 0 ;
        }
        
        
        verifica(0,m1a,m1b,valor[0]);
        verifica(1,m2a,m2b,valor[1]);
        verifica(2,m3a,m3b,valor[2]);
    

          
        //Angle_new = ( Received_INT - Initial_CHAR_Convert_to_INT ) * (360 / Possible_Ways);
        //timex = millis();
        //Angle_new = 45;

        //if(M[0]==0)speedE[0]=0;
        //if(M[1]==0)speedE[1]=0;
        //if(M[2]==0)speedE[2]=0;
        
  

       
       /*Serial.print("Velocidade Real 1: ");
        Serial.println(speedE[0]);
        Serial.print("Velocidade Desejada 1: ");
        Serial.println(M[0]); 
        Serial.print("Velocidade Real 2: ");
        Serial.println(speedE[1]);
        Serial.print("Velocidade Desejada 2: ");
        Serial.println(M[1]); 
        Serial.print("Velocidade Real 3: ");
        Serial.println(speedE[2]);
        Serial.print("Velocidade Desejada 3: ");
        Serial.println(M[2]);*/
     /* }
    

}
void speedmotor1 (){
   //if(digitalRead(encoder1B)==LOW)
    //pulses1--;
   //else
     pulses1++;
   if (abs(pulses1)>=10){ //para não ter que recalcular a velocidade a cada pulso do encoder e para ter uma velocidade média mais homôgenea
      timeF1 = micros() - time1;
      speedE[0] = ((float)pulses1/342) / ((float)(timeF1) / (60000000.0));
      //if(M[0]<0)speed1=-speed1;
      pulses1 = 0;
      time1 = micros();// Initialize initial time again
   }
   
}

void speedmotor2 (){
   //if(digitalRead(encoder2B)==LOW)
    pulses2++;
   //else
    //pulses2--;
   if (abs(pulses2)>=10){
      timeF2 = micros() - time2;
      speedE[1] = ((float)pulses2/342) / ((float)(timeF2) / (60000000.0));
      //if(M[1]<0)speed2=-speed2;
      pulses2 = 0;
      time2 = micros();    // Initialize initial time again
   }
}

void speedmotor3 (){
   //if(digitalRead(encoder3B)==LOW)
    pulses3++;
   //else
    //pulses3--;
   if (abs(pulses3)>=10){
      timeF3 = micros() - time3;
      speedE[2] = ((float)pulses3/342) / ((float)(timeF3) / (60000000.0));
      //if(M[2]<0)speed3=-speed3;
      pulses3 = 0;
      time3 = micros();    // Initialize initial time again
   }
}
*/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////









        /*if(abs((M[0]-speed1)/M[0])>=0.1)
        { 
          if(speed1 != 0)
          {
            valor[0] = valor[0] * (M[0]/speed1);
            aciona_motor(0,m1a,m1b, valor[0]);
          }
          else
          {
            aciona_motor(0,m1a,m1b,127);
          }
        }
        if(abs(abs(M[1])-speed2) >= M[1]*0.1)
        { 
          if(speed2 != 0)
          {
            valor[1] = valor[1] * (M[1]/speed2);
            aciona_motor(1,m2a,m2b, valor[1]);
          }
          else
          {
            aciona_motor(1,m2a,m2b,127);
          }
        }
        if(abs(abs(M[2])-speed3) >= M[2]*0.1)
        { 
          if(speed3 != 0)
          {
            valor[2] = valor[2] * (M[2]/speed3);
            aciona_motor(2,m3a,m3b, valor[2]);
          }
          else
          {
            aciona_motor(2,m3a,m3b,127);
          }
  
        }*/

             //}
      //do
      //{
 //while(millis()-timex<=4000){

        //}
        //x++;
        //if(x==6)x=0;
      //}while(abs(abs(M[0])-speed1) >= M[0]*0.1|| abs(abs(M[1])-speed2) >= M[1]*0.1 || abs(abs(M[2])-speed3) >= M[2]*0.1);

              //aciona_motor(0,m1a,m1b,M[0]);
        //aciona_motor(1,m2a,m2b,M[1]);
        //aciona_motor(2,m3a,m3b,M[2]);
