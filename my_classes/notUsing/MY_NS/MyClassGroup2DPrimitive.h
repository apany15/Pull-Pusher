#pragma once
#include "MyClass2DPrimitive.h"
#include "MyClassMaterial.h"
#include "MyClassMessageReceiver.h"
#include<vector>
namespace MY_NS_MODEL
{

	struct MyStrVertexInfo
	{
		unsigned int Num2DPrimitive;	//����� ��������� � ������
		unsigned int NumVertex;		//����� ������� � ���������
		MyClassVertex *pVertex;		//��������� �� �������
		MyStrVertexInfo(unsigned int _Num2DPrimitive=0, unsigned int _NumVertex=0, MyClassVertex *_pVertex=0)
			:Num2DPrimitive(_Num2DPrimitive),
			NumVertex(_NumVertex),
			pVertex(_pVertex)
		{}
	};
	typedef vector<MyStrVertexInfo> MyTypeArrVertexInfo;
	struct MyStrVerticesNeighboursGroup
	{
		MyClassVertex *pLeadingVertex;		//������� �������
		MyTypeArrVertexInfo groupVertex;	//������ ���� ������
		MyStrVerticesNeighboursGroup()
			:pLeadingVertex(0)
		{}
		void Clear()
		{
			groupVertex.clear();
			pLeadingVertex=0;
		}
	};


	typedef vector<MyStrVerticesNeighboursGroup> MyTypeArrVerticesNeighboursGroup;

	class MyClassGroup2DPrimitive: public MyClassMessageReceiver, public MyClassMessageSender
	{
	public:
		MyClassMaterial material;
	protected:
		MyClass2DPrimitive **arr2DPrimitive;
		unsigned int num2DPrimitives;
		//MyStrOldValidInfo oldValidInfo;
		bool m_isClipped;
		bool o_oldIsClipped;
		MyTypeArrVerticesNeighboursGroup ArrVerticesNeighboursGroup;	//������ ����� ������-�������
		//bool m_isFormedVerticesNeighboursGroup;
		bool m_isSorted;
		bool m_normalsIsCalculated;
		bool o_cleaning;	//���������, ������� � ��� ��� ���� ������ ������� (��� ��������� �������� ���� �������� MyClass2DPrimitive)
	public:
		MyClassGroup2DPrimitive(void);
		MyClassGroup2DPrimitive(const MyClassGroup2DPrimitive & _MyClassGroup2DPrimitiveObj);
		//��������� ����������
		void operator =(const MyClassGroup2DPrimitive & _MyClassGroup2DPrimitiveObj);
		unsigned int GetNum2DPrimitives()const;
		MyClass2DPrimitive * operator [](unsigned int num);
		const MyClass2DPrimitive * operator [](unsigned int num)const;
		void Add2DPrimitive(const MyClass2DPrimitive *_MyClass2DPrimitiveObj=0, int pos=-1);
		void Add2DPrimitive(const MyClass2DPrimitive &_MyClass2DPrimitiveObj, int pos=-1);

		void Remove2DPrimitive(unsigned int pos);
		void Clear();
		bool Validate();
		void MessageReseive(MyClassMessage _message, MyClassMessageSender * fromWho);
		//const MyStrOldValidInfo & GetOldValidInfo()const;
		void CalcNormals();
	protected:
		//����������� ����� ������-�������
		void FormVerticesNeighboursGroup();
	public:
		//���������� ������
		void ClipVerticesNeighboursGroup();
		void UnClipVerticesNeighboursGroup();
		bool IsClipped()const;
		/*bool IsFormedVerticesNeighboursGroup()const;*/
		bool IsSorted()const;
		const MyTypeArrVerticesNeighboursGroup * GetPointVerticesNeighboursGroups()const;
		void Sort2DPrimitive();
	protected:
		static bool CompareForSort(MyClass2DPrimitive * a, MyClass2DPrimitive * b);
	public:
		~MyClassGroup2DPrimitive(void);

	};
}
