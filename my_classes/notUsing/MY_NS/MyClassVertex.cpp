#include "MyClassVertex.h"
#include "MyClassNormal.h"
namespace MY_NS_MODEL
{
	//MyClassVertex::MyClassVertex(void)
	//{
	//	coord[0]=0.0f;
	//	coord[1]=0.0f;
	//	coord[2]=0.0f;
	//	texCoord[0]=0.0f;
	//	texCoord[1]=0.0f;
	//	texCoord[2]=0.0f;
	//}
	MyClassVertex::MyClassVertex(const float * _coord, const MyClassNormal *_MyClassNormalObj, const float * _texCoord)
	{
		if (_coord)
		{
			coord[0]=_coord[0];
			coord[1]=_coord[1];
			coord[2]=_coord[2];
		}
		else
		{
			coord[0]=0.0f;
			coord[1]=0.0f;
			coord[2]=0.0f;
		}
		if (_MyClassNormalObj!=0) normal=(*_MyClassNormalObj);
		if (_texCoord)
		{
			texCoord[0]=_texCoord[0];
			texCoord[1]=_texCoord[1];
			texCoord[2]=_texCoord[2];
		}
		else
		{
			texCoord[0]=0.0f;
			texCoord[1]=0.0f;
			texCoord[2]=0.0f;
		}

	}
	MyClassVertex::MyClassVertex(float _coordX, float _coordY, float _coordZ)
	{
		coord[0]=_coordX;
		coord[1]=_coordY;
		coord[2]=_coordZ;

	}
	//конструктор копирования
	MyClassVertex::MyClassVertex(const MyClassVertex & _MyClassVertexObj)
		//:MyClassMessageSender(_MyClassVertexObj) не копировать информацию о хозяине вершины!
	{
		normal=_MyClassVertexObj.normal;
		coord[0]=_MyClassVertexObj.coord[0];
		coord[1]=_MyClassVertexObj.coord[1];
		coord[2]=_MyClassVertexObj.coord[2];
		//MessageSendToAll(MyClassMessage(changeVertexCoord));
		texCoord[0]=_MyClassVertexObj.texCoord[0];
		texCoord[1]=_MyClassVertexObj.texCoord[1];
		texCoord[2]=_MyClassVertexObj.texCoord[2];
		//MessageSendToAll(changeVertexTextureCoord);
		
	}
	//оператор присвоения
	const MyClassVertex & MyClassVertex::operator =(const MyClassVertex & _MyClassVertexObj)
	{
		//при копировании вершин не копировать хозяина вершины! сообщения будут отсылатьться не туда, а новому хозяину(чужому)!
		//*((MyClassMessageSender *)this) =_MyClassVertexObj;
		normal=*(_MyClassVertexObj.GetNormal());
		SetCoord(_MyClassVertexObj.GetCoord());
		MessageSendToAll(MyClassMessage(changeVertexCoord));
		SetTexCoord(_MyClassVertexObj.GetTexCoord());
		//MessageSendToAll(changeVertexTextureCoord);

		return *this;
	}


	void MyClassVertex::SetNormal(const MyClassNormal * _MyClassNormalObj)
	{
		if (_MyClassNormalObj!=0) 
			normal=(*_MyClassNormalObj);
	}
	void MyClassVertex::SetNormal(const MyClassNormal & _MyClassNormalObj)
	{
		normal=_MyClassNormalObj;
	}
	void MyClassVertex::SetNormal(float _coordX, float _coordY, float _coordZ)
	{
		normal[0]=_coordX;
		normal[1]=_coordY;
		normal[2]=_coordZ;
	}
	const MyClassNormal * MyClassVertex::GetNormal() const
	{
		return &normal;
	}
	MyClassNormal * MyClassVertex::GetNormal()
	{
		return &normal;
	}
	void MyClassVertex::SetCoord(const float * _coord)
	{
		if (_coord)
		{
			bool ch=(_coord[0]!=coord[0] || _coord[1]!=coord[1] || _coord[2]!=coord[2]);
			coord[0]=_coord[0];
			coord[1]=_coord[1];
			coord[2]=_coord[2];
			if (ch)	
			{
				MessageSendToAll(MyClassMessage(changeVertexCoord,0));
			}
		}
	}
	void MyClassVertex::SetCoord(float _coordX, float _coordY, float _coordZ)
	{
		bool ch=(coord[0]!=_coordX || coord[1]!=_coordY || coord[2]!=_coordZ);
		coord[0]=_coordX;
		coord[1]=_coordY;
		coord[2]=_coordZ;
		if (ch) 
		{
			MessageSendToAll(MyClassMessage(changeVertexCoord,0));
		}
	}
	const float * MyClassVertex::GetCoord() const
	{
		return coord;
	}

	void MyClassVertex::SetTexCoord(const float * _texCoord)
	{
		if (_texCoord)
		{
			texCoord[0]=_texCoord[0];
			texCoord[1]=_texCoord[1];
			texCoord[2]=_texCoord[2];
			//MessageSendToAll(changeVertexTextureCoord);
		}
	}
	void MyClassVertex::SetTexCoord(float _texCoordX, float _texCoordY, float _texCoordZ)
	{
		texCoord[0]=_texCoordX;
		texCoord[1]=_texCoordY;
		texCoord[2]=_texCoordZ;
		//MessageSendToAll(changeVertexTextureCoord);
	}
	const float * MyClassVertex::GetTexCoord() const
	{
		return texCoord;
	}

	//MyClassVertex::operator float *()
	//{
	//	return coord;
	//}
	//MyClassVertex::operator const float *() const
	//{
	//	return coord;
	//}

	bool MyClassVertex::EquialCoord(const MyClassVertex & _MyClassVertexObj)const
	{
		const float *_coord=_MyClassVertexObj.coord;
		return (_coord[0]==this->coord[0] && _coord[1]==this->coord[1] && _coord[2]==this->coord[2]);
	}
	bool MyClassVertex::EquialCoordNormal(const MyClassVertex & _MyClassVertexObj)const
	{
		return (EquialCoord(_MyClassVertexObj) && this->normal==_MyClassVertexObj.normal);
	}
	bool MyClassVertex::EquialCoordNormalTexCoord(const MyClassVertex & _MyClassVertexObj)const
	{
		bool result=EquialCoordNormal(_MyClassVertexObj);
		const float * _texCoord=_MyClassVertexObj.texCoord;
		result&=(texCoord[0]==_texCoord[0] && texCoord[1]==_texCoord[1] && texCoord[2]==_texCoord[2]);
		return result;
	}
	MyClassVertex::~MyClassVertex()
	{
		MessageSendToAll(MyClassMessage(die,0)); //предсмертный вопль примитиву
	}
}
