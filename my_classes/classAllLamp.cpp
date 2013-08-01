#include "classAllLamp.h"
#include "classLamp.h"

classAllLamp::classAllLamp(void)
{
}

classAllLamp::~classAllLamp(void)
{
	std::vector <classLamp*>::iterator Iter=lampArray.begin();
	for(Iter=lampArray.begin(); Iter!=lampArray.end(); Iter++)
	{
		(*Iter)->turnOff();
		delete (*Iter);
	}
}


bool classAllLamp::addLamp(GLenum id)
{
	if (id<GL_LIGHT0 && id>GL_LIGHT7) return false;
	bool in=false;
	std::vector <classLamp*>::iterator Iter;
	for(Iter=lampArray.begin(); Iter!=lampArray.end(); Iter++)
		if ((*Iter)->getId()==id)
		{
			in=true;
			break;
		}
	if (!in)
	{
		classLamp *l=new classLamp(id);
		lampArray.push_back(l);
	}
	return (!in);
}
bool classAllLamp::addLamp(GLenum id, const GLfloat *pos, const GLfloat *dir, GLfloat spotExponent, GLfloat spotCutOff)
{
	if (id<GL_LIGHT0 && id>GL_LIGHT7) return false;
	bool in=false;
	std::vector <classLamp*>::iterator Iter;
	for(Iter=lampArray.begin(); Iter!=lampArray.end(); Iter++)
		if ((*Iter)->getId()==id)
		{
			in=true;
			break;
		}
	if (!in)
	{
		classLamp *l=new classLamp(id,pos,dir,spotExponent,spotCutOff);
		lampArray.push_back(l);
	}
	return (!in);
}
classLamp* classAllLamp::getPointToLamp(GLenum id)
{
	return (*this)[id];
}

classLamp* classAllLamp::getPointToLamp(int n)
{
	return (*this)[n];
}

classLamp* classAllLamp::operator [](GLenum id)
{
	std::vector <classLamp*>::iterator Iter;
	for(Iter=lampArray.begin(); Iter!=lampArray.end(); Iter++)
		if ((*Iter)->getId()==id) return *Iter;
	return NULL;
}
classLamp* classAllLamp::operator [](int n)
{
	if (n<0) return NULL;
	if (n>(int)(lampArray.size()-1)) return NULL;
	return lampArray[n];
}

void classAllLamp::putAll(void)
{
	int num=(int)lampArray.size();
	for(int i=0; i<num; i++)
		getPointToLamp(i)->put();
}

void classAllLamp::putAllBut(int n)
{
	int num=(int)lampArray.size();
	for(int i=0; i<num; i++)
		if (i!=n) lampArray[i]->put();
}

void classAllLamp::putAllBut(GLenum id)
{
	std::vector <classLamp*>::iterator Iter;
	for(Iter=lampArray.begin(); Iter!=lampArray.end(); Iter++)
		if ((*Iter)->getId()!=id) (*Iter)->put();
}
