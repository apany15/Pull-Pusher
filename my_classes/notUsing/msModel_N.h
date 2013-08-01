#pragma once
#include "msmodel.h"
#include<windows.h>
#include "GL/gl.h"
#include<vector>
//#include "GL/glu.h"
//#include "GL/glaux.h"
//#include "glut.h"

#define def_glTexEnv_GL_TEXTURE_ENV_MODE GL_DECAL
#define defstr_glTexEnv_GL_TEXTURE_ENV_MODE "GL_DECAL"
#define def_glTexEnv_GL_TEXTURE_ENV_COLOR_R 1.0f
#define def_glTexEnv_GL_TEXTURE_ENV_COLOR_G 1.0f
#define def_glTexEnv_GL_TEXTURE_ENV_COLOR_B 1.0f
#define def_glTexEnv_GL_TEXTURE_ENV_COLOR_A 1.0f
#define def_glTexParameter_GL_TEXTURE_MIN_FILTER GL_LINEAR
#define defstr_glTexParameter_GL_TEXTURE_MIN_FILTER "GL_LINEAR"
#define def_glTexParameter_GL_TEXTURE_MAG_FILTER GL_LINEAR
#define defstr_glTexParameter_GL_TEXTURE_MAG_FILTER "GL_LINEAR"
#define def_glTexParameter_GL_TEXTURE_WRAP_S GL_REPEAT
#define defstr_glTexParameter_GL_TEXTURE_WRAP_S "GL_REPEAT"
#define def_glTexParameter_GL_TEXTURE_WRAP_T GL_REPEAT
#define defstr_glTexParameter_GL_TEXTURE_WRAP_T "GL_REPEAT"
#define def_enaMipmaping false



