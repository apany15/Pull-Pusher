#include "classCharacter.h"
#define pi 3.1415926535897932384626433832795

classCharacter::classCharacter(void)
//: zahvatX(0)
{

}
classCharacter::~classCharacter(void)
{
}
classCharacter::classCharacter(GLfloat dw, /*classRoom *room,*/GLfloat startX, GLfloat startY, GLfloat startZ, GLfloat ax, GLfloat ay)
{
	(*this).dw=dw;
	//(*this).r=room;
	(*this).xx=startX;
	(*this).yy=startY;
	(*this).zz=startZ;
	(*this).ax=ax;
	(*this).ay=ay;
}

bool classCharacter::stepForward(void)
{
	GLfloat txx=(*this).xx;
	GLfloat tzz=(*this).zz;
	tzz-=(*this).dw*cos((*this).ax);
	txx+=(*this).dw*sin((*this).ax);
//	if ((*this).r->pointIn(txx,(*this).yy,tzz))
//	{
		(*this).xx=txx;
		(*this).zz=tzz;
		return true;
//	}
//	return false;

}

bool classCharacter::stepBack(void)
{
	GLfloat txx=(*this).xx;
	GLfloat tzz=(*this).zz;
	tzz+=(*this).dw*cos((*this).ax);
	txx-=(*this).dw*sin((*this).ax);
//	if ((*this).r->pointIn(txx,(*this).yy,tzz))
//	{
		(*this).xx=txx;
		(*this).zz=tzz;
		return true;
//	}
//	return false;
}


bool classCharacter::stepLeft(void)
{
	GLfloat txx=(*this).xx;
	GLfloat tzz=(*this).zz;

	tzz-=(*this).dw*cos(pi/2-(*this).ax);
	txx-=(*this).dw*sin(pi/2-(*this).ax);

//	if ((*this).r->pointIn(txx,(*this).yy,tzz))
//	{
		(*this).xx=txx;
		(*this).zz=tzz;
		return true;
//	}
//	return false;

}

bool classCharacter::stepRight(void)
{
	GLfloat txx=(*this).xx;
	GLfloat tzz=(*this).zz;

	tzz+=(*this).dw*cos(pi/2-(*this).ax);
	txx+=(*this).dw*sin(pi/2-(*this).ax);

//	if ((*this).r->pointIn(txx,(*this).yy,tzz))
//	{
		(*this).xx=txx;
		(*this).zz=tzz;
		return true;
//	}

//	return false;
}

void classCharacter::openEyes(void)
{
	gluLookAt((*this).xx,(*this).yy,(*this).zz,(*this).xx+sin((*this).ax),(*this).yy+sin((*this).ay),(*this).zz-cos((*this).ax),0,1,0);
}

void classCharacter::transferASightOnAMousePush(int button, int state, int x, int y)
{
	//if (state==GLUT_DOWN) 
	//{
		(*this).zahvatX=x;
		(*this).zahvatY=y;
	//}
}

//bool classCharacter::transferASightOnAMouseMove(int x, int y, int windowWidht, int windowHeight)
//{
//	if (x>=0 && x<=windowWidht && y>=0 && y<=windowHeight)
//	{
//		(*this).ax+=(x-(*this).zahvatX)*(pi/windowWidht);
//		if ((*this).ax>pi) (*this).ax=(*this).ax-2*pi;
//		if ((*this).ax<(-pi)) (*this).ax=(*this).ax+2*pi;
//		(*this).ay-=(y-(*this).zahvatY)*(pi/windowHeight);
//		if ((*this).ay>(pi/2)) (*this).ay=pi/2;
//		if ((*this).ay<(-pi/2)) (*this).ay=-pi/2;
//		(*this).zahvatX=x;
//		(*this).zahvatY=y;
//		return true;
//	}
//	return false;
//}
bool classCharacter::mouseMove(int x, int y, int windowWidht, int windowHeight, int *oldx, int *oldy)
{
	if (oldx) (*oldx)=(*this).zahvatX;
	if (oldy) (*oldy)=(*this).zahvatY;
	
	if (x>=0 && x<=windowWidht && y>=0 && y<=windowHeight)
	{
		(*this).ax+=(x-(*this).zahvatX)*(pi/windowWidht);
		if ((*this).ax>pi) (*this).ax=(*this).ax-2*pi;
		if ((*this).ax<(-pi)) (*this).ax=(*this).ax+2*pi;
		(*this).ay-=(y-(*this).zahvatY)*(pi/windowHeight);
		if ((*this).ay>(pi/2)) (*this).ay=pi/2;
		if ((*this).ay<(-pi/2)) (*this).ay=-pi/2;
		(*this).zahvatX=x;
		(*this).zahvatY=y;
		return true;
	}
	return false;
}
