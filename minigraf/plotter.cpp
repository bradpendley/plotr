// 
// 
// 

#include "plotter.h"
#include "debugging.h"
Point currentPos = {0, 0};

/*
This is Czech description of Minigraf Aritma connector:

�pi�ka
konektoru   
minigrafu	barva
��ly
kabelu   	sign�l
sch�matu   
minigrafu	logiok� z�t�   
sign�lu
v minigrafu	v�znam
12				nepou�ito
11	r	0V		spojen� zem� minigrafu a po��ta�e
10	�	X1	5	nejni��� bin�rn� ��d sou�adnice X
9	m	X2	5	prost�edn� bin�rn� ��d sou�adnice X
8	b	X4	5	nejvy��� bin�rn� ��d sou�adnice X
7				kl��
6				rezerva
5	r	P	5	P=H pis�tko spu�t�no, P=L pis�tko zvednuto
4	b	Y1	5	nejni��� bin�rn� ��d sou�adnice Y
3	m	Y2	5	prost�edn� bin�rn� ��d sou�adnice Y
2	�	Y4	5	nejvy��� bin�rn� ��d sou�adnice Y
1				spojen� zem� minigrafu a po��ta�e


*/

const int X1pin=7; 
const int X2pin=6; 
const int X4pin=5;

const int Y1pin=4; 
const int Y2pin=3; 
const int Y4pin=2; 
const int PENpin = 12; 

const int MAXX=1520;
const int MAXY=2160;
const int HOMEY=1700;

void initPlotter()
{
	pinMode(X1pin, OUTPUT);
	pinMode(X2pin, OUTPUT);
	pinMode(X4pin, OUTPUT);
	pinMode(Y1pin, OUTPUT);
	pinMode(Y2pin, OUTPUT);
	pinMode(Y4pin, OUTPUT);
	pinMode(PENpin, OUTPUT);
	BASICPOSITION();
	OK();
}

Point getCurrentPlotterPos()
{
	return currentPos;
}

void sendXY(int px, int py)
{
	px = -px;
	py = -py;
	//DDRD = pins 0-7 (D0-D7)
	//76543210
	//xxx124xx 	Y
	//124xxxxx 	X
	int state = DDRD & B00000011; 
	//int bx = px & B111; 
	int bx = ((px & B001)<<2) | (px&B010) | ((px&B100)>>2);	
	//int by = py & B111; 
	int by = ((py & B001)<<2) | (py&B010) | ((py&B100)>>2);
	DDRD = state | (bx<<5) | (by<<2);
	//Serial.println(DDRD, BIN);
	delay(3);
}


void DEMO()
{
	LOG(">Demo");
	PD();
	MOVETO(300,0);
	MOVETO(300,300);
	MOVETO(0,300);
	MOVETO(0,0);
	MOVETO(600,300);	
	PU();
	MOVETO(600,0);
	PD();
	MOVETO(0,300);
	PU();
}

void OK()
{
	Serial.write("OK ");
	Serial.print(currentPos.x,DEC);
	Serial.write(",");
	Serial.print(currentPos.y,DEC);
	Serial.write(";\n");
}

void BASICPOSITION()
{
	LOG(">Basic position");
	PU();
	MOVETO(-MAXX, 0);
	currentPos.x=0;
	currentPos.y=HOMEY;
}

void PD()
{
	digitalWrite(PENpin, HIGH);
	delay(50);
}

void PU()
{
	digitalWrite(PENpin, LOW);
	delay(50);
}

void MOVETO(Point *pt)
{
	MOVETO((*pt).x,(*pt).y);
}

void MOVETO(int nx,int ny)
{
	int dx = abs(nx-currentPos.x);
	int dy = abs(ny-currentPos.y);
	int sx = currentPos.x<nx?1:-1;
	int sy = currentPos.y<ny?1:-1;
	int err = dx-dy;

	while(true)
	{
		sendXY(currentPos.x,currentPos.y);
		if (currentPos.x == nx && currentPos.y == ny)
			return;
		int e2 = 2*err;
		if (e2 > -dy)
		{
			err = err - dy;
			currentPos.x = currentPos.x + sx;
		}

		if (e2 <  dx)
		{
			err = err + dx;
			currentPos.y = currentPos.y + sy;
		}
	}
}