class strDefTextureParam
{
protected:
	GLenum glTexEnv_GL_TEXTURE_ENV_MODE;
	GLfloat glTexEnv_GL_TEXTURE_ENV_COLOR[4];
	GLenum glTexParameter_GL_TEXTURE_MIN_FILTER;
	GLenum glTexParameter_GL_TEXTURE_MAG_FILTER;
	GLenum glTexParameter_GL_TEXTURE_WRAP_S;
	GLenum glTexParameter_GL_TEXTURE_WRAP_T;
	bool enaMipmaping;
public:
	//конструктор
	strDefTextureParam()
	{
		glTexEnv_GL_TEXTURE_ENV_MODE=def_glTexEnv_GL_TEXTURE_ENV_MODE;
		glTexEnv_GL_TEXTURE_ENV_COLOR[0]=def_glTexEnv_GL_TEXTURE_ENV_COLOR_R;
		glTexEnv_GL_TEXTURE_ENV_COLOR[1]=def_glTexEnv_GL_TEXTURE_ENV_COLOR_G;
		glTexEnv_GL_TEXTURE_ENV_COLOR[2]=def_glTexEnv_GL_TEXTURE_ENV_COLOR_B;
		glTexEnv_GL_TEXTURE_ENV_COLOR[3]=def_glTexEnv_GL_TEXTURE_ENV_COLOR_A;
		glTexParameter_GL_TEXTURE_MIN_FILTER=def_glTexParameter_GL_TEXTURE_MIN_FILTER;
		glTexParameter_GL_TEXTURE_MAG_FILTER=def_glTexParameter_GL_TEXTURE_MAG_FILTER;
		glTexParameter_GL_TEXTURE_WRAP_S=def_glTexParameter_GL_TEXTURE_WRAP_S;
		glTexParameter_GL_TEXTURE_WRAP_T=def_glTexParameter_GL_TEXTURE_WRAP_T;
		enaMipmaping=def_enaMipmaping;
	}
	//set
	const GLenum & SetGlTexEnv_GL_TEXTURE_ENV_MODE(const GLenum & _glTexEnv_GL_TEXTURE_ENV_MODE)
	{
		if (_glTexEnv_GL_TEXTURE_ENV_MODE==GL_DECAL || _glTexEnv_GL_TEXTURE_ENV_MODE==GL_REPLACE || _glTexEnv_GL_TEXTURE_ENV_MODE!=GL_MODULATE || _glTexEnv_GL_TEXTURE_ENV_MODE==GL_BLEND)
			glTexEnv_GL_TEXTURE_ENV_MODE=_glTexEnv_GL_TEXTURE_ENV_MODE;
		return glTexEnv_GL_TEXTURE_ENV_MODE;
	}
	const GLfloat * SetGlTexEnv_GL_TEXTURE_ENV_COLOR(const GLfloat *_glTexEnv_GL_TEXTURE_ENV_COLOR)
	{
		if (_glTexEnv_GL_TEXTURE_ENV_COLOR==NULL) return glTexEnv_GL_TEXTURE_ENV_COLOR;
		for(int i=0; i<4; i++)
		{
			glTexEnv_GL_TEXTURE_ENV_COLOR[i]=_glTexEnv_GL_TEXTURE_ENV_COLOR[i];
			if (glTexEnv_GL_TEXTURE_ENV_COLOR[i]>1.0f) glTexEnv_GL_TEXTURE_ENV_COLOR[i]=1.0f;
			if (glTexEnv_GL_TEXTURE_ENV_COLOR[i]<0.0f) glTexEnv_GL_TEXTURE_ENV_COLOR[i]=0.0f;
		}
		return glTexEnv_GL_TEXTURE_ENV_COLOR;
	}
	const GLenum & SetGlTexParameter_GL_TEXTURE_MIN_FILTER(const GLenum & _glTexParameter_GL_TEXTURE_MIN_FILTER)
	{
		if (_glTexParameter_GL_TEXTURE_MIN_FILTER==GL_NEAREST || _glTexParameter_GL_TEXTURE_MIN_FILTER==GL_LINEAR || _glTexParameter_GL_TEXTURE_MIN_FILTER==GL_NEAREST_MIPMAP_NEAREST
				|| _glTexParameter_GL_TEXTURE_MIN_FILTER==GL_NEAREST_MIPMAP_LINEAR || _glTexParameter_GL_TEXTURE_MIN_FILTER==GL_LINEAR_MIPMAP_NEAREST
				|| _glTexParameter_GL_TEXTURE_MIN_FILTER==GL_LINEAR_MIPMAP_LINEAR)
			glTexParameter_GL_TEXTURE_MIN_FILTER=_glTexParameter_GL_TEXTURE_MIN_FILTER;
		return glTexParameter_GL_TEXTURE_MIN_FILTER;
	}
	const GLenum & SetGlTexParameter_GL_TEXTURE_MAG_FILTER(const GLenum & _glTexParameter_GL_TEXTURE_MAG_FILTER)
	{
		if (_glTexParameter_GL_TEXTURE_MAG_FILTER==GL_NEAREST || _glTexParameter_GL_TEXTURE_MAG_FILTER==GL_LINEAR)
			glTexParameter_GL_TEXTURE_MAG_FILTER=_glTexParameter_GL_TEXTURE_MAG_FILTER;
		return glTexParameter_GL_TEXTURE_MAG_FILTER;
	}
	const GLenum & SetGlTexParameter_GL_TEXTURE_WRAP_S(const GLenum & _glTexParameter_GL_TEXTURE_WRAP_S)
	{
		if (_glTexParameter_GL_TEXTURE_WRAP_S==GL_CLAMP || _glTexParameter_GL_TEXTURE_WRAP_S==GL_REPEAT /*||_glTexParameter_GL_TEXTURE_WRAP_S==GL_CLAMP_TO_EDGE*/)
			glTexParameter_GL_TEXTURE_WRAP_S=_glTexParameter_GL_TEXTURE_WRAP_S;
		return glTexParameter_GL_TEXTURE_WRAP_S;
	}
	const GLenum & SetGlTexParameter_GL_TEXTURE_WRAP_T(const GLenum & _glTexParameter_GL_TEXTURE_WRAP_T)
	{
		if (_glTexParameter_GL_TEXTURE_WRAP_T==GL_CLAMP || _glTexParameter_GL_TEXTURE_WRAP_T==GL_REPEAT /*||_glTexParameter_GL_TEXTURE_WRAP_T==GL_CLAMP_TO_EDGE*/)
			glTexParameter_GL_TEXTURE_WRAP_T=_glTexParameter_GL_TEXTURE_WRAP_T;
		return glTexParameter_GL_TEXTURE_WRAP_T;
	}
	const bool & SetEnaMipmaping(const bool & _enaMipmaping)
	{
		enaMipmaping=_enaMipmaping;
		return _enaMipmaping;
	}

