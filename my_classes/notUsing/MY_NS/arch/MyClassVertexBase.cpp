#include "MyClassVertexBase.h"
#include "MyClassNormal.h"
namespace MY_NS_MODEL
{
	//MyClassVertexBase::MyClassVertexBase(void)
	//{
	//	coord[0]=0.0f;
	//	coord[1]=0.0f;
	//	coord[2]=0.0f;
	//	texCoord[0]=0.0f;
	//	texCoord[1]=0.0f;
	//	texCoord[2]=0.0f;
	//}
	MyClassVertexBase::MyClassVertexBase(const float * _coord, const MyClassNormal *_MyClassNormalObj, const float * _texCoord)
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
	MyClassVertexBase::MyClassVertexBase(float _coordX, float _coordY, float _coordZ)
	{
		coord[0]=_coordX;
		coord[1]=_coordY;
		coord[2]=_coordZ;
	}
	//конструктор копирования
	MyClassVertexBase::MyClassVertexBase(const MyClassVertexBase & _MyClassVertexBaseObj)
		//:MyClassMessageSender(_MyClassVertexBaseObj) не копировать информацию о хозяине вершины!
	{
		normal=_MyClassVertexBaseObj.normal;
		coord[0]=_MyClassVertexBaseObj.coord[0];
		coord[1]=_MyClassVertexBaseObj.coord[1];
		coord[2]=_MyClassVertexBaseObj.coord[2];
		//MessageSendToAll(MyClassMessage(changeVertexCoord));
		texCoord[0]=_MyClassVertexBaseObj.texCoord[0];
		texCoord[1]=_MyClassVertexBaseObj.texCoord[1];
		texCoord[2]=_MyClassVertexBaseObj.texCoord[2];
		//MessageSendToAll(changeVertexTextureCoord);
	}
	//оператор присвоения
	const MyClassVertexBase & MyClassVertexBase::operator =(const MyClassVertexBase & _MyClassVertexBaseObj)
	{
		//при копировании вершин не копировать хозяина вершины! сообщения будут отсылатьться не туда, а новому хозяину(чужому)!
		//*((MyClassMessageSender *)this) =_MyClassVertexBaseObj;
		normal=*(_MyClassVertexBaseObj.GetNormal());
		SetCoord(_MyClassVertexBaseObj.GetCoord());
		MessageSendToAll(MyClassMessage(changeVertexCoord));
		SetTexCoord(_MyClassVertexBaseObj.GetTexCoord());
		//MessageSendToAll(changeVertexTextureCoord);
		return *this;
	}


	void MyClassVertexBase::SetNormal(const MyClassNormal * _MyClassNormalObj)
	{
		if (_MyClassNormalObj!=0) 
			normal=(*_MyClassNormalObj);
	}
	void MyClassVertexBase::SetNormal(const MyClassNormal & _MyClassNormalObj)
	{
		normal=_MyClassNormalObj;
	}
	void MyClassVertexBase::SetNormal(float _coordX, float _coordY, float _coordZ)
	{
		normal[0]=_coordX;
		normal[1]=_coordY;
		normal[2]=_coordZ;
	}
	const MyClassNormal * MyClassVertexBase::GetNormal() const
	{
		return &normal;
	}
	MyClassNormal * MyClassVertexBase::GetNormal()
	{
		return &normal;
	}
	void MyClassVertexBase::SetCoord(const float * _coord)
	{
		if (_coord)
		{
			bool ch=(_coord[0]!=coord[0] || _coord[1]!=coord[1] || _coord[2]!=coord[2]);
			coord[0]=_coord[0];
			coord[1]=_coord[1];
			coord[2]=_coord[2];
			if (ch)	MessageSendToAll(MyClassMessage(changeVertexCoord,0));
		}
	}
	void MyClassVertexBase::SetCoord(float _coordX, float _coordY, float _coordZ)
	{
		bool ch=(coord[0]!=_coordX || coord[1]!=_coordY || coord[2]!=_coordZ);
		coord[0]=_coordX;
		coord[1]=_coordY;
		coord[2]=_coordZ;
		if (ch) MessageSendToAll(MyClassMessage(changeVertexCoord,0));
	}
	const float * MyClassVertexBase::GetCoord() const
	{
		return coord;
	}

	void MyClassVertexBase::SetTexCoord(const float * _texCoord)
	{
		if (_texCoord)
		{
			texCoord[0]=_texCoord[0];
			texCoord[1]=_texCoord[1];
			texCoord[2]=_texCoord[2];
			//MessageSendToAll(changeVertexTextureCoord);
		}
	}
	void MyClassVertexBase::SetTexCoord(float _texCoordX, float _texCoordY, float _texCoordZ)
	{
		texCoord[0]=_texCoordX;
		texCoord[1]=_texCoordY;
		texCoord[2]=_texCoordZ;
		//MessageSendToAll(changeVertexTextureCoord);
	}
	const float * MyClassVertexBase::GetTexCoord() const
	{
		return texCoord;
	}

	//MyClassVertexBase::operator float *()
	//{
	//	return coord;
	//}
	//MyClassVertexBase::operator const float *() const
	//{
	//	return coord;
	//}

	bool MyClassVertexBase::EquialCoord(const MyClassVertexBase & _MyClassVertexBaseObj)const
	{
		const float *_coord=_MyClassVertexBaseObj.coord;
		return (_coord[0]==this->coord[0] && _coord[1]==this->coord[1] && _coord[2]==this->coord[2]);
	}
	bool MyClassVertexBase::EquialCoordNormal(const MyClassVertexBase & _MyClassVertexBaseObj)const
	{
		return (EquialCoord(_MyClassVertexBaseObj) && this->normal==_MyClassVertexBaseObj.normal);
	}
	bool MyClassVertexBase::EquialCoordNormalTexCoord(const MyClassVertexBase & _MyClassVertexBaseObj)const
	{
		bool result=EquialCoordNormal(_MyClassVertexBaseObj);
		const float * _texCoord=_MyClassVertexBaseObj.texCoord;
		result&=(texCoord[0]==_texCoord[0] && texCoord[1]==_texCoord[1] && texCoord[2]==_texCoord[2]);
		return result;
	}
	MyClassVertexBase::~MyClassVertexBase()
	{
		MessageSendToAll(MyClassMessage(die,0)); //предсмертный вопль примитиву
	}
}
