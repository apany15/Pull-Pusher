#pragma once
//#include "windows.h"
//#include "GL/gl.h"
//#include "GL/glu.h"
#include"GL/glew.h"
#include "GL/glaux.h"
//#include "glut.h"
#include "math.h"

class classLamp
{
	GLfloat pos[4];
	GLfloat *posX;
	GLfloat *posY;
	GLfloat *posZ;
	GLfloat dir[3];
	GLfloat *dirX;
	GLfloat *dirY;
	GLfloat *dirZ;
	GLenum idLamp;
	GLfloat spotExponent;
	GLfloat spotCutOff;

//	bool bIsPositional;
	//запрещаем конструктор по умолчанию
	classLamp(void);
public:

	GLfloat ambientColor[4];
	GLfloat diffuseColor[4];
	GLfloat specularColor[4];

	GLfloat constAttenuation;
	GLfloat linearAttenuation;
	GLfloat quadraticAttenuation;
	
	classLamp(GLenum id);
	classLamp(GLenum id, const GLfloat *pos, const GLfloat *dir, GLfloat spotExponent, GLfloat spotCutOff);
	~classLamp(void);

	void turnOn(void);
	void turnOff(void);
	void put(void);
	void init(void);

	void setPosX(GLfloat posx);
	void setPosY(GLfloat posy);
	void setPosZ(GLfloat posz);
	void setPos(const GLfloat *pos);
	void setMePositional(void);
	void setMeDirectional(void);


	void setDirX(GLfloat dirx);
	void setDirY(GLfloat diry);
	void setDirZ(GLfloat dirz);
	void setDir(const GLfloat *dir);

	GLfloat getPosX(void);
	GLfloat getPosY(void);
	GLfloat getPosZ(void);
	bool isPositional(void);

	GLfloat getDirX(void);
	GLfloat getDirY(void);
	GLfloat getDirZ(void);

	void setSpotExponent(GLfloat spotExponent);
	void setSpotCutOff(GLfloat spotCutOff);
	GLfloat getSpotExponent(void);
	GLfloat getSpotCutOff(void);
	

	GLenum getId(void);
};