	//get
	const GLenum & GetGlTexEnv_GL_TEXTURE_ENV_MODE() const
	{
		return glTexEnv_GL_TEXTURE_ENV_MODE;
	}
	const GLfloat * GetGlTexEnv_GL_TEXTURE_ENV_COLOR() const
	{
		return glTexEnv_GL_TEXTURE_ENV_COLOR;
	}
	const GLenum & GetGlTexParameter_GL_TEXTURE_MIN_FILTER() const
	{
		return glTexParameter_GL_TEXTURE_MIN_FILTER;
	}
	const GLenum & GetGlTexParameter_GL_TEXTURE_MAG_FILTER() const
	{
		return glTexParameter_GL_TEXTURE_MAG_FILTER;
	}
	const GLenum & GetGlTexParameter_GL_TEXTURE_WRAP_S() const
	{
		return glTexParameter_GL_TEXTURE_WRAP_S;
	}
	const GLenum & GetGlTexParameter_GL_TEXTURE_WRAP_T() const
	{
		return glTexParameter_GL_TEXTURE_WRAP_T;
	}
	const bool & GetEnaMipmaping() const
	{
		return enaMipmaping;
	}

};
class strTextureParam : public strDefTextureParam
{
public:
	int nGroup;
	GLuint id;
	bool Loaded;
	char *filePicName;
public:
	strTextureParam ()
	{
		nGroup=-1;
		id=0;
		Loaded=false;
		filePicName=NULL;
	}
	//конструктор копирования
	strTextureParam(const strTextureParam& _strTextureParam)
	{
		nGroup=_strTextureParam.nGroup;
		id=_strTextureParam.id;
		glTexEnv_GL_TEXTURE_ENV_MODE=_strTextureParam.glTexEnv_GL_TEXTURE_ENV_MODE;
		glTexEnv_GL_TEXTURE_ENV_COLOR[0]=_strTextureParam.glTexEnv_GL_TEXTURE_ENV_COLOR[0];
		glTexEnv_GL_TEXTURE_ENV_COLOR[1]=_strTextureParam.glTexEnv_GL_TEXTURE_ENV_COLOR[1];
		glTexEnv_GL_TEXTURE_ENV_COLOR[2]=_strTextureParam.glTexEnv_GL_TEXTURE_ENV_COLOR[2];
		glTexEnv_GL_TEXTURE_ENV_COLOR[3]=_strTextureParam.glTexEnv_GL_TEXTURE_ENV_COLOR[3];
		glTexParameter_GL_TEXTURE_MIN_FILTER=_strTextureParam.glTexParameter_GL_TEXTURE_MIN_FILTER;
		glTexParameter_GL_TEXTURE_MAG_FILTER=_strTextureParam.glTexParameter_GL_TEXTURE_MAG_FILTER;
		glTexParameter_GL_TEXTURE_WRAP_S=_strTextureParam.glTexParameter_GL_TEXTURE_WRAP_S;
		glTexParameter_GL_TEXTURE_WRAP_T=_strTextureParam.glTexParameter_GL_TEXTURE_WRAP_T;
		enaMipmaping=_strTextureParam.enaMipmaping;
		Loaded=_strTextureParam.Loaded;

		if (filePicName!=NULL) 
		{
			delete [] filePicName;
			filePicName=NULL;
		}
		if (_strTextureParam.filePicName!=NULL)
		{
			filePicName=new char[strlen(_strTextureParam.filePicName)+1];
			for(int i=0; i<(int)(strlen(_strTextureParam.filePicName)+1); i++)
				filePicName[i]=_strTextureParam.filePicName[i];
		}

	}

	//деструктор
	~strTextureParam ()
	{
		if (filePicName!=NULL) 
			delete [] filePicName;
	}

