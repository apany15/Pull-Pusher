#pragma once
#include"MyNSModel.h"
namespace MY_NS_MODEL
{
	class MyClassVector;

	class MyClassNormal
	{
	protected:
		float coord[3];
	public:

		MyClassNormal(const float *_coord = 0);
		MyClassNormal(float _coordX, float _coordY, float _coordZ);
		const float * GetCoord()const;
		void SetCoord(const float * coord);
		void SetCoord(float coordX, float coordY, float coordZ);
		
		//сравнение объектов
		bool operator == (const MyClassNormal & _normalObj) const;
		//преобразование в объект класса MyClassVector
		operator MyClassVector();
		////преобразование в указатель на float
		//operator float*();
		////преобразование в указатель на const float
		//operator const float*() const;
		const float * operator = (const float * _floatObj);
		float & operator[](unsigned int num);
		const float & operator[](unsigned int num)const;
		MyClassNormal Averaging(const MyClassNormal & _MyClassNormalObj) const;
		void AveragToMe(const MyClassNormal & _MyClassNormalObj);
		void AveragToMe(const MyClassNormal * _MyClassNormalObj);

		bool IsNormalized();
		void Normalize();
		//MyClassNormal VectorMultToMe(const MyClassNormal & _MyClassNormalObj) const;
	};
}

