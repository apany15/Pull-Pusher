#pragma once
#include "windows.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glaux.h"
#include "glut.h"
#include "math.h"
//#include "classRoom.h"

class classCharacter
{
public:
	GLfloat xx;	//координата x персонажа 
	GLfloat yy;	//координата y персонажа (рост)
	GLfloat zz;

	GLfloat ax;	//угол поворота по оси x
	GLfloat ay;	//угол поворота по оси y
//	classRoom *r;
	GLfloat dw;
protected:
	classCharacter(void);
public:
	classCharacter(GLfloat dw, /*classRoom *room,*/GLfloat startX, GLfloat startY, GLfloat startZ, GLfloat ax, GLfloat ay);
	~classCharacter(void);
	bool stepForward(void);
	bool stepBack(void);
	bool stepLeft(void);
	bool stepRight(void);
	void openEyes(void);
	void transferASightOnAMousePush(int button, int state, int x, int y);
	//bool transferASightOnAMouseMove(int x, int y, int windowWidht, int windowHeight);
	bool mouseMove(int x, int y, int windowWidht, int windowHeight, int *oldx=0, int *oldy=0);
protected:
	int zahvatX;
	int zahvatY;
};