	const strTextureParam & operator =(const strTextureParam & source)
	{
		glTexEnv_GL_TEXTURE_ENV_MODE=source.GetGlTexEnv_GL_TEXTURE_ENV_MODE();
		SetGlTexEnv_GL_TEXTURE_ENV_COLOR(source.GetGlTexEnv_GL_TEXTURE_ENV_COLOR());
		glTexParameter_GL_TEXTURE_MIN_FILTER=source.GetGlTexParameter_GL_TEXTURE_MIN_FILTER();
		glTexParameter_GL_TEXTURE_MAG_FILTER=source.GetGlTexParameter_GL_TEXTURE_MAG_FILTER();
		glTexParameter_GL_TEXTURE_WRAP_S=source.GetGlTexParameter_GL_TEXTURE_WRAP_S();
		glTexParameter_GL_TEXTURE_WRAP_T=source.GetGlTexParameter_GL_TEXTURE_WRAP_T();
		enaMipmaping=source.GetEnaMipmaping();
		nGroup=source.nGroup;
		id=source.id;
		Loaded=source.Loaded;
		if (filePicName!=NULL) 
		{
			delete [] filePicName;
			filePicName=NULL;
		}
		if (source.filePicName!=NULL)
		{
			filePicName=new char[strlen(source.filePicName)+1];
			int i;
			for(i=0; i<(int)(strlen(source.filePicName)+1); i++)
				filePicName[i]=source.filePicName[i];
		}
		return *this;
	}
	const strTextureParam & operator =(const strDefTextureParam & source)
	{
		glTexEnv_GL_TEXTURE_ENV_MODE=source.GetGlTexEnv_GL_TEXTURE_ENV_MODE();
		SetGlTexEnv_GL_TEXTURE_ENV_COLOR(source.GetGlTexEnv_GL_TEXTURE_ENV_COLOR());
		glTexParameter_GL_TEXTURE_MIN_FILTER=source.GetGlTexParameter_GL_TEXTURE_MIN_FILTER();
		glTexParameter_GL_TEXTURE_MAG_FILTER=source.GetGlTexParameter_GL_TEXTURE_MAG_FILTER();
		glTexParameter_GL_TEXTURE_WRAP_S=source.GetGlTexParameter_GL_TEXTURE_WRAP_S();
		glTexParameter_GL_TEXTURE_WRAP_T=source.GetGlTexParameter_GL_TEXTURE_WRAP_T();
		enaMipmaping=source.GetEnaMipmaping();
		nGroup=-1;
		id=0;
		Loaded=false;
		if (filePicName!=NULL) 
		{
			delete [] filePicName;
			filePicName=NULL;
		}
		return *this;
	}
};

class msModel_N : public msModel
{
protected:
	std::vector<strTextureParam*> textureParamArr;	//массив параметров наложения тукстур для каждой группы
	bool TexturesLoaded;
	bool ModelLoaded;
public:
	strDefTextureParam textureParamDefault;	//массив параметров наложения тукстур по умолчанию
	msModel_N(void);
	~msModel_N(void);
	bool LoadModelFile(const char *filename);
	bool LoadModel(const char *ModelFilename, const char *TextureFilename);
	//перебирает базу параметров и загружает картинки текстур в память
	bool LoadTextures(void);
	//загружает параметры текстур из файла в базу(кроме названия картинок)
	bool LoadTexturesFromFileToBase(const char *file);
	//выгружает из пямяти картинки текстур
	void UnLoadTextures(void);
	//bool ReloadTextures();
	bool SetTextureParam(const strTextureParam *TP);
	bool DelTextureParam(unsigned int nGroup);
	bool DelTextureParam(const strTextureParam *TP);
	const strTextureParam* GetTextureParam(unsigned int nGroup);
	GLuint GetTextureId(int nGroup);
	bool BindTexParam(int nGroup);
	bool BindTextureWithParam(int nGroup);	//биндить текстуру, настроить ее
private:
	void picRGBDataCopyBGR(const unsigned char * source, unsigned char * dist, int count);
	std::vector<strTextureParam*>::iterator getTexParam(int nGroup);
	//перебрать все записи в базе и дописать ее настройками по умолчанию если запись для какойто группы отсуствует
	//но требует настройки для текстуры
	//если запись есть но нет названия файла-картинки - дописывает в этой записи название картинки
	void refreshBaseTextureParam(void);
	//вспомагательные методы для загрузки базы настроек из ini-файла
	//преобразование беззнакового int в строку
	char* intToCString(int n);
	//преобразование строки беззнакового целого в int
	int CStringToInt(const char *str);

};
