int Receive_Command()
{
  //Recebe Caracter via RF
  radio.read(&Received_CHAR, sizeof(Received_CHAR));
  Received_INT = int(Received_CHAR); //converte CHAR em INT
}

void Set_Rotation()
{
  int Angle = ( Received_INT - Initial_CHAR_Convert_to_INT ) * (360 / Possible_Ways);

  int theta1 = 0.0174533 * (150 - Angle);
  M1_PWM = abs(sin(theta1));
  if (sin(theta1)>=0)M1_S=0;
  else M1_S=1;
  
  int theta2 = 0.0174533 * (30 - Angle);
  M2_PWM = abs(sin(theta2));
  if (sin(theta2)>=0)M2_S=0;
  else M2_S=1;
  
  int theta3 = 0.0174533 * (270 - Angle);
  M3_PWM = abs(sin(theta3));
  if (sin(theta3)>=0)M3_S=0;
  else M3_S=1;
}

void Set_Motors()
{
  analogWrite(M1_PWM_port, 255* M1_PWM);
  analogWrite(M2_PWM_port, 255* M2_PWM);
  analogWrite(M3_PWM_port, 255* M3_PWM);
  
  if(M1_S==0){			//Define sentidos de rotaçao
    digitalWrite(M1_ENA, HIGH);
    digitalWrite(M1_ENB, LOW);
  }
  else{
    digitalWrite(M1_ENB, HIGH);
    digitalWrite(M1_ENA, LOW);
  }
  if(M2_S==0){
    digitalWrite(M2_ENA, HIGH);
    digitalWrite(M2_ENB, LOW);
  }
  else{
    digitalWrite(M2_ENB, HIGH);
    digitalWrite(M2_ENA, LOW);
  }
  if(M3_S==0){
    digitalWrite(M3_ENA, HIGH);
    digitalWrite(M3_ENB, LOW);
  }
  else{
    digitalWrite(M3_ENB, HIGH);
    digitalWrite(M3_ENA, LOW);
}
  
}

void Verify_Speed()
{
    //float v60 = M1_PWM;
    //float v180 = M3_PWM;
    //float v300 = M2_PWM;
}

void Correct_Rotation(){
  if(abs((v60-speed1)/v60)>=0.1){
     M1_PWM =2* M1_PWM - (speed1/210);
  }
  if(abs((v300-speed2)/v300)>=0.1){
     M3_PWM =2* M3_PWM - (speed2/210);
  }
  if(abs((v180-speed3)/v180)>=0.1){
     M2_PWM =2* M2_PWM - (speed3/210);
  }
  if(M1_PWM>255)M1_PWM=255;
  if(M2_PWM>255)M2_PWM=255;
  if(M3_PWM>255)M3_PWM=255;
}

void speedmotor1 (){
   pulses1++;
   if (pulses1>=10){
      timeF1 = micros() - time1;
      speed1 = ((float)pulses1/342) / ((float)(timeF1) / (60000000.0));
      pulses1 = 0;
      time1 = micros();    // Initialize initial time again
   }
}

void speedmotor2 (){
   pulses2++;
   if (pulses2>=10){
      timeF2 = micros() - time2;
      speed2 = ((float)pulses2/342) / ((float)(timeF2) / (60000000.0));
      pulses2 = 0;
      time2 = micros();    // Initialize initial time again
   }
}

void speedmotor3 (){
   pulses3++;
   if (pulses3>=10){
      timeF3 = micros() - time3;
      speed3 = ((float)pulses3/342) / ((float)(timeF3) / (60000000.0));
      pulses3 = 0;
      time3 = micros();    // Initialize initial time again
   }
}
