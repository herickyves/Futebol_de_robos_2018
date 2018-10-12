volatile long pulses1 = 0;
volatile long pulses2 = 0;
volatile long pulses3 = 0;
float M[3] = {0,0,0};
float theta;
int A[3] = {60,180,300};

void Set_Motor(int id_motor,int EN_A,int EN_B, int pwm_motor);
void speedmotor1();
void speedmotor2();
void speedmotor3();

void Set_Rotation(int Received_Angle)
{    
    for(int i =0; i < 3;i++)
    {
      //Calcule the power for each Motor
      theta = A[i] - Received_Angle;
      theta = theta * 0.0174533;
      M[i] = 255 * sin(theta);  
    }
    Serial.print("Motor 1 = ");
    Serial.print(M[0]);
    Serial.print("Motor 2 = ");
    Serial.print(M[1]);
    Serial.print("Motor 3 = ");
    Serial.print(M[2]);  
}

void Set_Motor(int id_motor,int EN_A,int EN_B, int pwm_motor)
{
  if(pwm_motor < 0)
  {
    //In case of negative PWM
    pwm_motor = pwm_motor * -1;
    digitalWrite(EN_A,0);
    digitalWrite(EN_B,1);
  }
  else
  {
    digitalWrite(EN_A,1);
    digitalWrite(EN_B,0);
  }
  
  analogWrite(id_motor, pwm_motor) ;
}

void speedmotor1()
{
  pulses1++;
}

void speedmotor2()
{
  pulses2++;
}

void speedmotor3()
{
  pulses3++;
}
