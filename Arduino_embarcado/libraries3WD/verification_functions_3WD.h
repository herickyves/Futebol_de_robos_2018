float current_time, previous_time;
float time_interval, inverted_time_interval;
float Encoder_pps1, Encoder_pps2, Encoder_pps3;
float Higher_Speed;
int Higher_Speed_ID;

void Verify_Speed();
void Find_Higher_Speed();
void Compare_Actual_Speed_to_Expected();
void speedmotor1();
void speedmotor2();
void speedmotor3();

void Verify_Speed()
{
	//calcule the time interval since last verification
	current_time = millis();
	time_interval = (current_time - previous_time) * 0.001;
	inverted_time_interval = 1 / time_interval;

	//calcule pulses per seconde of each encoder
	Encoder_pps1 = pulses1 * inverted_time_interval;
	Encoder_pps2 = pulses2 * inverted_time_interval;
	Encoder_pps3 = pulses3 * inverted_time_interval;

	Serial.print("Speed 1 = ");
	Serial.print(Encoder_pps1);
	Serial.print(" Speed 2 = ");
	Serial.print(Encoder_pps2);
	Serial.print(" Speed 3 = ");
	Serial.println(Encoder_pps3);

	//restart the score
	pulses1 = 0; 
	pulses2 = 0; 
	pulses3 = 0;
	previous_time = current_time;
}

void Find_Higher_Speed()
{
	Higher_Speed = Encoder_pps1;
	Higher_Speed_ID = 1;
	if(Encoder_pps2 > Encoder_pps1)
	{
		Higher_Speed = Encoder_pps2;
		Higher_Speed_ID = 2;
	}
	if(Encoder_pps3 > Encoder_pps2)
	{
		Higher_Speed = Encoder_pps3;
		Higher_Speed_ID = 3;
	}
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
