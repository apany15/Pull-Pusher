#include "MyClassModelBase.h"
namespace MY_NS_MODEL
{
	MyClassModelBase::MyClassModelBase(void)
	{
		o_cleaning=false;
		m_isFormedVerticesNeighboursGroups=false;
		m_normalsIsCalculated=false;
	}
	MyClassModelBase::MyClassModelBase(const MyClassModelBase & _MyClassModelBaseObj)
	{
		MyClassGroup2DPrimitive * tP;
		unsigned int size=(unsigned int) arrPointGroup2DPrimitive.size();
		for (unsigned int i=0; i<size; i++)
		{
			tP=new MyClassGroup2DPrimitive(*(_MyClassModelBaseObj.arrPointGroup2DPrimitive[i]));
			tP->RegMessReceiver(this);
			arrPointGroup2DPrimitive.push_back(tP);
		}
		o_cleaning=false;
		m_isFormedVerticesNeighboursGroups=false;
		m_normalsIsCalculated=false;
	}
	MyClassGroup2DPrimitive * MyClassModelBase::operator [] (unsigned int num)
	{
		unsigned int size=(unsigned int) arrPointGroup2DPrimitive.size();
		if (size==0) return 0;
		if (num>=size) num=size-1;
		return arrPointGroup2DPrimitive[num];
	}
	const MyClassGroup2DPrimitive * MyClassModelBase::operator [] (unsigned int num)const
	{
		unsigned int size=(unsigned int) arrPointGroup2DPrimitive.size();
		if (size==0) return 0;
		if (num>=size) num=size-1;
		return arrPointGroup2DPrimitive[num];
	}
	unsigned int MyClassModelBase::GetNumGroup()const
	{
		return (unsigned int) arrPointGroup2DPrimitive.size();
	}
	void MyClassModelBase::operator = (const MyClassModelBase & _MyClassModelBaseObj)
	{
		Clear();
		MyClassGroup2DPrimitive * tP;
		unsigned int size=(unsigned int) arrPointGroup2DPrimitive.size();
		for (unsigned int i=0; i<size; i++)
		{
			tP=new MyClassGroup2DPrimitive(*(_MyClassModelBaseObj.arrPointGroup2DPrimitive[i]));
			tP->RegMessReceiver(this);
			arrPointGroup2DPrimitive.push_back(tP);
		}
		m_isFormedVerticesNeighboursGroups=false;
		m_normalsIsCalculated=false;
	}
	void MyClassModelBase::AddGroup(const MyClassGroup2DPrimitive * _MyClassGroup2DPrimitiveObj)
	{
		MyClassGroup2DPrimitive * tP;
		if (_MyClassGroup2DPrimitiveObj) tP=new MyClassGroup2DPrimitive(*_MyClassGroup2DPrimitiveObj);
		else tP=new MyClassGroup2DPrimitive();
		tP->RegMessReceiver(this);
		arrPointGroup2DPrimitive.push_back(tP);
		m_isFormedVerticesNeighboursGroups=false;
		m_normalsIsCalculated=false;
	}
	void MyClassModelBase::AddGroup(const MyClassGroup2DPrimitive & _MyClassGroup2DPrimitiveObj)
	{
		MyClassGroup2DPrimitive * tP=new MyClassGroup2DPrimitive(_MyClassGroup2DPrimitiveObj); ;
		tP->RegMessReceiver(this);
		arrPointGroup2DPrimitive.push_back(tP);
		m_isFormedVerticesNeighboursGroups=false;
		m_normalsIsCalculated=false;
	}
	void MyClassModelBase::RemoveGroup(unsigned int num)
	{
		unsigned int size=(unsigned int) arrPointGroup2DPrimitive.size();
		if (size==0) return;
		if (num>=size) num=size-1;
		//MyTypeVectorPointGroup2DPrimitive::iterator iter=arrPointGroup2DPrimitive.begin();
		//for(unsigned int i=0; i<num; i++) iter++;
		delete arrPointGroup2DPrimitive[num];
		//arrPointGroup2DPrimitive.erase(iter);
		m_isFormedVerticesNeighboursGroups=false;
		m_normalsIsCalculated=false;
	}
	void MyClassModelBase::Clear()
	{
		o_cleaning=true;
		MyTypeVectorPointGroup2DPrimitive::iterator iterEnd=arrPointGroup2DPrimitive.end();
		for(MyTypeVectorPointGroup2DPrimitive::iterator iter=arrPointGroup2DPrimitive.begin(); iter!=iterEnd; iter++)
			delete *iter;
		arrPointGroup2DPrimitive.clear();
		o_cleaning=false;
		m_isFormedVerticesNeighboursGroups=false;
		m_normalsIsCalculated=false;
	}

