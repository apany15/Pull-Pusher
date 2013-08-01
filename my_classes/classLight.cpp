#include "classLight.h"

classLight::classLight(void)
{
	ambientColor[0]=0.2f;
	ambientColor[1]=0.2f;
	ambientColor[2]=0.2f;
	ambientColor[3]=1.0f;
	glLightModelLocalViewer=GL_TRUE;
	glLightModelTwoSide=GL_TRUE;
	shadeModel=GL_SMOOTH;
	init();
}

classLight::~classLight(void)
{
	off();
}

void classLight::on(void)
{
	glEnable(GL_LIGHTING);
}

void classLight::off(void)
{
	glDisable(GL_LIGHTING);
}

void classLight::init(void)
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,glLightModelLocalViewer);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,glLightModelTwoSide);
	glShadeModel(shadeModel);
}

GLenum classLight::getShadeModel(void)
{
	return shadeModel;
}

bool classLight::setShadeModel(GLenum shadeModel)
{
	if (shadeModel!=GL_SMOOTH && shadeModel!=GL_FLAT) return false;
	this->shadeModel=shadeModel;
	glShadeModel(shadeModel);
	return true;
}

