#include "msModel_N.h"
#include<IL/il.h>
#include<IL/ilu.h>
#include<IL/ilut.h>
#include "windows.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include<vector>
#include"IniParser_src/IniParser.h"
using namespace IniParser;

#define def_MaxLenTextInt 5

msModel_N::msModel_N(void)
{
	textureParamArr.clear();
	TexturesLoaded=false;
	ModelLoaded=false;
}

msModel_N::~msModel_N(void)
{
	this->UnLoadTextures();
	for (std::vector<strTextureParam*>::iterator Iterator=textureParamArr.begin(); Iterator!=textureParamArr.end(); Iterator++)
	{
		delete (*Iterator);
	}
	textureParamArr.clear();
}
bool msModel_N::LoadModelFile(const char *filename)
{
	ModelLoaded=Load(filename);
	return ModelLoaded;
}
bool msModel_N::LoadModel(const char *ModelFilename, const char *TextureFilename)
{
	bool res;
	if (LoadModelFile(ModelFilename))
	{
		res=LoadTexturesFromFileToBase(TextureFilename);
		res&=LoadTextures();
		return res;
	}
	else return false;

}

bool msModel_N::LoadTextures(void)
{
	if (TexturesLoaded || !ModelLoaded) return false;
	//обновляем базу настроек текстур
	refreshBaseTextureParam();
	ilInit();
	iluInit();
	std::vector<strTextureParam*>::iterator Iterator;
	for (Iterator=textureParamArr.begin(); Iterator!=textureParamArr.end(); Iterator++)
	{
		if ((*Iterator)->Loaded==false)
		{
			//проверяем если есть уже такая текстура с таким мипмапом загруженная
			bool absent=true;
			for (std::vector<strTextureParam*>::iterator Iterator2=textureParamArr.begin(); Iterator2!=textureParamArr.end(); Iterator2++)
			{
				if ((*Iterator2)->Loaded && strcmp((*Iterator2)->filePicName,(*Iterator)->filePicName)==0 && (*Iterator2)->GetEnaMipmaping()==(*Iterator)->GetEnaMipmaping())
				{
					(*Iterator)->Loaded=true;
					(*Iterator)->id=(*Iterator2)->id;
					absent=false;
					break;
				}
			}
			if (!absent) continue;

			ILuint IlImage;
			ilGenImages(1, &IlImage);
			ilBindImage(IlImage);

			ilLoad(IL_BMP,(TCHAR*)(*Iterator)->filePicName);
			if (ilGetError()) 
			{
				UnLoadTextures();
				ilDeleteImages(1,&IlImage);
				return false;
			}
			int bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
			if (bpp!=3) 
			{
				UnLoadTextures();
				ilDeleteImages(1,&IlImage);
				return false;
			}
			int width = ilGetInteger(IL_IMAGE_WIDTH);
			int height = ilGetInteger(IL_IMAGE_HEIGHT);
			int pic_size=width*height*bpp;
			unsigned char* dataRGB=new unsigned char[pic_size];
			picRGBDataCopyBGR(ilGetData(),dataRGB,pic_size);
			//больше картинка в IL не нужна
			ilDeleteImages(1,&IlImage);
			unsigned char* copyData=new unsigned char[width * height * 4];
			//добавление A-канала и складываение всего в data	
			int j=0;
			for (int i2=0; i2<pic_size ; i2+=3)
			{
				copyData[j++]=dataRGB[i2];
				copyData[j++]=dataRGB[i2+1];
				copyData[j++]=dataRGB[i2+2];
				copyData[j++]=255;
			}
			//удаление картинки без A-канала
			delete [] dataRGB;
			//перевернуть рисунок!
			unsigned char* dataRGBA=new unsigned char[width * height * 4];
			for (int i2=0; i2<height; i2++)
				//копирование ряда copydata2[height-1-i]=data[i]
				for(int j2=0; j2<width*4; j2++)
					dataRGBA[width*4*(height-1-i2)+j2]=copyData[width*4*i2+j2];
			delete [] copyData;
			GLuint id;
			glGenTextures(1,&id);
			glBindTexture(GL_TEXTURE_2D,id);
			(*Iterator)->id=id;
			if ((*Iterator)->GetEnaMipmaping())
				gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width,height,GL_RGBA,GL_UNSIGNED_BYTE,dataRGBA);
			else
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,dataRGBA);
			delete [] dataRGBA;
			(*Iterator)->Loaded=true;
		}
	}
	glBindTexture(GL_TEXTURE_2D,0);
	TexturesLoaded=true;
	return true;
}
bool msModel_N::LoadTexturesFromFileToBase(const char *file)
{
	if (TexturesLoaded || !ModelLoaded) return false;
	if (file==NULL) return false;

	CParser pars(false);
		
	CSection *pSectMain = pars.AddSection("main");
	int numGroup=0;
	pSectMain->AddParameter("numGroup", new CNumericType<int>(&numGroup, NULL), true);
	try
	{
		pars.ParseFile(file);
	}
	catch (CException *pException)
	{
		pException->Delete();
		return false;
	}
	if (numGroup<=0) return false;

	struct str_pairNameNumberGroup
	{
		std::string name;
		int number;
		str_pairNameNumberGroup():number(-1){}
	};
	str_pairNameNumberGroup *arrPairNameNumberGroup=new str_pairNameNumberGroup[numGroup];

	char sectionKeyNameBase[]="section";
	char *sectionKeyNameNum;
	char *sectionKeyName;
	char sectionValueBase[]="group_";


	for(int i=0; i<numGroup; i++)
	{
		sectionKeyNameNum=intToCString(i);
		sectionKeyName=new char[strlen(sectionKeyNameBase)+strlen(sectionKeyNameNum)+1];
		strcpy(sectionKeyName,sectionKeyNameBase);
		strcat(sectionKeyName,sectionKeyNameNum);
		delete [] sectionKeyNameNum;
		// Параметр arrPairNameNumberGroup[i].name. Строковый параметр (строка std::string). Длина не ограничена. Пустые строки недопустимы. Обязательный.
		pSectMain->AddParameter(sectionKeyName, new CStdStringValue(&arrPairNameNumberGroup[i].name, -1, false), true);
		delete [] sectionKeyName;
		try
		{
			pars.ParseFile(file);
		}
		catch (CException *pException)
		{
			pException->Delete();
			delete [] arrPairNameNumberGroup;
			return false;
		}
		const char *t=arrPairNameNumberGroup[i].name.c_str();
		//если длинна имени секции < strlen(sectionValueBase)+1
		if (strlen(t)<(strlen(sectionValueBase)+1))
		{
			delete [] arrPairNameNumberGroup;
			return false;
		}
		//выкалупываем строку с цифрой и превращаем ее в int c проверкой результата
		if ((arrPairNameNumberGroup[i].number=CStringToInt(t+strlen(sectionValueBase)))==-1)
		{
			delete [] arrPairNameNumberGroup;
			return false;
		}
	}
	pars.RemoveSection("main");

	//шагаем по секциям и выбираем данные
	//вспомагательные переменные куда будет все считыватся из секции для определенной группы ini-файла
	std::string string_glTexEnv_GL_TEXTURE_ENV_MODE;
	GLfloat _glTexEnv_GL_TEXTURE_ENV_COLOR[4];	//вспомагательная переменная
	GLfloat _glTexEnv_GL_TEXTURE_ENV_COLOR_R;
	GLfloat _glTexEnv_GL_TEXTURE_ENV_COLOR_G;
	GLfloat _glTexEnv_GL_TEXTURE_ENV_COLOR_B;
	GLfloat _glTexEnv_GL_TEXTURE_ENV_COLOR_A;
	std::string string_glTexParameter_GL_TEXTURE_MIN_FILTER;
	std::string string_glTexParameter_GL_TEXTURE_MAG_FILTER;
	std::string string_glTexParameter_GL_TEXTURE_WRAP_S;
	std::string string_glTexParameter_GL_TEXTURE_WRAP_T;
	bool _enaMipmaping;
	std::string string_fileName;

	//объект с настройками текстуры
	strTextureParam *texParam;
	CSection *pSectGroup;
	//итератор для проверки нужно ли грузить настройки для этой группы
	//std::vector<strTextureParam*>::iterator Iterator;
	for(int i=0; i<numGroup; i++)
	{
		//если такая запись есть и ее текстура загружена - пропустить секцию
		//Iterator=getTexParam(arrPairNameNumberGroup[i].number);
		//if (Iterator!=textureParamArr.end() && (*Iterator)->Loaded==true) continue;

		//если для этой группы не нужна картинка - пропустить
		if (arrPairNameNumberGroup[i].number>GetNumGroups() || arrPairNameNumberGroup[i].number<0) continue;
		string_fileName=GetMaterial(GetGroup(arrPairNameNumberGroup[i].number)->materialIndex)->texture;
		if ((string_fileName.c_str())[0]==0) continue;


		pSectGroup=pars.AddSection(arrPairNameNumberGroup[i].name.c_str());

		//значения по умолчанию
		string_glTexEnv_GL_TEXTURE_ENV_MODE=defstr_glTexEnv_GL_TEXTURE_ENV_MODE;
		_glTexEnv_GL_TEXTURE_ENV_COLOR_R=def_glTexEnv_GL_TEXTURE_ENV_COLOR_R;
		_glTexEnv_GL_TEXTURE_ENV_COLOR_G=def_glTexEnv_GL_TEXTURE_ENV_COLOR_G;
		_glTexEnv_GL_TEXTURE_ENV_COLOR_B=def_glTexEnv_GL_TEXTURE_ENV_COLOR_B;
		_glTexEnv_GL_TEXTURE_ENV_COLOR_A=def_glTexEnv_GL_TEXTURE_ENV_COLOR_A;
		string_glTexParameter_GL_TEXTURE_MIN_FILTER=defstr_glTexParameter_GL_TEXTURE_MIN_FILTER;
		string_glTexParameter_GL_TEXTURE_MAG_FILTER=defstr_glTexParameter_GL_TEXTURE_MAG_FILTER;
		string_glTexParameter_GL_TEXTURE_WRAP_S=defstr_glTexParameter_GL_TEXTURE_WRAP_S;
		string_glTexParameter_GL_TEXTURE_WRAP_T=defstr_glTexParameter_GL_TEXTURE_WRAP_T;
		_enaMipmaping=def_enaMipmaping;
		
		pSectGroup->AddParameter("GL_TEXTURE_ENV_MODE", new CStdStringValue(&string_glTexEnv_GL_TEXTURE_ENV_MODE, -1, true), false);
		try
		{
			pars.ParseFile(file);
		}
		catch (CException *pException)
		{
			pException->Delete();
		}
		pSectGroup->RemoveParameter("GL_TEXTURE_ENV_MODE");

		pSectGroup->AddParameter("TEXTURE_ENV_COLOR_R", new CNumericType<GLfloat>(&_glTexEnv_GL_TEXTURE_ENV_COLOR_R, NULL, CNumericType<GLfloat>::fixed), false);
		try
		{
			pars.ParseFile(file);
		}
		catch (CException *pException)
		{
			pException->Delete();
		}
		pSectGroup->RemoveParameter("TEXTURE_ENV_COLOR_R");

		pSectGroup->AddParameter("TEXTURE_ENV_COLOR_G", new CNumericType<GLfloat>(&_glTexEnv_GL_TEXTURE_ENV_COLOR_G, NULL, CNumericType<GLfloat>::fixed), false);
		try
		{
			pars.ParseFile(file);
		}
		catch (CException *pException)
		{
			pException->Delete();
		}
		pSectGroup->RemoveParameter("TEXTURE_ENV_COLOR_G");

		pSectGroup->AddParameter("TEXTURE_ENV_COLOR_B", new CNumericType<GLfloat>(&_glTexEnv_GL_TEXTURE_ENV_COLOR_B, NULL, CNumericType<GLfloat>::fixed), false);
		try
		{
			pars.ParseFile(file);
		}
		catch (CException *pException)
		{
			pException->Delete();
		}
		pSectGroup->RemoveParameter("TEXTURE_ENV_COLOR_B");

		pSectGroup->AddParameter("TEXTURE_ENV_COLOR_A", new CNumericType<GLfloat>(&_glTexEnv_GL_TEXTURE_ENV_COLOR_A, NULL, CNumericType<GLfloat>::fixed), false);
		try
		{
			pars.ParseFile(file);
		}
		catch (CException *pException)
		{
			pException->Delete();
		}
		pSectGroup->RemoveParameter("TEXTURE_ENV_COLOR_A");


		pSectGroup->AddParameter("GL_TEXTURE_MIN_FILTER", new CStdStringValue(&string_glTexParameter_GL_TEXTURE_MIN_FILTER, -1, true), false);
		try
		{
			pars.ParseFile(file);
		}
		catch (CException *pException)
		{
			pException->Delete();
		}
		pSectGroup->RemoveParameter("GL_TEXTURE_MIN_FILTER");

		pSectGroup->AddParameter("GL_TEXTURE_MAG_FILTER", new CStdStringValue(&string_glTexParameter_GL_TEXTURE_MAG_FILTER, -1, true), false);
		try
		{
			pars.ParseFile(file);
		}
		catch (CException *pException)
		{
			pException->Delete();
		}
		pSectGroup->RemoveParameter("GL_TEXTURE_MAG_FILTER");

		pSectGroup->AddParameter("GL_TEXTURE_WRAP_S", new CStdStringValue(&string_glTexParameter_GL_TEXTURE_WRAP_S, -1, true), false);
		try
		{
			pars.ParseFile(file);
		}
		catch (CException *pException)
		{
			pException->Delete();
		}
		pSectGroup->RemoveParameter("GL_TEXTURE_WRAP_S");

		pSectGroup->AddParameter("GL_TEXTURE_WRAP_T", new CStdStringValue(&string_glTexParameter_GL_TEXTURE_WRAP_T, -1, true), false);
		try
		{
			pars.ParseFile(file);
		}
		catch (CException *pException)
		{
			pException->Delete();
		}
		pSectGroup->RemoveParameter("GL_TEXTURE_WRAP_T");

		pSectGroup->AddParameter("enaMipmaping", new CBoolValue(&_enaMipmaping), false);	
		try
		{
			pars.ParseFile(file);
		}
		catch (CException *pException)
		{
			pException->Delete();
		}
		pSectGroup->RemoveParameter("enaMipmaping");

		pSectGroup->AddParameter("fileName", new CStdStringValue(&string_fileName, -1, true), false);
		try
		{
			pars.ParseFile(file);
		}
		catch (CException *pException)
		{
			pException->Delete();
		}
		pSectGroup->RemoveParameter("fileName");


		//try
		//{
		//	pars.ParseFile(file);
		//}
		//catch (CException *pException)
		//{
		//	//в случае ошибки в сексии группы
		//	//delete [] arrPairNameNumberGroup;
		//	pException->Delete();
		//	////return false;
		//	pars.RemoveSection(arrPairNameNumberGroup[i].name.c_str());
		//	continue;
		//}
		pars.RemoveSection(arrPairNameNumberGroup[i].name.c_str());
		//разбор вытащенных из секции данных и формирование объекта с настроками
		texParam=new strTextureParam();
		texParam->nGroup=arrPairNameNumberGroup[i].number;

		//GL_TEXTURE_ENV_MODE
		if (!string_glTexEnv_GL_TEXTURE_ENV_MODE.compare("GL_DECAL")) 
			texParam->SetGlTexEnv_GL_TEXTURE_ENV_MODE(GL_DECAL);
		if (!string_glTexEnv_GL_TEXTURE_ENV_MODE.compare("GL_REPLACE")) 
			texParam->SetGlTexEnv_GL_TEXTURE_ENV_MODE(GL_REPLACE);
		if (!string_glTexEnv_GL_TEXTURE_ENV_MODE.compare("GL_MODULATE")) 
			texParam->SetGlTexEnv_GL_TEXTURE_ENV_MODE(GL_MODULATE);
		if (!string_glTexEnv_GL_TEXTURE_ENV_MODE.compare("GL_BLEND"))
			texParam->SetGlTexEnv_GL_TEXTURE_ENV_MODE(GL_BLEND);
		//TEXTURE_ENV_COLOR
		_glTexEnv_GL_TEXTURE_ENV_COLOR[0]=_glTexEnv_GL_TEXTURE_ENV_COLOR_R;
		_glTexEnv_GL_TEXTURE_ENV_COLOR[1]=_glTexEnv_GL_TEXTURE_ENV_COLOR_G;
		_glTexEnv_GL_TEXTURE_ENV_COLOR[2]=_glTexEnv_GL_TEXTURE_ENV_COLOR_B;
		_glTexEnv_GL_TEXTURE_ENV_COLOR[3]=_glTexEnv_GL_TEXTURE_ENV_COLOR_A;
		texParam->SetGlTexEnv_GL_TEXTURE_ENV_COLOR(_glTexEnv_GL_TEXTURE_ENV_COLOR);
		//GL_TEXTURE_MIN_FILTER
		if (!string_glTexParameter_GL_TEXTURE_MIN_FILTER.compare("GL_NEAREST")) 
			texParam->SetGlTexParameter_GL_TEXTURE_MIN_FILTER(GL_NEAREST);
		if (!string_glTexParameter_GL_TEXTURE_MIN_FILTER.compare("GL_LINEAR")) 
			texParam->SetGlTexParameter_GL_TEXTURE_MIN_FILTER(GL_LINEAR);
		if (!string_glTexParameter_GL_TEXTURE_MIN_FILTER.compare("GL_NEAREST_MIPMAP_NEAREST")) 
			texParam->SetGlTexParameter_GL_TEXTURE_MIN_FILTER(GL_NEAREST_MIPMAP_NEAREST);
		if (!string_glTexParameter_GL_TEXTURE_MIN_FILTER.compare("GL_NEAREST_MIPMAP_LINEAR")) 
			texParam->SetGlTexParameter_GL_TEXTURE_MIN_FILTER(GL_NEAREST_MIPMAP_LINEAR);
		if (!string_glTexParameter_GL_TEXTURE_MIN_FILTER.compare("GL_LINEAR_MIPMAP_NEAREST")) 
			texParam->SetGlTexParameter_GL_TEXTURE_MIN_FILTER(GL_LINEAR_MIPMAP_NEAREST);
		if (!string_glTexParameter_GL_TEXTURE_MIN_FILTER.compare("GL_LINEAR_MIPMAP_LINEAR")) 
			texParam->SetGlTexParameter_GL_TEXTURE_MIN_FILTER(GL_LINEAR_MIPMAP_LINEAR);
		//GL_TEXTURE_MAG_FILTER
		if (!string_glTexParameter_GL_TEXTURE_MAG_FILTER.compare("GL_NEAREST")) 
			texParam->SetGlTexParameter_GL_TEXTURE_MAG_FILTER(GL_NEAREST);
		if (!string_glTexParameter_GL_TEXTURE_MAG_FILTER.compare("GL_LINEAR")) 
			texParam->SetGlTexParameter_GL_TEXTURE_MAG_FILTER(GL_LINEAR);
		//GL_TEXTURE_WRAP_S
		if (!string_glTexParameter_GL_TEXTURE_WRAP_S.compare("GL_CLAMP")) 
			texParam->SetGlTexParameter_GL_TEXTURE_WRAP_S(GL_CLAMP);
		if (!string_glTexParameter_GL_TEXTURE_WRAP_S.compare("GL_REPEAT")) 
			texParam->SetGlTexParameter_GL_TEXTURE_WRAP_S(GL_REPEAT);
		//GL_TEXTURE_WRAP_T
		if (!string_glTexParameter_GL_TEXTURE_WRAP_T.compare("GL_CLAMP")) 
			texParam->SetGlTexParameter_GL_TEXTURE_WRAP_T(GL_CLAMP);
		if (!string_glTexParameter_GL_TEXTURE_WRAP_T.compare("GL_REPEAT")) 
			texParam->SetGlTexParameter_GL_TEXTURE_WRAP_T(GL_REPEAT);
		//enaMipmaping
		texParam->SetEnaMipmaping(_enaMipmaping);
		//fileName
		if ((string_fileName.c_str())[0]!=0)
		{
			texParam->filePicName=new char[strlen(string_fileName.c_str())+1];
			strcpy(texParam->filePicName,string_fileName.c_str());
		}
		SetTextureParam(texParam);
		
		delete texParam;
	}

	delete [] arrPairNameNumberGroup;

	return true;
}
void msModel_N::UnLoadTextures(void)
{
	std::vector<strTextureParam*>::iterator Iterator;
	for (Iterator=textureParamArr.begin(); Iterator!=textureParamArr.end(); Iterator++)
	{
		glDeleteTextures(1,&((*Iterator)->id));
		(*Iterator)->Loaded=false;
	}
	TexturesLoaded=false;
}
//bool msModel_N::ReloadTextures()
//{
//	if (TexturesLoaded) UnLoadTextures();
//	return LoadTextures();
//}
//void msModel_N::setTextureParamArr(const std::vector<strTextureParam> *TPArr)
//{
//	textureParamArr.clear();
//	size_t count=TPArr->size();
//	for(size_t i=0; i<count; i++)
//		textureParamArr.push_back((*TPArr)[i]);
//}
//void msModel_N::setTextureParamArr(const strTextureParam *TPArr, unsigned int num)
//{
//	textureParamArr.clear();
//	for(unsigned int i=0; i<num; i++)
//		textureParamArr.push_back(TPArr[i]);
//
//}
bool msModel_N::SetTextureParam(const strTextureParam *TP)
{
	if (TexturesLoaded || !ModelLoaded) return false;
	if ((TP->nGroup)<0) return false;
	std::vector<strTextureParam*>::iterator Iterator=getTexParam(TP->nGroup);
	//if (Iterator!=textureParamArr.end() && (*Iterator)->Loaded==true) return false;
	//проверка нужна ли текстура этой группе
	ms3d_group_t *group=GetGroup(TP->nGroup);
	ms3d_material_t *material=GetMaterial(group->materialIndex);
	if (material->texture[0]==0) return false;
	strTextureParam *t=new strTextureParam();
	(*t)=(*TP);
	t->id=0;
	t->Loaded=false;
	if (t->filePicName==NULL)
	{
		t->filePicName=new char[strlen(material->texture)+1];
		strcpy(t->filePicName,material->texture);
	}
	else
	{
		//проверка наличия файла описаного в t->filePicName
		FILE *fp = fopen(t->filePicName, "rb");
		if (!fp)
		{
			delete	t->filePicName;
			t->filePicName=new char[strlen(material->texture)+1];
			strcpy(t->filePicName,material->texture);
		}
		else fclose(fp);
	}

	if (Iterator==textureParamArr.end()) 
	{
		textureParamArr.push_back(t);
	}
	else 
	{
		(*(*Iterator))=(*t);
	}
	return true;
}
bool msModel_N::DelTextureParam(unsigned int nGroup)
{
	if (TexturesLoaded) return false;
	if (nGroup<0) return false;
	std::vector<strTextureParam*>::iterator Iterator=getTexParam(nGroup);
	if (Iterator==textureParamArr.end()) return false;
	//if ((*Iterator)->Loaded==true) return false;
	delete (*Iterator);
	std::vector<strTextureParam*>::iterator Iterator_;
	std::vector<strTextureParam*> tArr;
	for (Iterator_=textureParamArr.begin(); Iterator_!=textureParamArr.end(); Iterator_++)
		if (Iterator_!=Iterator) tArr.push_back(*Iterator_);
	textureParamArr.clear();
	for (Iterator_=tArr.begin(); Iterator_!=tArr.end(); Iterator_++)
		textureParamArr.push_back(*Iterator_);
	tArr.clear();
	return true;
}
bool msModel_N::DelTextureParam(const strTextureParam *TP)
{
	if (TP==NULL) return false;
	return DelTextureParam(TP->nGroup);
}
const strTextureParam* msModel_N::GetTextureParam(unsigned int nGroup)
{
	std::vector<strTextureParam*>::iterator Iterator=getTexParam(nGroup);
	if (Iterator==textureParamArr.end()) return NULL;
	return (*Iterator);
}