	void MyClassModelBase::FormVerticesNeighboursGroups()
	{
		if (m_isFormedVerticesNeighboursGroups) return;
		ArrayArrayLeadingVertecies.clear();
		std::vector<MyClassVertex *> ArrayLeadingVertecies;
		//просчитываем нормали для всех групп и скрепляем их, запоминая их предидущее состояние
		vector<bool> arrInfoClipGroup;

		unsigned int ttt=(unsigned int)arrPointGroup2DPrimitive.size();
		for(unsigned int i=0; i<ttt; i++)
		{
			arrInfoClipGroup.push_back(arrPointGroup2DPrimitive[i]->IsClipped());
			arrPointGroup2DPrimitive[i]->ClipVerticesNeighboursGroup();
		}

		//MyTypeVectorPointGroup2DPrimitive::iterator iterEnd=arrPointGroup2DPrimitive.end();
		//for(MyTypeVectorPointGroup2DPrimitive::iterator iter=arrPointGroup2DPrimitive.begin(); iter!=iterEnd; iter++)
		//{
		//	arrInfoClipGroup.push_back((*iter)->IsClipped());
		//	(*iter)->ClipVerticesNeighboursGroup();
		//}
		vector<MyClassVertex *> arrVertEx;
		unsigned int numGroup=(unsigned int) arrPointGroup2DPrimitive.size();
		for(unsigned int i=0; i<numGroup; i++)
		{
			const MyTypeArrVerticesNeighboursGroup * tP1=arrPointGroup2DPrimitive[i]->GetPointVerticesNeighboursGroups();
			unsigned int numGroupGroup=(unsigned int) tP1->size();
			for(unsigned int j=0; j<numGroupGroup; j++)
			{
				//обработка лидера j-ой группы вершин i-й группы примитивов
				//(*tP1)[j].pLeadingVertex;
				bool exM=false;
				unsigned int ttt=arrVertEx.size();
				for(unsigned int ii=0; ii<ttt; ii++)
					if (arrVertEx[ii]==(*tP1)[j].pLeadingVertex)
					{
						exM=true;
						break;
					}
				if (exM) continue;
				//ArrayLeadingVertecies.clear();

				for(unsigned int m=i+1; m<numGroup; m++)
				{
					const MyTypeArrVerticesNeighboursGroup * tP2=arrPointGroup2DPrimitive[m]->GetPointVerticesNeighboursGroups();
					unsigned int numGroupGroup=(unsigned int) tP2->size();
					for(unsigned int n=0; n<numGroupGroup; n++)
					{
						//обработка лидера n-ой группы вершин m-й группы примитивов
						//(*tP2)[n].pLeadingVertex;

						//если координаты совпадают
						if ((*tP1)[j].pLeadingVertex->EquialCoord(*((*tP2)[n].pLeadingVertex)))
						{
							if (!exM)
							{
								exM=true;
								ArrayLeadingVertecies.push_back((*tP1)[j].pLeadingVertex);
							}
							arrVertEx.push_back((*tP2)[n].pLeadingVertex);
							ArrayLeadingVertecies.push_back((*tP2)[n].pLeadingVertex);
						}
					}
				}
				if (exM) 
				{
					ArrayArrayLeadingVertecies.push_back(ArrayLeadingVertecies);
					ArrayLeadingVertecies.clear();
				}
			}
		}

		//раскрепляем те что были раскреплены до
		for(unsigned int i=0; i<numGroup; i++)
			if (!arrInfoClipGroup[i]) arrPointGroup2DPrimitive[i]->UnClipVerticesNeighboursGroup();
		m_isFormedVerticesNeighboursGroups=true;
	}




