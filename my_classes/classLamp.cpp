#include "classLamp.h"
classLamp::classLamp(void)
{}
classLamp::classLamp(GLenum id)
{
	idLamp=id;
	posX=pos;
	posY=&pos[1];
	posZ=&pos[2];
	*posX=0;
	*posY=0;
	*posZ=0;
	pos[3]=1;

	dirX=dir;
	dirY=&dir[1];
	dirZ=&dir[2];
	*dirX=0;
	*dirY=0;
	*dirZ=-1;

	spotExponent=0;
	spotCutOff=180;

	ambientColor[0]=0.0f;
	ambientColor[1]=0.0f;
	ambientColor[2]=0.0f;
	ambientColor[3]=1.0f;

	diffuseColor[0]=1.0f;
	diffuseColor[1]=1.0f;
	diffuseColor[2]=1.0f;
	diffuseColor[3]=1.0f;

	specularColor[0]=1.0f;
	specularColor[1]=1.0f;
	specularColor[2]=1.0f;
	specularColor[3]=1.0f;

	constAttenuation=1.0f;
	linearAttenuation=0.0f;
	quadraticAttenuation=0.0f;



}
classLamp::classLamp(GLenum id, const GLfloat *pos, const GLfloat *dir, GLfloat spotExponent, GLfloat spotCutOff)
{
	idLamp=id;

	posX=&(this->pos[0]);
	posY=&(this->pos[1]);
	posZ=&(this->pos[2]);

	*posX=pos[0];
	*posY=pos[1];
	*posZ=pos[2];
	this->pos[3]=pos[3];

	dirX=&(this->dir[0]);
	dirY=&(this->dir[1]);
	dirZ=&(this->dir[2]);

	*dirX=dir[0];
	*dirY=dir[1];
	*dirZ=dir[2];

	(*this).spotExponent=spotExponent;
	(*this).spotCutOff=spotCutOff;

	ambientColor[0]=0.0f;
	ambientColor[1]=0.0f;
	ambientColor[2]=0.0f;
	ambientColor[3]=1.0f;

	diffuseColor[0]=1.0f;
	diffuseColor[1]=1.0f;
	diffuseColor[2]=1.0f;
	diffuseColor[3]=1.0f;

	specularColor[0]=1.0f;
	specularColor[1]=1.0f;
	specularColor[2]=1.0f;
	specularColor[3]=1.0f;

	constAttenuation=1.0f;
	linearAttenuation=0.0f;
	quadraticAttenuation=0.0f;

}
classLamp::~classLamp(void)
{
}

void classLamp::turnOn(void)
{
	glEnable(idLamp);
}

void classLamp::turnOff(void)
{
	glDisable(idLamp);
}

void classLamp::put(void)
{
	glLightfv(idLamp, GL_POSITION, pos);
	glLightfv(idLamp, GL_SPOT_DIRECTION, dir);
}

void classLamp::setPosX(GLfloat posx)
{
	*posX=posx;
}
void classLamp::setPosY(GLfloat posy)
{
	*posY=posy;
}
void classLamp::setPosZ(GLfloat posz)
{
	*posZ=posz;
}

void classLamp::setPos(const GLfloat *pos)
{
	*posX=pos[0];
	*posY=pos[1];
	*posZ=pos[2];
	this->pos[3]=pos[3];
}
void classLamp::setDirX(GLfloat dirx)
{
	*dirX=dirx;
}
void classLamp::setDirY(GLfloat diry)
{
	*dirY=diry;
}
void classLamp::setDirZ(GLfloat dirz)
{
	*dirZ=dirz;
}
void classLamp::setDir(const GLfloat *dir)
{
	*dirX=dir[0];
	*dirY=dir[1];
	*dirZ=dir[2];
}
GLfloat classLamp::getPosX(void)
{
	return *(this->posX);
}
GLfloat classLamp::getPosY(void)
{
	return *(this->posY);
}
GLfloat classLamp::getPosZ(void)
{
	return *(this->posZ);
}
GLfloat classLamp::getDirX(void)
{
	return *(this->dirX);
}
GLfloat classLamp::getDirY(void)
{
	return *(this->dirY);
}
GLfloat classLamp::getDirZ(void)
{
	return *(this->dirZ);
}
void classLamp::setSpotExponent(GLfloat spotExponent)
{
	//тут будут проверки
	this->spotExponent=spotExponent;
}
void classLamp::setSpotCutOff(GLfloat spotCutOff)
{
	//тут будут проверки
	this->spotCutOff=spotCutOff;
}
GLfloat classLamp::getSpotExponent(void)
{
	return this->spotExponent;
}
GLfloat classLamp::getSpotCutOff(void)
{
	return this->spotCutOff;
}
void classLamp::init(void)
{
	glLightf(idLamp,GL_SPOT_EXPONENT,spotExponent);
	glLightf(idLamp,GL_SPOT_CUTOFF,spotCutOff);

	glLightfv(idLamp,GL_AMBIENT,this->ambientColor);
	glLightfv(idLamp,GL_DIFFUSE,this->diffuseColor);
	glLightfv(idLamp,GL_SPECULAR,this->specularColor);

	glLightf(idLamp,GL_CONSTANT_ATTENUATION,this->constAttenuation);
	glLightf(idLamp,GL_LINEAR_ATTENUATION,this->linearAttenuation);
	glLightf(idLamp,GL_QUADRATIC_ATTENUATION,this->quadraticAttenuation);

}

bool classLamp::isPositional(void)
{
	return (pos[3]==1.0f ? true : false);
}

void classLamp::setMePositional(void)
{
	this->pos[3]=1;
}

void classLamp::setMeDirectional(void)
{
	this->pos[3]=0;
}

GLenum classLamp::getId(void)
{
	return idLamp;
}
