#include "drive_functions_3WD.h"
#include "verification_functions_3WD.h"
#include "Initialization.h"
#include "Communication.h"

void setup() 
{
  Serial.begin(9600);
  Initialize_Motors();
  Initialize_Encoders();
  Initialize_Radio();
}

void loop() 
{
  int Angle = 180 ;
  Set_Rotation(Angle);
  Set_Motor(pwm1,m1a,m1b,M[0]);
  Set_Motor(pwm2,m2a,m2b,M[1]);
  Set_Motor(pwm3,m3a,m3b,M[2]);
  Serial.print("Angulo:");
  Serial.println(Angle);
  Serial.println("");
  Verify_Speed();
  Correct_Rotation();
  Set_Motor(pwm1,m1a,m1b,PWM_New[0]);
  Set_Motor(pwm2,m2a,m2b,PWM_New[1]);
  Set_Motor(pwm3,m3a,m3b,PWM_New[2]);
}

void Correct_Rotation()
{
  Find_Higher_Speed();
  Compare_Actual_Speed_to_Expected();
  Serial.print(" Corrigido 1 = ");
  Serial.print(PWM_New[0]);
  Serial.print(" Corrigido 2 = ");
  Serial.print(PWM_New[1]);
  Serial.print(" Corrigido 3 = ");
  Serial.println(PWM_New[2]); 
}

void Compare_Actual_Speed_to_Expected()
{
  if(Higher_Speed_ID == 1)
  {
   if(Encoder_pps2 != 0)
    PWM_New[1] = M[1] *  ((Higher_Speed*M[1]) / (M[0]*Encoder_pps2));
   else
    PWM_New[1] = M[1];
   if(Encoder_pps3 != 0)
    PWM_New[2] = M[2] *  ((Higher_Speed*M[2]) / (M[0]*Encoder_pps3));
   else
    PWM_New[2] = M[2];
  }
  else if(Higher_Speed_ID == 2)
  {
   if(Encoder_pps1 != 0)
    PWM_New[0] = M[0] *  ((Higher_Speed*M[0]) / (M[1]*Encoder_pps1));
   else
    PWM_New[0] = M[0];
   if(Encoder_pps3 != 0)
    PWM_New[2] = M[2] *  ((Higher_Speed*M[2]) / (M[1]*Encoder_pps3));
   else
    PWM_New[2] = M[2];
  }
  else if(Higher_Speed_ID == 3)
  {
   if(Encoder_pps1 != 0)
   PWM_New[0] = M[0] *  ((Higher_Speed*M[0]) / (M[2]*Encoder_pps1));
   else
   PWM_New[0] = M[0];
   if(Encoder_pps2 != 0)
   PWM_New[1] = M[1] *  ((Higher_Speed*M[1]) / (M[2]*Encoder_pps2));
   else
   PWM_New[1] = M[1];
  }
}
