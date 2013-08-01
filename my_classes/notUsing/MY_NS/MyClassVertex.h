#pragma once
#include"MyClassNormal.h"
#include "MyClassMessageSender.h"
namespace MY_NS_MODEL
{
	class MyClassVertex: public MyClassMessageSender
	{
	protected:
		MyClassNormal normal;
		float coord[3];
		float texCoord[3];
	public:
		unsigned int numGroupInGroup;

		MyClassVertex(const float * _coord=0, const MyClassNormal *_MyClassNormalObj=0, const float * _texCoord=0);
		MyClassVertex(float _coordX, float _coordY, float _coordZ);
		//конструктор копирования
		MyClassVertex(const MyClassVertex & _MyClassVertexObj);
		//оператор присвоения
		const MyClassVertex & operator =(const MyClassVertex & _MyClassVertexObj);
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
		
		bool EquialCoord(const MyClassVertex & _MyClassVertexObj)const;
		bool EquialCoordNormal(const MyClassVertex & _MyClassVertexObj)const;
		bool EquialCoordNormalTexCoord(const MyClassVertex & _MyClassVertexObj)const;
		~MyClassVertex();
	};
}
