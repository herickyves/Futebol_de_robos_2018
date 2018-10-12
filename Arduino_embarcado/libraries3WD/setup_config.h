void setup_config()
{
	Serial.begin(9600);  
	pinMode(alarm, OUTPUT);

	pinMode(M1_ENA, OUTPUT);
	pinMode(M1_ENB, OUTPUT);
	pinMode(M1_PWM_port, OUTPUT);
	pinMode(encoder60, INPUT);   //motor 60

	pinMode(M2_ENA, OUTPUT);
	pinMode(M2_ENB, OUTPUT);
	pinMode(M2_PWM_port, OUTPUT);
	pinMode(encoder300, INPUT);   //motor 300

	pinMode(M3_ENA, OUTPUT);
	pinMode(M3_ENB, OUTPUT);
	pinMode(M3_PWM_port, OUTPUT);
	pinMode(encoder180, INPUT);   //motor 180


	digitalWrite(alarm,1); 
	radio.begin();
	radio.setDataRate( RF24_2MBPS );
	radio.setPALevel( RF24_PA_HIGH);
	radio.setChannel(0);
	radio.openReadingPipe(1,pipes[0]);
	radio.startListening();
	attachInterrupt(0, interruptFunction, FALLING);

	attachInterrupt(digitalPinToInterrupt(encoder60), speedmotor1 , RISING);   
	attachInterrupt(digitalPinToInterrupt(encoder180), speedmotor2 , RISING);    
	attachInterrupt(digitalPinToInterrupt(encoder300), speedmotor3 , RISING);   
}
