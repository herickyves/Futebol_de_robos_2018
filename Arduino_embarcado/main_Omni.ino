#include "functions_omni_3WD.h"
#include "Initialization.h"

float current_time, previous_time;
float time_interval, inverted_time_interval;
float pulses_sec_encoder1, pulses_sec_encoder2, pulses_sec_encoder3;

void setup() 
{
  Serial.begin(9600);
  Initialize_Motors();
  Initialize_Encoders();
}

void loop() 
{
  int Angle = 60 + 180 ;
  Set_Rotation(Angle);
  Set_Motor(pwm1,m1a,m1b,M[0]);
  Set_Motor(pwm2,m2a,m2b,M[1]);
  Set_Motor(pwm3,m3a,m3b,M[2]);
  Serial.print("Angulo:");
  Serial.println(Angle);
  Serial.println("");
  Verify_Speed();
}

void Verify_Speed()
{
  //calcule the time interval since last verification
  current_time = millis();
  time_interval = (current_time - previous_time) * 0.001;
  inverted_time_interval = 1 / time_interval;

  //calcule pulses per seconde of each encoder
  pulses_sec_encoder1 = pulses1 * inverted_time_interval;
  pulses_sec_encoder2 = pulses2 * inverted_time_interval;
  pulses_sec_encoder3 = pulses3 * inverted_time_interval;
  
  Serial.print("Speed 1 = ");
  Serial.print(pulses_sec_encoder1);
  Serial.print(" Speed 2 = ");
  Serial.print(pulses_sec_encoder2);
  Serial.print(" Speed 3 = ");
  Serial.println(pulses_sec_encoder3);
  pulses1 = 0; 
  pulses2 = 0; 
  pulses3 = 0;
  previous_time = current_time;
}
