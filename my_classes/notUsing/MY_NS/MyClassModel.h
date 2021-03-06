#pragma once
#include "MyClassGroup2DPrimitive.h"
#include "MyClassMessageSender.h"
#include<vector>

namespace MY_NS_MODEL
{

	typedef std::vector<MyClassGroup2DPrimitive *> MyTypeVectorPointGroup2DPrimitive;
	typedef std::vector<std::vector<MyClassVertex *>> MyTypeArrayArrayLeadingVertecies;
	class MyClassModel: public MyClassMessageSender, public MyClassMessageReceiver
	{
	protected:
		MyTypeVectorPointGroup2DPrimitive arrPointGroup2DPrimitive;	//������ �����
		//MyTypeVerticesNeighboursGroupV2 VerticesNeighboursGroupV2;	//������ ����� ������-�������
		bool o_cleaning;	//���������, ������� � ��� ��� ���� ������ ������� (��� ��������� �������� ���� �������� MyClassGroup2DPrimitive)
		MyTypeArrayArrayLeadingVertecies ArrayArrayLeadingVertecies;
		bool m_isFormedVerticesNeighboursGroups;
		bool m_normalsIsCalculated;
	public:
		MyClassModel(void);
		MyClassModel(const MyClassModel & _MyClassModelObj);
		MyClassGroup2DPrimitive * operator [] (unsigned int num);
		const MyClassGroup2DPrimitive * operator [] (unsigned int num)const;
		unsigned int GetNumGroup()const;
		void operator = (const MyClassModel & _MyClassModelObj);
		void AddGroup(const MyClassGroup2DPrimitive * _MyClassGroup2DPrimitiveObj=0);
		void AddGroup(const MyClassGroup2DPrimitive & _MyClassGroup2DPrimitiveObj);
		void RemoveGroup(unsigned int num);
		void Clear();
	protected:
		void FormVerticesNeighboursGroups();
	public:
		
		void CalcNormals();
		void MessageReseive(MyClassMessage _message, MyClassMessageSender * fromWho);
		~MyClassModel(void);
		//���������������
		void UnRegAllMessReceiver();
		void UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj);

		
	};

}
