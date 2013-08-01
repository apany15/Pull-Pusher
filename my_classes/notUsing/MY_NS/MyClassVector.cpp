#include "MyClassVector.h"
#include"MyClassNormal.h"
namespace MY_NS_MODEL
{
	MyClassVector::MyClassVector(void)
	{
		coordA[0]=0.0f;
		coordA[1]=0.0f;
		coordA[2]=0.0f;
		coordB[0]=0.0f;
		coordB[1]=0.0f;
		coordB[2]=0.0f;
	}
	MyClassVector::MyClassVector(const float *_coordB, const float *_coordA)
	{
		if (_coordA)
		{
			coordA[0]=_coordA[0];
			coordA[1]=_coordA[1];
			coordA[2]=_coordA[2];
		}
		else
		{
			coordA[0]=0.0f;
			coordA[1]=0.0f;
			coordA[2]=0.0f;
		}
		if (_coordB)
		{
			coordB[0]=_coordB[0];
			coordB[1]=_coordB[1];
			coordB[2]=_coordB[2];
		}
		else
		{
			coordB[0]=0.0f;
			coordB[1]=0.0f;
			coordB[2]=0.0f;
		}
	}

	void MyClassVector::SetCoordA(const float * _coordA)
	{
		if (_coordA)
		{
			coordA[0]=_coordA[0];
			coordA[1]=_coordA[1];
			coordA[2]=_coordA[2];
		}
	}
	void MyClassVector::SetCoordA(float _coordAX, float _coordAY, float _coordAZ)
	{
		coordA[0]=_coordAX;
		coordA[1]=_coordAY;
		coordA[2]=_coordAZ;
	}
	void MyClassVector::SetCoordB(const float * _coordB)
	{
		if (_coordB)
		{
			coordB[0]=_coordB[0];
			coordB[1]=_coordB[1];
			coordB[2]=_coordB[2];
		}
	}
	void MyClassVector::SetCoordB(float _coordBX, float _coordBY, float _coordBZ)
	{
		coordB[0]=_coordBX;
		coordB[1]=_coordBY;
		coordB[2]=_coordBZ;
	}

	//сравнение объектов
	bool MyClassVector::operator == (const MyClassVector & _vectorObj) const
	{
		bool result=(coordA[0]==_vectorObj.coordA[0] && coordA[1]==_vectorObj.coordA[1] && coordA[2]==_vectorObj.coordA[2]);
		result&=(coordB[0]==_vectorObj.coordB[0] && coordB[1]==_vectorObj.coordB[1] && coordB[2]==_vectorObj.coordB[2]);
		return result;
	}
	MyClassVector::operator MyClassNormal()
	{
		MyClassVector tResult=(*this);
		tResult.ToStart();
		MyClassNormal result;
		result.SetCoord(tResult.coordB);
		return result;
	}
	MyClassVector MyClassVector::VectorMultiplication(const MyClassVector & _MyClassVectorObj) const
	{
		MyClassVector vA, vB;
		vA=(*this);
		vB=_MyClassVectorObj;
		MyClassVector result;
		if (vA.coordA[0]==vB.coordA[0] && vA.coordA[1]==vB.coordA[1] && vA.coordA[2]==vB.coordA[2])
		{
			result.coordA[0]=vA.coordA[0];
			result.coordA[1]=vA.coordA[1];
			result.coordA[2]=vA.coordA[2];
			vA.ToStart();
			vB.ToStart();
			result.coordB[0]=vA.coordB[1]*vB.coordB[2]-vA.coordB[2]*vB.coordB[1]+result.coordA[0];
			result.coordB[1]=vA.coordB[2]*vB.coordB[0]-vA.coordB[0]*vB.coordB[2]+result.coordA[1];
			result.coordB[2]=vA.coordB[0]*vB.coordB[1]-vA.coordB[1]*vB.coordB[0]+result.coordA[2];
		}
		else
		{
			vA.ToStart();
			vB.ToStart();
			result.coordB[0]=vA.coordB[1]*vB.coordB[2]-vA.coordB[2]*vB.coordB[1];
			result.coordB[1]=vA.coordB[2]*vB.coordB[0]-vA.coordB[0]*vB.coordB[2];
			result.coordB[2]=vA.coordB[0]*vB.coordB[1]-vA.coordB[1]*vB.coordB[0];
		}
		return result;
	}
	void MyClassVector::ToStart()
	{
		coordB[0]-=coordA[0];
		coordB[1]-=coordA[1];
		coordB[2]-=coordA[2];
		coordA[0]=0.0f;
		coordA[1]=0.0f;
		coordA[2]=0.0f;
	}
	bool MyClassVector::isInStart() const
	{
		return (coordA[0]==0.0f && coordA[1]==0.0f && coordA[2]==0.0f);
	}
	//MyClassVector::~MyClassVector(void)
	//{
	//}
}
