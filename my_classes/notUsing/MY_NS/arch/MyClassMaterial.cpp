#include "MyClassMaterial.h"
#include "windows.h"
#include "GL/gl.h"
namespace MY_NS_MODEL
{
	MyClassMaterial::MyClassMaterial(void)
	{
		color_AMBIENT[0]=1.0f;
		color_AMBIENT[1]=1.0f;
		color_AMBIENT[2]=1.0f;
		color_AMBIENT[3]=1.0f;
		color_DIFFUSE[0]=1.0f;
		color_DIFFUSE[1]=1.0f;
		color_DIFFUSE[2]=1.0f;
		color_DIFFUSE[3]=1.0f;
		color_SPECULAR[0]=1.0f;
		color_SPECULAR[1]=1.0f;
		color_SPECULAR[2]=1.0f;
		color_SPECULAR[3]=1.0f;
		color_EMISSION[0]=0.0f;
		color_EMISSION[1]=0.0f;
		color_EMISSION[2]=0.0f;
		color_EMISSION[3]=0.0f;
		shininess=0.0f;
	}
	void MyClassMaterial::setColorAmbient(const float *colorAmbient)
	{
		if (colorAmbient)
		{
			color_AMBIENT[0]=colorAmbient[0];
			color_AMBIENT[1]=colorAmbient[1];
			color_AMBIENT[2]=colorAmbient[2];
			color_AMBIENT[3]=colorAmbient[3];
		}
	}
	void MyClassMaterial::setColorAmbient(float colorR, float colorG, float colorB, float colorA)
	{
		color_AMBIENT[0]=colorR;
		color_AMBIENT[1]=colorG;
		color_AMBIENT[2]=colorB;
		color_AMBIENT[3]=colorA;
	}
	void MyClassMaterial::setColorDiffuse(const float *colorDiffuse)
	{
		if (colorDiffuse)
		{
			color_DIFFUSE[0]=colorDiffuse[0];
			color_DIFFUSE[1]=colorDiffuse[1];
			color_DIFFUSE[2]=colorDiffuse[2];
			color_DIFFUSE[3]=colorDiffuse[3];
		}
	}
	void MyClassMaterial::setColorDiffuse(float colorR, float colorG, float colorB, float colorA)
	{
		color_DIFFUSE[0]=colorR;
		color_DIFFUSE[1]=colorG;
		color_DIFFUSE[2]=colorB;
		color_DIFFUSE[3]=colorA;
	}
	void MyClassMaterial::setColorSpecular(const float *colorSpecular)
	{
		if (colorSpecular)
		{
			color_SPECULAR[0]=colorSpecular[0];
			color_SPECULAR[1]=colorSpecular[1];
			color_SPECULAR[2]=colorSpecular[2];
			color_SPECULAR[3]=colorSpecular[3];
		}
	}
	void MyClassMaterial::setColorSpecular(float colorR, float colorG, float colorB, float colorA)
	{
		color_SPECULAR[0]=colorR;
		color_SPECULAR[1]=colorG;
		color_SPECULAR[2]=colorB;
		color_SPECULAR[3]=colorA;
	}
	void MyClassMaterial::setColorEmission(const float *colorEmission)
	{
		if (colorEmission)
		{
			color_EMISSION[0]=colorEmission[0];
			color_EMISSION[1]=colorEmission[1];
			color_EMISSION[2]=colorEmission[2];
			color_EMISSION[3]=colorEmission[3];
		}
	}
	void MyClassMaterial::setColorEmission(float colorR, float colorG, float colorB, float colorA)
	{
		color_EMISSION[0]=colorR;
		color_EMISSION[1]=colorG;
		color_EMISSION[2]=colorB;
		color_EMISSION[3]=colorA;
	}
	//void MyClassMaterial::BindOpenGL()
	//{
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color_AMBIENT);
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color_DIFFUSE);
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color_SPECULAR);
	//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color_EMISSION);
	//}
	bool MyClassMaterial::operator == (const MyClassMaterial & _MyClassMaterialObj)
	{
		bool result=true;
		for(int i=0; i<4; i++)
			if (color_AMBIENT[i]!=_MyClassMaterialObj.color_AMBIENT[i] || color_DIFFUSE[i]!=_MyClassMaterialObj.color_DIFFUSE[i] || color_SPECULAR[i]!=_MyClassMaterialObj.color_SPECULAR[i] || color_EMISSION[i]!=_MyClassMaterialObj.color_EMISSION[i])
			{
				result=false;
				break;
			}

		if (shininess!=_MyClassMaterialObj.shininess) result=false;
		return result;
	}
	//MyClassMaterial::~MyClassMaterial(void)
	//{
	//}
}
