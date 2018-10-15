#include "drive_functions_3WD.h"
#include "verification_functions_3WD.h"
#include "Initialization.h"
#include "Communication.h"

int Angle = 90 ;

void setup() 
{
  Serial.begin(9600);
  Initialize_Motors();
  Initialize_Encoders();
  Initialize_Radio();
  Set_Rotation(Angle);
  Set_Motor(pwm1,m1a,m1b,M[0]);
  Set_Motor(pwm2,m2a,m2b,M[1]);
  Set_Motor(pwm3,m3a,m3b,M[2]);
}

void loop() 
{

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

void interruptFunction()
{
  if(radio.available())
  {
    int Received_Angle = Receive_Command();
    Set_Rotation(Received_Angle);
  }
}

