#pragma once
#include"MyClassNormal.h"
#include "MyClassMessageSender.h"
namespace MY_NS_MODEL
{
	class MyClassVertexBase: public MyClassMessageSender
	{
	protected:
		MyClassNormal normal;
		float coord[3];
		float texCoord[3];
	public:
		
		MyClassVertexBase(const float * _coord=0, const MyClassNormal *_MyClassNormalObj=0, const float * _texCoord=0);
		MyClassVertexBase(float _coordX, float _coordY, float _coordZ);
		//конструктор копирования
		MyClassVertexBase(const MyClassVertexBase & _MyClassVertexBaseObj);
		//оператор присвоения
		const MyClassVertexBase & operator =(const MyClassVertexBase & _MyClassVertexBaseObj);
		void SetNormal(const MyClassNormal * _MyClassNormalObj);
		void SetNormal(const MyClassNormal & _MyClassNormalObj);
		void SetNormal(float _coordX, float _coordY, float _coordZ);
		const MyClassNormal * GetNormal() const;
		MyClassNormal * GetNormal();
		void SetCoord(const float * _coord);
		void SetCoord(float _coordX, float _coordY, float _coordZ);
		const float * GetCoord() const;
		void SetTexCoord(const float * _texCoord);
		void SetTexCoord(float _texCoordX, float _texCoordY, float _texCoordZ);
		const float * GetTexCoord() const;

		//operator float *();
		//operator const float *() const;
		
		bool EquialCoord(const MyClassVertexBase & _MyClassVertexBaseObj)const;
		bool EquialCoordNormal(const MyClassVertexBase & _MyClassVertexBaseObj)const;
		bool EquialCoordNormalTexCoord(const MyClassVertexBase & _MyClassVertexBaseObj)const;
		~MyClassVertexBase();
	};
}