void msModel_N::picRGBDataCopyBGR(const unsigned char * source, unsigned char * dist, int count)
{
	for(int i=0; i<count; i+=3)
	{
		dist[i]=source[i+2];
		dist[i+1]=source[i+1];
		dist[i+2]=source[i];
	}
}
std::vector<strTextureParam*>::iterator msModel_N::getTexParam(int nGroup)
{
	std::vector<strTextureParam*>::iterator Iterator=textureParamArr.begin();
	for (Iterator=textureParamArr.begin(); Iterator!=textureParamArr.end(); Iterator++)
		if ((*Iterator)->nGroup==nGroup) return Iterator;
	return textureParamArr.end();
}
GLuint msModel_N::GetTextureId(int nGroup)
{
	std::vector<strTextureParam*>::iterator Iterator=getTexParam(nGroup);
	if (Iterator!=textureParamArr.end()) return (*Iterator)->id;
	else return 0;
}
bool msModel_N::BindTexParam(int nGroup)
{
	if (!TexturesLoaded) return false;
	std::vector<strTextureParam*>::iterator Iterator=getTexParam(nGroup);
	if (Iterator==textureParamArr.end()) return false;
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, (*Iterator)->GetGlTexParameter_GL_TEXTURE_WRAP_S());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, (*Iterator)->GetGlTexParameter_GL_TEXTURE_WRAP_T());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (*Iterator)->GetGlTexParameter_GL_TEXTURE_MIN_FILTER());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (*Iterator)->GetGlTexParameter_GL_TEXTURE_MAG_FILTER());
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,(*Iterator)->GetGlTexEnv_GL_TEXTURE_ENV_MODE());
	glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,(*Iterator)->GetGlTexEnv_GL_TEXTURE_ENV_COLOR());
	return true;
}
bool msModel_N::BindTextureWithParam(int nGroup)
{
	if (!TexturesLoaded) return false;
	GLuint id=GetTextureId(nGroup);
	if (id==0) return false;
	glBindTexture(GL_TEXTURE_2D,id);
	BindTexParam(nGroup);
	return true;
}
void msModel_N::refreshBaseTextureParam(void)
{
	if (TexturesLoaded || !ModelLoaded) return;
	int numGroup=this->GetNumGroups();
	for(int i=0; i<numGroup; i++)
	{
		ms3d_group_t *group=GetGroup(i);
		ms3d_material_t *material=GetMaterial(group->materialIndex);
		if (material->texture[0]!=0)
		{
			std::vector<strTextureParam*>::iterator iter=getTexParam(i);
			if (iter!=textureParamArr.end())
			{
				//if ((*iter)->filePicName==NULL)
				//{
				//	(*iter)->filePicName=new char[strlen(material->texture)+1];
				//	strcpy((*iter)->filePicName,material->texture);
				//}
				continue;
			}
			strTextureParam *t=new strTextureParam();
			(*t)=textureParamDefault;
			t->filePicName=new char[strlen(material->texture)+1];
			strcpy(t->filePicName,material->texture);
			t->nGroup=i;
			t->id=0;
			t->Loaded=false;
			textureParamArr.push_back(t);
		}
	}
}
//преобразование беззнакового int в строку
char* msModel_N::intToCString(int n)
{
	if (n<0) return NULL;
	char *str=new char[def_MaxLenTextInt+1];
	int i=0;
	int ich;
	do
	{
		ich=n%10;
		n/=10;
		str[i++]=(int)'0'+ich;
	}
	while(n>0 && i<def_MaxLenTextInt);

	str[i++]=0;
	char *str2=new char[i--];
	int k=0;
	for(int j=i-1; j>=0; j--)
		str2[k++]=str[j];
	str2[k]=0;
	delete [] str;
	return str2;
}
//преобразование строки беззнакового целого в int
int msModel_N::CStringToInt(const char *str)
{
	if (str==NULL) return -1;
	int size=strlen(str);
	if (size>def_MaxLenTextInt) return -1;
	int k=1;
	int result=0;
	for(int i=size-1; i>=0; i--)
	{
		if (str[i]<(int)'0' && str[i]>(int)'9') return -1;
		result+=(str[i]-(int)'0') *k;
		k*=10;
	}
	return result;
}