#pragma once
#include "MyClassVertex.h"
#include "MyClassMaterial.h"
#include "MyClassMessageSender.h"
#include "MyClassMessageReceiver.h"
#include<vector>
namespace MY_NS_MODEL
{
	enum MyEnumPrimitive2DType
	{
		triangle=0,	//����� ���������� 3
		quad=1,		//����� ���������� 4
		polygon=2	//����� ���������� �� 5
	};
	struct MyStrOldValidInfo
	{
		bool isValidating;
		bool oldValidResult;
		MyStrOldValidInfo()
		{
			isValidating=false;
			oldValidResult=false;
		}
		MyStrOldValidInfo(bool _isValidating, bool _oldValidResult)
		{
			isValidating=_isValidating;
			oldValidResult=_oldValidResult;
		}
		//MyStrOldValidInfo(const MyStrOldValidInfo & _MyStrOldValidInfoObj)
		//{
		//	isValidating=_MyStrOldValidInfoObj.isValidating;
		//	oldValidResult=_MyStrOldValidInfoObj.oldValidResult;
		//}
	};
	class MyClass2DPrimitiveBase;
	MyClassVertex ** GetPointPointVertex(MyClass2DPrimitiveBase * _MyClass2DPrimitiveBaseObj, unsigned int _numVertex);
	class MyClass2DPrimitiveBase: public MyClassMessageSender, public MyClassMessageReceiver
	{
	public:

	protected:
		MyClassVertex **arrVertices;
		MyEnumPrimitive2DType type;
		unsigned int num_vertices;
		MyStrOldValidInfo oldValidInfo;
		bool normalsIsAlreadyCalculated;
		//void changed();		//����� ���������� ��������� �� ��������� �������
		bool o_cleaning;	//���������, ������� � ��� ��� ���� ������ �������
		MyClassMessageSender * o_pdieVertex;
		bool o_unClipingNow;
	public:
		bool direction_CCW;
		MyClass2DPrimitiveBase(MyEnumPrimitive2DType _type=triangle);
		MyClass2DPrimitiveBase(unsigned int _numVertices);
		//����������� �����������
		MyClass2DPrimitiveBase(const MyClass2DPrimitiveBase & _MyClass2DPrimitiveBaseObj);
		//�������� �����������
		MyClass2DPrimitiveBase & operator = (const MyClass2DPrimitiveBase & _MyClass2DPrimitiveBaseObj);
		////�������� ���������
		//bool EquialToVerticesCoord (const MyClass2DPrimitiveBase & _MyClass2DPrimitiveBaseObj);
		MyEnumPrimitive2DType GetType() const;
		unsigned int GetNumVertices() const;
		MyClassVertex & operator[](unsigned int _numVertex);
		const MyClassVertex & operator[](unsigned int _numVertex) const;
		void CalcNormals();
		bool Validate();
		const MyStrOldValidInfo & GetOldValidInfo()const;
		~MyClass2DPrimitiveBase(void);
		void MessageReseive(MyClassMessage _message, MyClassMessageSender * fromWho);
		bool NormalsIsCalculated()const;

		//����
		friend MyClassVertex ** GetPointPointVertex(MyClass2DPrimitiveBase * _MyClass2DPrimitiveBaseObj, unsigned int _numVertex);
	};
}
