#pragma once
#include<vector>
#include"classLamp.h"
#include<gl/glew.h>

class classAllLamp
{
private:
	std::vector <classLamp*> lampArray;	
public:
	classAllLamp(void);
	~classAllLamp(void);
	bool addLamp(GLenum id);
	bool addLamp(GLenum id, const GLfloat *pos, const GLfloat *dir, GLfloat spotExponent, GLfloat spotCutOff);
	classLamp* getPointToLamp(GLenum id);
	classLamp* getPointToLamp(int n);
	classLamp* operator [](GLenum id);
	classLamp* operator [](int n);
	void putAll(void);
	
	void putAllBut(int n);
	void putAllBut(GLenum id);
};
