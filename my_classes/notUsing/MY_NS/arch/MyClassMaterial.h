#pragma once
namespace MY_NS_MODEL
{
	class MyClassMaterial
	{
	public:
		float color_AMBIENT[4];
		float color_DIFFUSE[4];
		float color_SPECULAR[4];	//отраженный цвет
		float shininess;			//коэф отражения (0(макс)..128)
		float color_EMISSION[4];	//излучаемый цвет
	public:
		MyClassMaterial(void);
		void setColorAmbient(const float *colorAmbient);
		void setColorAmbient(float colorR, float colorG, float colorB, float colorA);
		void setColorDiffuse(const float *colorDiffuse);
		void setColorDiffuse(float colorR, float colorG, float colorB, float colorA);
		void setColorSpecular(const float *colorSpecular);
		void setColorSpecular(float colorR, float colorG, float colorB, float colorA);
		void setColorEmission(const float *colorEmission);
		void setColorEmission(float colorR, float colorG, float colorB, float colorA);
		bool operator == (const MyClassMaterial & _MyClassMaterialObj);
		//void BindOpenGL();
		//~MyClassMaterial(void);
	};
}
