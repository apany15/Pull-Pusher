#pragma once
#include "classalllamp.h"
#include"gl\glew.h"

class classLight :
	public classAllLamp
{
	GLenum shadeModel;

public:
	GLfloat ambientColor[4];
	GLenum glLightModelLocalViewer;
	GLenum glLightModelTwoSide;

	classLight(void);
	~classLight(void);
	void on(void);
	void off(void);
	void init(void);
	GLenum getShadeModel(void);
	bool setShadeModel(GLenum shadeModel);
};