	void MyClassModelBase::CalcNormals()
	{
		if (m_normalsIsCalculated) return;
		//просчитываем нормали для всех групп и скрепляем их, запоминая их предидущее состояние
		vector<bool> arrInfoClipGroup;
		MyTypeVectorPointGroup2DPrimitive::iterator iterEnd=arrPointGroup2DPrimitive.end();
		for(MyTypeVectorPointGroup2DPrimitive::iterator iter=arrPointGroup2DPrimitive.begin(); iter!=iterEnd; iter++)
		{
			(*iter)->CalcNormals();
			arrInfoClipGroup.push_back((*iter)->IsClipped());
			(*iter)->ClipVerticesNeighboursGroup();
		}

		FormVerticesNeighboursGroups();

		MyTypeArrayArrayLeadingVertecies::iterator iterArrArrEnd=ArrayArrayLeadingVertecies.end();
		for(MyTypeArrayArrayLeadingVertecies::iterator iter=ArrayArrayLeadingVertecies.begin(); iter!=iterArrArrEnd; iter++)
		{
			std::vector<MyClassVertex *>::iterator iterIterEnd=iter->end();
			std::vector<MyClassVertex *>::iterator iterIterBegin=iter->begin();
			for(std::vector<MyClassVertex *>::iterator iterIter=iterIterBegin+1; iterIter!=iterIterEnd; iterIter++)
				(*iterIterBegin)->GetNormal()->AveragToMe((*iterIter)->GetNormal());
			(*iterIterBegin)->GetNormal()->Normalize();
			for(std::vector<MyClassVertex *>::iterator iterIter=iterIterBegin+1; iterIter!=iterIterEnd; iterIter++)
				(*iterIter)->SetNormal((*iterIterBegin)->GetNormal());
		}
		unsigned int numGroup=(unsigned int) arrPointGroup2DPrimitive.size();
		//раскрепляем те что были раскреплены до
		for(unsigned int i=0; i<numGroup; i++)
			if (!arrInfoClipGroup[i]) arrPointGroup2DPrimitive[i]->UnClipVerticesNeighboursGroup();
		m_normalsIsCalculated=true;
	}
	void MyClassModelBase::MessageReseive(MyClassMessage _message, MyClassMessageSender * fromWho)
	{
		if (_message.message==die)
		{
			if (o_cleaning) return;
			//поиск указателя на группу в массиве и удаление от туда
			MyTypeVectorPointGroup2DPrimitive::iterator iter=arrPointGroup2DPrimitive.begin();
			MyTypeVectorPointGroup2DPrimitive::iterator iterEnd=arrPointGroup2DPrimitive.end();
			for(iter=arrPointGroup2DPrimitive.begin(); iter!=iterEnd; iter++)
				if ((*iter)==fromWho)
				{
					arrPointGroup2DPrimitive.erase(iter);
					m_isFormedVerticesNeighboursGroups=false;
					m_normalsIsCalculated=false;
					break;
				}
		}
		if (_message.message==changeGroup2DPrimitive)
		{
			m_isFormedVerticesNeighboursGroups=false;
			m_normalsIsCalculated=false;
		}
		
	}
	MyClassModelBase::~MyClassModelBase(void)
	{
		MessageSendToAll(MyClassMessage(die,0));	//предсмертный вопль
		Clear();
	}
}
