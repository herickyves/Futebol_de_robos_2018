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
#define encoder3A 1

int pwm[3] = {6,5,3};

void Initialize_Motors();
void Initialize_Encoders();

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
  attachInterrupt(digitalPinToInterrupt(encoder1A), speedmotor1 , RISING);   
  attachInterrupt(digitalPinToInterrupt(encoder2A), speedmotor2 , RISING);    
  attachInterrupt(digitalPinToInterrupt(encoder3A), speedmotor3 , RISING);
}
