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
	struct MyStrValidInfo
	{
		bool isValidated;
		bool ValidResult;
		MyStrValidInfo()
		{
			isValidated=false;
			ValidResult=false;
		}
		MyStrValidInfo(bool _isValidated, bool _ValidResult)
		{
			isValidated=_isValidated;
			ValidResult=_ValidResult;
		}
		//MyStrOldValidInfo(const MyStrOldValidInfo & _MyStrOldValidInfoObj)
		//{
		//	isValidating=_MyStrOldValidInfoObj.isValidating;
		//	oldValidResult=_MyStrOldValidInfoObj.oldValidResult;
		//}
	};
	class MyClass2DPrimitive;
	MyClassVertex ** GetPointPointVertex(MyClass2DPrimitive * _MyClass2DPrimitiveObj, unsigned int _numVertex);
	class MyClass2DPrimitive: public MyClassMessageSender, public MyClassMessageReceiver
	{
	public:

	protected:
		MyClassVertex **arrVertices;
		MyEnumPrimitive2DType type;
		unsigned int num_vertices;
		MyStrValidInfo ValidInfo;
		bool normalsIsAlreadyCalculated;
		//void changed();		//����� ���������� ��������� �� ��������� �������
		bool o_cleaning;	//���������, ������� � ��� ��� ���� ������ �������
		MyClassMessageSender * o_pdieVertex;
		bool o_unClipingNow;
	public:
		bool direction_CCW;
		MyClass2DPrimitive(MyEnumPrimitive2DType _type=triangle);
		MyClass2DPrimitive(unsigned int _numVertices);
		//����������� �����������
		MyClass2DPrimitive(const MyClass2DPrimitive & _MyClass2DPrimitiveObj);
		//�������� �����������
		MyClass2DPrimitive & operator = (const MyClass2DPrimitive & _MyClass2DPrimitiveObj);
		////�������� ���������
		//bool EquialToVerticesCoord (const MyClass2DPrimitive & _MyClass2DPrimitiveObj);
		MyEnumPrimitive2DType GetType() const;
		unsigned int GetNumVertices() const;
		MyClassVertex & operator[](unsigned int _numVertex);
		const MyClassVertex & operator[](unsigned int _numVertex) const;
		void CalcNormals();
		bool Validate();
		const MyStrValidInfo & GetValidInfo()const;
		~MyClass2DPrimitive(void);
		void MessageReseive(MyClassMessage _message, MyClassMessageSender * fromWho);
		bool NormalsIsCalculated()const;

		//����
		friend MyClassVertex ** GetPointPointVertex(MyClass2DPrimitive * _MyClass2DPrimitiveObj, unsigned int _numVertex);
	};
}
