#pragma once
#include "MyClassGroup2DPrimitive.h"
#include "MyClassMessageSender.h"
#include<vector>

namespace MY_NS_MODEL
{

	typedef std::vector<MyClassGroup2DPrimitive *> MyTypeVectorPointGroup2DPrimitive;
	typedef std::vector<std::vector<MyClassVertex *>> MyTypeArrayArrayLeadingVertecies;
	class MyClassModelBase: public MyClassMessageSender, public MyClassMessageReceiver
	{
	protected:
		MyTypeVectorPointGroup2DPrimitive arrPointGroup2DPrimitive;	//массив групп
		//MyTypeVerticesNeighboursGroupV2 VerticesNeighboursGroupV2;	//массив групп вершин-соседей
		bool o_cleaning;	//служебный, говорит о том что идет полная очистка (для ускорения удаления всех объектов MyClassGroup2DPrimitive)
		MyTypeArrayArrayLeadingVertecies ArrayArrayLeadingVertecies;
		bool m_isFormedVerticesNeighboursGroups;
		bool m_normalsIsCalculated;
	public:
		MyClassModelBase(void);
		MyClassModelBase(const MyClassModelBase & _MyClassModelBaseObj);
		MyClassGroup2DPrimitive * operator [] (unsigned int num);
		const MyClassGroup2DPrimitive * operator [] (unsigned int num)const;
		unsigned int GetNumGroup()const;
		void operator = (const MyClassModelBase & _MyClassModelBaseObj);
		void AddGroup(const MyClassGroup2DPrimitive * _MyClassGroup2DPrimitiveObj=0);
		void AddGroup(const MyClassGroup2DPrimitive & _MyClassGroup2DPrimitiveObj);
		void RemoveGroup(unsigned int num);
		void Clear();
	protected:
		void FormVerticesNeighboursGroups();
	public:
		
		void CalcNormals();
		void MessageReseive(MyClassMessage _message, MyClassMessageSender * fromWho);
		~MyClassModelBase(void);
		
	};

}
