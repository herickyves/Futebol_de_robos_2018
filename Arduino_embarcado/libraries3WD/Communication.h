//Define primeiro caracter da faixa que o robo ira receber
#define Initial_CHAR_Convert_to_INT 65

char Received_CHAR;
int Received_INT;

int Receive_Command();

int Receive_Command()
{
	//Recebe Caracter via RF
	radio.read(&Received_CHAR, sizeof(Received_CHAR));
	Received_INT = int(Received_CHAR); //converte CHAR em INT
	Serial.println("received INT");
	Serial.println(Received_INT);
	return Received_INT;
}
