#include "MyClassNormal.h"
#include <math.h>
#include "MyClassVector.h"
namespace MY_NS_MODEL
{
	MyClassNormal::MyClassNormal(const float *_coord)
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
	}
	MyClassNormal::MyClassNormal(float _coordX, float _coordY, float _coordZ)
	{
		coord[0]=_coordX;
		coord[1]=_coordY;
		coord[2]=_coordZ;
	}

	const float * MyClassNormal::GetCoord()const
	{
		return coord;
	}
	void MyClassNormal::SetCoord(const float * _coord)
	{
		if (_coord)
		{
			coord[0]=_coord[0];
			coord[1]=_coord[1];
			coord[2]=_coord[2];
		}
	}
	void MyClassNormal::SetCoord(float coordX, float coordY, float coordZ)
	{
			coord[0]=coordX;
			coord[1]=coordY;
			coord[2]=coordZ;
	}

	bool MyClassNormal::operator == (const MyClassNormal & _normalObj) const
	{
		return ((coord[0]==_normalObj.coord[0]) && (coord[1]==_normalObj.coord[1]) && (coord[2]==_normalObj.coord[2]));
	}

	MyClassNormal::operator MyClassVector()
	{
		MyClassVector result;
		result.SetCoordB(this->coord);
		return result;
	}

	//MyClassNormal::operator float*()
	//{
	//	return coord;
	//}
	//MyClassNormal::operator const float*() const
	//{
	//	return coord;
	//}

	bool MyClassNormal::IsNormalized()
	{
		return sqrt(coord[0]*coord[0]+coord[1]*coord[1]+coord[2]*coord[2])==1;
	}
	void MyClassNormal::Normalize()
	{
		float l=sqrt(coord[0]*coord[0]+coord[1]*coord[1]+coord[2]*coord[2]);
		coord[0]/=l;
		coord[1]/=l;
		coord[2]/=l;
	}
	const float * MyClassNormal::operator = (const float * _floatObj)
	{
		coord[0]=_floatObj[0];
		coord[1]=_floatObj[1];
		coord[2]=_floatObj[2];
		return coord;
	}
	float & MyClassNormal::operator[](unsigned int num)
	{
		if (num>2) return coord[2];
		else return coord[num];
	}
	const float & MyClassNormal::operator[](unsigned int num)const
	{
		if (num>2) return coord[2];
		else return coord[num];
	}

	MyClassNormal MyClassNormal::Averaging(const MyClassNormal & _MyClassNormalObj) const
	{
		MyClassNormal result;
		result.coord[0]+=_MyClassNormalObj.coord[0];
		result.coord[1]+=_MyClassNormalObj.coord[1];
		result.coord[2]+=_MyClassNormalObj.coord[2];

		//result.coord[0]=(coord[0]+_MyClassNormalObj.coord[0])/2;
		//result.coord[1]=(coord[1]+_MyClassNormalObj.coord[1])/2;
		//result.coord[2]=(coord[2]+_MyClassNormalObj.coord[2])/2;
		//result.Normalize();
		return result;
	}
	void MyClassNormal::AveragToMe(const MyClassNormal & _MyClassNormalObj)
	{
		coord[0]+=_MyClassNormalObj.coord[0];
		coord[1]+=_MyClassNormalObj.coord[1];
		coord[2]+=_MyClassNormalObj.coord[2];

		//coord[0]=(coord[0]+_MyClassNormalObj.coord[0])/2;
		//coord[1]=(coord[1]+_MyClassNormalObj.coord[1])/2;
		//coord[2]=(coord[2]+_MyClassNormalObj.coord[2])/2;
		//Normalize();
	}
	void MyClassNormal::AveragToMe(const MyClassNormal * _MyClassNormalObj)
	{
		if (_MyClassNormalObj)
		{
			coord[0]+=_MyClassNormalObj->coord[0];
			coord[1]+=_MyClassNormalObj->coord[1];
			coord[2]+=_MyClassNormalObj->coord[2];
			//coord[0]=(coord[0]+_MyClassNormalObj->coord[0])/2;
			//coord[1]=(coord[1]+_MyClassNormalObj->coord[1])/2;
			//coord[2]=(coord[2]+_MyClassNormalObj->coord[2])/2;
			//Normalize();
		}
	}
	//MyClassNormal MyClassNormal::VectorMultToMe(const MyClassNormal & _MyClassNormalObj) const
	//{
	//	MyClassNormal result;
	//	result.coord[0]=coord[1]*_MyClassNormalObj.coord[2]-coord[2]*_MyClassNormalObj.coord[1];
	//	result.coord[1]=coord[2]*_MyClassNormalObj.coord[0]-coord[0]*_MyClassNormalObj.coord[2];
	//	result.coord[2]=coord[0]*_MyClassNormalObj.coord[1]-coord[1]*_MyClassNormalObj.coord[0];
	//	return result;
	//}
}