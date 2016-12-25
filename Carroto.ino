#include <SoftwareSerial.h>
#include <Stepper.h>
//Definicao de pinos do motor esquerdo
#define pinE_A1 8 //Roxo
#define pinE_A2 9 //Cinza
#define pinE_A3 10 //Branco
#define pinE_A4 11 //Preto
//Definicao de pinos do motor direito
#define pinD_A1 2 //Amarelo
#define pinD_A2 3 //Laranja
#define pinD_A3 4 //Vermelho
#define pinD_A4 5 //Marrom
//Definicao de pinos do BT HC-05
#define pinBT_TX 12
#define pinBT_RX 13
//Definicao da relacao steps/cm
#define stepsCM 87
#define stepsPA 17 //17 steps para girar um grau com uma roda pivo.

#define entreEixos 13 //Distancia entre-eixos, em cm.

Stepper motorE(32, pinE_A1, pinE_A3, pinE_A2, pinE_A4);
Stepper motorD(32, pinD_A1, pinD_A3, pinD_A2, pinD_A4);
SoftwareSerial sBT(pinBT_TX, pinBT_RX);

//Vai para a frente: motorE negativo, motorD positivo.
//Comprimento da roda: 27cm
//Uma volta = 2048 steps. 
//1cm = stepsCM
//string de entrada: Mxxx (M o tipo de movimento e xxx dist�ncia ou angulo)

void setup() {
	sBT.begin(9600);
  Serial.begin(9600);
}

void loop() {
	char ch = 'P';
	int dist = 0;

  motorE.setSpeed(800);
  motorD.setSpeed(800);

	if (sBT.available())
	{
		ch = sBT.read();
		if (ch == 'F' || ch == 'T')
		{
			 dist = sBT.parseInt();
			 carrotoAndaReto(ch, dist);
		}
		if (ch == 'D' || ch == 'E' || ch == 'd' || ch == 'e')
		{
			dist = sBT.parseInt();
			carrotoAndaCurva(ch, dist);
		}
		if (ch == 'C' || ch == 'c')
		{
			dist = sBT.parseInt();
			carrotoAndaCirculo (ch, dist);
		}
   if (ch == 'R' || ch == 'r')
   {
      dist = sBT.parseInt();
      carrotoRotaciona(ch, dist);
   }
   sBT.flush();
	}
 carrotoAndaCirculo("C", 20);
}

void carrotoAndaReto(char idenMovimento, int distMovimento)
{
	//Algoritmo para movimentacao para frente
	int direcME = 1, direcMD = 0;
	int stepsToDo = 0;

	if (idenMovimento == 'F')
	{
		direcME = -1;
		direcMD = 1;
	}
	if (idenMovimento == 'T')
	{
		direcME = 1;
		direcMD = -1;
	}
	if (idenMovimento != 'F'&'T')
		return;


	//Executa o movimento, laco valido para movimento Frente e Tras
	int i = 0, j = 0;
	//1cm  = 76 steps. Valores inteiros, sempre.
	stepsToDo = stepsCM * distMovimento;
	if (idenMovimento == 'F' || idenMovimento == 'T')
	{
		while (i <= stepsToDo && j <= stepsToDo)
		{
			motorE.step(1 * direcME);
			motorD.step(1 * direcMD);
			i++;
			j++;
		}
		//Seta todos os pinos dos motores em LOW para impedir uso desnecessario de corrente.
		digitalWrite(pinE_A1, LOW);
		digitalWrite(pinE_A2, LOW);
		digitalWrite(pinE_A3, LOW);
		digitalWrite(pinE_A4, LOW);
		digitalWrite(pinD_A1, LOW);
		digitalWrite(pinD_A2, LOW);
		digitalWrite(pinD_A3, LOW);
		digitalWrite(pinD_A4, LOW);
			
		}
}

void carrotoAndaCurva(char idenMovimento, int angle)
{
	int steps;

	steps = angle * stepsPA;
	if (idenMovimento == 'E')
	{
		motorD.step(steps);
	}
	if (idenMovimento == 'D')
	{
		motorE.step(-steps);
	}
	if (idenMovimento == 'e')
		motorD.step(steps * -1);
	if (idenMovimento == 'd')
		motorE.step(steps);
	if (idenMovimento != 'D'&'E'&'d'&'e')
		return;

	digitalWrite(pinE_A1, LOW);
	digitalWrite(pinE_A2, LOW);
	digitalWrite(pinE_A3, LOW);
	digitalWrite(pinE_A4, LOW);
	digitalWrite(pinD_A1, LOW);
	digitalWrite(pinD_A2, LOW);
	digitalWrite(pinD_A3, LOW);
	digitalWrite(pinD_A4, LOW);
}

void carrotoRotaciona (int idenMovimento, int angle)
{
  int steps = 0;
  steps = (stepsPA * angle)/2;
  if (idenMovimento == 'R') //Sentido horario
  {
    for(int i = 1; i <= steps; i++)
    {
      motorE.step(-1);
      motorD.step(-1);
    }
  }
  
  if (idenMovimento == 'r') //Sentido anti-horario
  {
    for(int i = 1; i <= steps; i++)
    {
      motorE.step(1);
      motorD.step(1);
    }
  }

  digitalWrite(pinE_A1, LOW);
  digitalWrite(pinE_A2, LOW);
  digitalWrite(pinE_A3, LOW);
  digitalWrite(pinE_A4, LOW);
  digitalWrite(pinD_A1, LOW);
  digitalWrite(pinD_A2, LOW);
  digitalWrite(pinD_A3, LOW);
  digitalWrite(pinD_A4, LOW);
  
}

void carrotoAndaCirculo(char idenMovimento, int raio)
{
  int steps, i = 0;
  float per1, per2, step1, step2, total;

  per1 = 2 * 3.14 * (raio);
  total = per1 * 87;
  Serial.println(per1);
  per2 = (2 * 3.14 * (raio - entreEixos));
  Serial.println(per2);

  per1= ((per1 /per2)*87)/10;
  Serial.println(per1);

  per2 = (per2*87)/10;
  Serial.println(per2);
  
  if (idenMovimento == 'C')
  {
    if (raio <=13){
      motorD.step(-1);
    }
    else{ 
      while (i < total){
      motorE.setSpeed(800);
      motorD.setSpeed(800);
      motorE.step(int(per2));
      motorD.step(-int(per1));
      i = i + per1;
      }
    }
  }


 /* while(true)
  {
    carrotoAndaReto('F', 001);
    carrotoRotaciona('R', 001);
  }
  */
  digitalWrite(pinE_A1, LOW);
  digitalWrite(pinE_A2, LOW);
  digitalWrite(pinE_A3, LOW);
  digitalWrite(pinE_A4, LOW);
  digitalWrite(pinD_A1, LOW);
  digitalWrite(pinD_A2, LOW);
  digitalWrite(pinD_A3, LOW);
  digitalWrite(pinD_A4, LOW);
}

