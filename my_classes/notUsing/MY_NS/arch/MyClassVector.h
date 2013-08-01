#pragma once

namespace MY_NS_MODEL
{
	class MyClassNormal;

	class MyClassVector
	{
	public:
		float coordA[3];
		float coordB[3];
	public:
		MyClassVector(void);
		MyClassVector(const float *_coordB, const float *_coordA = 0);
		void SetCoordA(const float * _coordA);
		void SetCoordA(float _coordAX, float _coordAY, float _coordAZ);
		void SetCoordB(const float * _coordB);
		void SetCoordB(float _coordBX, float _coordBY, float _coordBZ);

		bool operator == (const MyClassVector & _vectorObj) const;
		//преобразование в объект класса MyClassNormal
		operator MyClassNormal();
		//векторное умножение
		MyClassVector VectorMultiplication(const MyClassVector & _MyClassVectorObj) const;
		//перенос в начало координат
		void ToStart();
		//точка A в начале координат?
		bool isInStart() const;
		//~MyClassVector(void);
	};
}