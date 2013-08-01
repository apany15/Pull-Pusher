#include "MyClassGroup2DPrimitive.h"
#include<vector>
#include<algorithm>
namespace MY_NS_MODEL
{
	typedef std::vector<MyClassVertex *> MyTypeArrPointsVertecies;

	MyClassGroup2DPrimitive::MyClassGroup2DPrimitive(void)
	{
		arr2DPrimitive=0;
		num2DPrimitives=0;
		m_isClipped=false;
		o_cleaning=false;
		m_isSorted=false;
		m_normalsIsCalculated=false;
	}
	MyClassGroup2DPrimitive::MyClassGroup2DPrimitive(const MyClassGroup2DPrimitive & _MyClassGroup2DPrimitiveObj)
	{
		//не закреплены
		m_isClipped=false;

		arr2DPrimitive=0;
		num2DPrimitives=_MyClassGroup2DPrimitiveObj.num2DPrimitives;
		if (num2DPrimitives>0)
		{
			arr2DPrimitive=new MyClass2DPrimitive*[num2DPrimitives];
			for(unsigned int i=0; i<num2DPrimitives; i++)
			{
				arr2DPrimitive[i]=new MyClass2DPrimitive(*(_MyClassGroup2DPrimitiveObj.arr2DPrimitive[i]));
				arr2DPrimitive[i]->RegMessReceiver(this);
			}
		}
		FormVerticesNeighboursGroup();
		//если в передаваемом объекте вершины сгруппированы
		if (_MyClassGroup2DPrimitiveObj.m_isClipped)
		{
			//сделать с текущим объектом тоже
			ClipVerticesNeighboursGroup();
		}
		o_cleaning=false;
		m_isSorted=_MyClassGroup2DPrimitiveObj.IsSorted();
	}
	void MyClassGroup2DPrimitive::operator =(const MyClassGroup2DPrimitive & _MyClassGroup2DPrimitiveObj)
	{
		Clear();
		num2DPrimitives=_MyClassGroup2DPrimitiveObj.GetNum2DPrimitives();
		if (num2DPrimitives>0)
		{
			arr2DPrimitive=new MyClass2DPrimitive*[num2DPrimitives];
			for(unsigned int i=0; i<num2DPrimitives; i++)
			{
				arr2DPrimitive[i]=new MyClass2DPrimitive(*(_MyClassGroup2DPrimitiveObj[i]));
				arr2DPrimitive[i]->RegMessReceiver(this);
			}
			FormVerticesNeighboursGroup();
			if (_MyClassGroup2DPrimitiveObj.IsClipped())
			{
				//сделать с текущим объектом тоже
				ClipVerticesNeighboursGroup();
			}
			m_isSorted=_MyClassGroup2DPrimitiveObj.IsSorted();
		}
	}
	unsigned int MyClassGroup2DPrimitive::GetNum2DPrimitives()const
	{
		return num2DPrimitives;
	}
	MyClass2DPrimitive * MyClassGroup2DPrimitive::operator [](unsigned int num)
	{
		if (this->arr2DPrimitive==0) return 0;
		if (num>=num2DPrimitives) num=num2DPrimitives-1;
		return arr2DPrimitive[num];
	}
	const MyClass2DPrimitive * MyClassGroup2DPrimitive::operator [](unsigned int num)const
	{
		if (!arr2DPrimitive) return 0;
		if (num>=num2DPrimitives) num=num2DPrimitives-1;
		return arr2DPrimitive[num];
	}

	void MyClassGroup2DPrimitive::Add2DPrimitive(const MyClass2DPrimitive *_MyClass2DPrimitiveObj, int pos)
	{
		if (pos>((int)num2DPrimitives) || pos==-1) pos=num2DPrimitives;
		//разргуппировать
		//UnClipVerticesNeighboursGroup();
		//очитсить массив групп
		//ArrVerticesNeighboursGroup.clear();
		//m_isFormedVerticesNeighboursGroup=false;

		num2DPrimitives++;
		MyClass2DPrimitive **newArr=new MyClass2DPrimitive*[num2DPrimitives];
		int j=0;
		for (int i=0; i<(int)num2DPrimitives; i++)
		{
			if (i!=pos) newArr[i]=arr2DPrimitive[j++];
			else 
			{
				if (_MyClass2DPrimitiveObj) newArr[i]=new MyClass2DPrimitive(*_MyClass2DPrimitiveObj);
				else newArr[i]=new MyClass2DPrimitive();
				newArr[i]->RegMessReceiver(this);
				//пройти по всем группам и сравнить координаты с их лидером(нати группы к которой отностся вершины и поместить их туда)
				size_t s=ArrVerticesNeighboursGroup.size();
				unsigned int numVert=newArr[i]->GetNumVertices();
				for(unsigned int ii=0; ii<numVert; ii++)
				{
					//вершина ii
					bool inGroup=false;
					MyClassVertex *pV=&(newArr[i]->operator [](ii));
					for(size_t j=0; j<s; j++)
					{
						if (ArrVerticesNeighboursGroup[j].pLeadingVertex->EquialCoord(*pV))
						{
							inGroup=true;
							ArrVerticesNeighboursGroup[j].groupVertex.push_back(MyStrVertexInfo(i,ii,pV));
							pV->numGroupInGroup=(unsigned int)j;
						}
					}
					//если нет группы для данной вершины
					if (!inGroup)
					{
						//сообщаем вершине номер группы в которой она находится
						size_t s=ArrVerticesNeighboursGroup.size();
						pV->numGroupInGroup=(unsigned int)s;
						MyStrVerticesNeighboursGroup groupVerticesNeighbours;
						groupVerticesNeighbours.pLeadingVertex=pV;
						groupVerticesNeighbours.groupVertex.push_back(MyStrVertexInfo(i,ii,pV));
						ArrVerticesNeighboursGroup.push_back(groupVerticesNeighbours);
					}
				}

			}
		}
		if (arr2DPrimitive)	
		{
			if (num2DPrimitives==2) delete arr2DPrimitive;
			else delete [] arr2DPrimitive;
		}
		arr2DPrimitive=newArr;
		m_isSorted=false;
	}
	void MyClassGroup2DPrimitive::Add2DPrimitive(const MyClass2DPrimitive &_MyClass2DPrimitiveObj, int pos)
	{
		Add2DPrimitive(&_MyClass2DPrimitiveObj);
		return;
		//if (pos>((int)num2DPrimitives) || pos==-1) pos=num2DPrimitives;
		//////разргуппировать
		////UnClipVerticesNeighboursGroup();
		//////очитсить массив групп
		////ArrVerticesNeighboursGroup.clear();
		//////m_isFormedVerticesNeighboursGroup=false;

		//num2DPrimitives++;
		//MyClass2DPrimitive **newArr=new MyClass2DPrimitive*[num2DPrimitives];
		//int j=0;
		//for (int i=0; i<(int)num2DPrimitives; i++)
		//{
		//	if (i!=pos) newArr[i]=arr2DPrimitive[j++];
		//	else 
		//	{
		//		newArr[i]=new MyClass2DPrimitive(_MyClass2DPrimitiveObj);
		//		newArr[i]->RegMessReceiver(this);
		//		//пройти по всем группам и сравнить координаты с их лидером(нати группы к которой отностся вершины и поместить их туда)
		//		size_t s=ArrVerticesNeighboursGroup.size();
		//		unsigned int numVert=newArr[i]->GetNumVertices();
		//		for(unsigned int ii=0; ii<numVert; ii++)
		//		{
		//			//вершина ii
		//			bool inGroup=false;
		//			MyClassVertex *pV=&(newArr[i]->operator [](ii));
		//			for(size_t j=0; j<s; j++)
		//			{
		//				if (ArrVerticesNeighboursGroup[j].pLeadingVertex->EquialCoord(*pV))
		//				{
		//					inGroup=true;
		//					ArrVerticesNeighboursGroup[j].groupVertex.push_back(MyStrVertexInfo(i,ii,pV));
		//					pV->numGroupInGroup=j;
		//				}
		//			}
		//			//если нет группы для данной вершины
		//			if (!inGroup)
		//			{
		//				//сообщаем вершине номер группы в которой она находится
		//				size_t s=ArrVerticesNeighboursGroup.size();
		//				pV->numGroupInGroup=(unsigned int)s;
		//				MyStrVerticesNeighboursGroup groupVerticesNeighbours;
		//				groupVerticesNeighbours.pLeadingVertex=pV;
		//				groupVerticesNeighbours.groupVertex.push_back(MyStrVertexInfo(i,ii,pV));
		//				ArrVerticesNeighboursGroup.push_back(groupVerticesNeighbours);
		//			}
		//		}
		//	}
		//}
		//if (arr2DPrimitive)	
		//{
		//	if (num2DPrimitives==2) delete arr2DPrimitive;
		//	else delete [] arr2DPrimitive;
		//}
		//arr2DPrimitive=newArr;
		//m_isSorted=false;
	}

	void MyClassGroup2DPrimitive::Remove2DPrimitive(unsigned int pos)
	{
		if (num2DPrimitives==0) return;

		//разргуппировать
		//UnClipVerticesNeighboursGroup();
		//очитсить массив групп
		//ArrVerticesNeighboursGroup.clear();
		//m_isFormedVerticesNeighboursGroup=false;

		if (pos>(num2DPrimitives-1)) pos=num2DPrimitives-1;
		if (arr2DPrimitive) delete arr2DPrimitive[pos];

	}
	void MyClassGroup2DPrimitive::Clear()
	{
		if (arr2DPrimitive)	
		{
			o_cleaning=true;
			//разргуппировать
			UnClipVerticesNeighboursGroup();
			//очистить массив групп
			ArrVerticesNeighboursGroup.clear();
			//m_isFormedVerticesNeighboursGroup=false;

			for (unsigned int i=0; i<num2DPrimitives; i++)
				delete arr2DPrimitive[i];
			if (num2DPrimitives==1) delete arr2DPrimitive;
			else delete [] arr2DPrimitive;
			arr2DPrimitive=0;
			num2DPrimitives=0;
			//oldValidInfo.isValidating=true;
			//oldValidInfo.oldValidResult=false;
			o_cleaning=false;
			m_isSorted=false;
		}
	}
	////проверка на то что все примитивы соприкасаются хоть в одной точке
	bool MyClassGroup2DPrimitive::Validate()
	{
		for(unsigned int i=0; i<num2DPrimitives; i++)
			//если примитив сам по себе не валидный
			if (!(arr2DPrimitive[i]->Validate())) return false;
		return true;
	}
	void MyClassGroup2DPrimitive::MessageReseive(MyClassMessage _message, MyClassMessageSender * fromWho)
	{
		if (_message.message==change2DPrimitive)
		{
			if (!m_isClipped)
			{
				//исключаем изменившуюся вершину из ее группы и ищем ей другую(или создаем)
				//информация о вершине
				unsigned int num2DPrimitive;
				unsigned int numVertex;
				//указатель на вершину, которая изменилась
				MyClassVertex * pVertex=(MyClassVertex *)_message.data;
				//в какой группе вершин-соседей находилась вершина
				unsigned int numGroup=pVertex->numGroupInGroup;
				size_t s=ArrVerticesNeighboursGroup[numGroup].groupVertex.size();
				//найти вершину в группе группы
				MyTypeArrVertexInfo::iterator iterGroupGroup=ArrVerticesNeighboursGroup[numGroup].groupVertex.begin();
				for(size_t i=0; i<s; i++)
				{
					//если это она
					if (ArrVerticesNeighboursGroup[numGroup].groupVertex[i].pVertex==pVertex)
					{
						//запомнить данные вершины
						num2DPrimitive=ArrVerticesNeighboursGroup[numGroup].groupVertex[i].Num2DPrimitive;
						numVertex=ArrVerticesNeighboursGroup[numGroup].groupVertex[i].NumVertex;
						break;
					}
					iterGroupGroup++;
				}
				bool needRenum=false;					//требуется перенумеровка вершин т.к. была удалена целая группа
				//если в группе есть другие вершины
				if (s>1)
				{
					//если эта вершина была лидером
					if (ArrVerticesNeighboursGroup[numGroup].pLeadingVertex==pVertex)
					{
						//сделать лидером другую (следующую)
						ArrVerticesNeighboursGroup[numGroup].pLeadingVertex=ArrVerticesNeighboursGroup[numGroup].groupVertex[1].pVertex;
					}
					//удалить вершину из группы группы
					ArrVerticesNeighboursGroup[numGroup].groupVertex.erase(iterGroupGroup);
				}
				else	//удалить группу с указанием на перенумеровку вершин в остальных группах
				{
					MyTypeArrVerticesNeighboursGroup::iterator iter=ArrVerticesNeighboursGroup.begin()+numGroup;
					
					//for(unsigned int i=0; i<numGroup; i++, iter++);
					ArrVerticesNeighboursGroup.erase(iter);
					//потребуется перенумеровка вершин в последующих группах
					if (numGroup<ArrVerticesNeighboursGroup.size()) needRenum=true;
				}
				//поиск или создание новой группы для этой вершины
				bool find=false;
				size_t sizeBig=ArrVerticesNeighboursGroup.size();
				for(size_t i=0; i<sizeBig; i++)
				{
					//если нужна перенумеровка вершин
					if (needRenum && i>=numGroup)
					{
						size_t sizeSmall=ArrVerticesNeighboursGroup[i].groupVertex.size();
						for(size_t j=0; j<sizeSmall; j++)
							ArrVerticesNeighboursGroup[i].groupVertex[j].pVertex->numGroupInGroup=(unsigned int)i;
					}
					if (!find && ArrVerticesNeighboursGroup[i].pLeadingVertex->EquialCoord(*pVertex))
					{
						find=true;
						pVertex->numGroupInGroup=(unsigned int) i;
						//добавить в группу!
						ArrVerticesNeighboursGroup[i].groupVertex.push_back(MyStrVertexInfo(num2DPrimitive,numVertex,pVertex));
						if (!needRenum) break;
					}
				}
				if (!find)
				{
					pVertex->numGroupInGroup=(unsigned int) sizeBig;
					MyStrVerticesNeighboursGroup groupVerticesNeighbours;
					groupVerticesNeighbours.pLeadingVertex=pVertex;
					groupVerticesNeighbours.groupVertex.push_back(MyStrVertexInfo(num2DPrimitive,numVertex,pVertex));
					ArrVerticesNeighboursGroup.push_back(groupVerticesNeighbours);
				}
			}
			MessageSendToAll(MyClassMessage(changeGroup2DPrimitive,0));
		}
		if (_message.message==dieCompl)
		{
			if (o_oldIsClipped)
				ClipVerticesNeighboursGroup();
		}
		
		if (_message.message==die)
		{
			if (o_cleaning) return;
			//удалить указатель на этот примитив из списка примитивов
			if (num2DPrimitives==0) return;
			if (num2DPrimitives==1 && arr2DPrimitive[0]==fromWho)
			{
				delete arr2DPrimitive;
				arr2DPrimitive=0;
				num2DPrimitives=0;
				return;
			}
			MyClass2DPrimitive **temparr=new MyClass2DPrimitive*[num2DPrimitives-1];
			bool in=false;
			unsigned int j=0;
			for(unsigned int i=0; i<num2DPrimitives; i++)
				if (arr2DPrimitive[i]!=fromWho) 
				{
					if (j<num2DPrimitives-1) temparr[j++]=arr2DPrimitive[i];
				}
				else in=true;
			if (in)
			{	
				o_oldIsClipped=m_isClipped;
				UnClipVerticesNeighboursGroup();
				ArrVerticesNeighboursGroup.clear();
				//m_isFormedVerticesNeighboursGroup=false;
				if (num2DPrimitives==1) delete arr2DPrimitive;
				else delete [] arr2DPrimitive;
				arr2DPrimitive=temparr;
				num2DPrimitives--;
				MessageSendToAll(MyClassMessage(changeGroup2DPrimitive,0));
			}
			else
			{
				if (j==1) delete temparr;
				else delete [] temparr;
			}
		}
	}
	//const MyStrOldValidInfo & MyClassGroup2DPrimitive::GetOldValidInfo()const
	//{
	//	return oldValidInfo;
	//}

	void MyClassGroup2DPrimitive::CalcNormals()
	{
		if (num2DPrimitives==0) return;
		if (m_normalsIsCalculated) return;
		bool old_m_isClipped=m_isClipped;
		FormVerticesNeighboursGroup();
		UnClipVerticesNeighboursGroup();

		for (unsigned int i=0; i<num2DPrimitives; i++)
			arr2DPrimitive[i]->CalcNormals();


		
		MyTypeArrVerticesNeighboursGroup::iterator iter, iterEnd;
		iterEnd=ArrVerticesNeighboursGroup.end();
		MyTypeArrVertexInfo::iterator iterVertInfo, iterVertInfoEnd;
		for(iter=ArrVerticesNeighboursGroup.begin(); iter!=iterEnd; iter++)
		{
			iter->pLeadingVertex=(*iter).groupVertex.begin()->pVertex;
			iterVertInfoEnd=(*iter).groupVertex.end();
			for(iterVertInfo=(*iter).groupVertex.begin()+1; iterVertInfo!=iterVertInfoEnd; iterVertInfo++)
				iter->pLeadingVertex->GetNormal()->AveragToMe(iterVertInfo->pVertex->GetNormal());

			iter->pLeadingVertex->GetNormal()->Normalize();

			for(iterVertInfo=(*iter).groupVertex.begin()+1; iterVertInfo!=iterVertInfoEnd; iterVertInfo++)			
				iterVertInfo->pVertex->SetNormal(iter->pLeadingVertex->GetNormal());
		}
		if (old_m_isClipped) ClipVerticesNeighboursGroup();
		m_normalsIsCalculated=true;

	}
	void MyClassGroup2DPrimitive::FormVerticesNeighboursGroup()
	{
		if (num2DPrimitives==0) return;
		//if (m_isFormedVerticesNeighboursGroup) return;
		ArrVerticesNeighboursGroup.clear();
		MyTypeArrPointsVertecies arrVertEx;
		MyTypeArrPointsVertecies::iterator iter, iterEnd;
		MyStrVerticesNeighboursGroup groupVerticesNeighbours;
		unsigned int numGroup=0;
		MyClassVertex *pVert;
		MyClassVertex *pVert2;
		for (unsigned int i=0; i<num2DPrimitives; i++)
		{
			unsigned int numVetrecies=arr2DPrimitive[i]->GetNumVertices();	//кол-во вершин i-го примитива
			for(unsigned int j=0; j<numVetrecies; j++)
			{
				//обработка j-ой вершины i-го примитива
				pVert=&(*arr2DPrimitive[i])[j];
				//проверка на эта вершина не проверялась
				bool ex=false;
				unsigned int ttt=(unsigned int) arrVertEx.size();
				for(unsigned int vvv=0; vvv<ttt; vvv++)
					if (arrVertEx[vvv]==pVert)
					{
						ex=true;
						break;
					}
				//iterEnd=arrVertEx.end();
				//for(iter=arrVertEx.begin(); iter!=iterEnd; iter++)
				//	if ((*iter)==pVert)
				//	{
				//		ex=true;
				//		break;
				//	}
				if (ex) continue;
				

				//если не проверялась продолжаем
				arrVertEx.push_back(pVert);
				pVert->numGroupInGroup=numGroup;
				groupVerticesNeighbours.pLeadingVertex=pVert;
				groupVerticesNeighbours.groupVertex.push_back(MyStrVertexInfo(i,j,pVert));
				
				for(unsigned int n=i+1; n<num2DPrimitives; n++)
				{
					unsigned int numVetreciesOther=arr2DPrimitive[n]->GetNumVertices();	//кол-во вершин n-го примитива
					for(unsigned int m=0; m<numVetreciesOther; m++)
					{
						//обработка m-ой вершины n-го примитива
						pVert2=&(*arr2DPrimitive[n])[m];
						////проверка на эта вершина не проверялась
						//bool ex=false;
						//iterEnd=arrVertEx.end();
						//for(iter=arrVertEx.begin(); iter!=iterEnd; iter++)
						//	if ((*iter)==pVert2)
						//	{
						//		ex=true;
						//		break;
						//	}
						//if (ex) continue;
						//если не проверялась продолжаем
						if (pVert->EquialCoord(*pVert2))
						{
							pVert2->numGroupInGroup=numGroup;
							arrVertEx.push_back(pVert2);
							groupVerticesNeighbours.groupVertex.push_back(MyStrVertexInfo(n,m,pVert2));
						}
					}
				}
				//тут сформирована структура groupVerticesNeighbours
				ArrVerticesNeighboursGroup.push_back(groupVerticesNeighbours);
				numGroup++;
				groupVerticesNeighbours.Clear();
			}
		}
		//m_isFormedVerticesNeighboursGroup=true;
	}
	void MyClassGroup2DPrimitive::ClipVerticesNeighboursGroup()
	{
		if (num2DPrimitives==0) return;
		if (m_isClipped) return;
		//if (!m_isFormedVerticesNeighboursGroup) FormVerticesNeighboursGroup();
		unsigned int size=(unsigned int)ArrVerticesNeighboursGroup.size();
		for(unsigned int i=0; i<size; i++)
		{
			MyStrVerticesNeighboursGroup * pGroup=&(ArrVerticesNeighboursGroup[i]);
			pGroup->pLeadingVertex=pGroup->groupVertex[0].pVertex;
			//ArrVerticesNeighboursGroup[i].pLeadingVertex=ArrVerticesNeighboursGroup[i].groupVertex[0].pVertex;
			unsigned int sizeSize=(unsigned int)pGroup->groupVertex.size();
			for(unsigned int j=1; j<sizeSize; j++)
			{
				MyStrVertexInfo * pVI=&(pGroup->groupVertex[j]);
				*(GetPointPointVertex(arr2DPrimitive[pVI->Num2DPrimitive],pVI->NumVertex))=pGroup->pLeadingVertex;
				//сделать приемниками сообщений для лидирующей вершины все примыкающие примитивы
				//добыть указатель на примитив и скормить лидирующей вершине
				pGroup->pLeadingVertex->RegMessReceiver(arr2DPrimitive[pVI->Num2DPrimitive]);
			}
				
		}

		//MyTypeArrVerticesNeighboursGroup::iterator iter, iterEnd;
		//iterEnd=ArrVerticesNeighboursGroup.end();
		//MyTypeArrVertexInfo::iterator iterVertInfo, iterVertInfoEnd;
		//for(iter=ArrVerticesNeighboursGroup.begin(); iter!=iterEnd; iter++)
		//{
		//	iter->pLeadingVertex=(*iter).groupVertex.begin()->pVertex;
		//	iterVertInfoEnd=(*iter).groupVertex.end();
		//	for(iterVertInfo=(*iter).groupVertex.begin(); iterVertInfo!=iterVertInfoEnd; iterVertInfo++)
		//		//применяется дружественная функция к классу MyClass2DPrimitive
		//		*(GetPointPointVertex(arr2DPrimitive[iterVertInfo->Num2DPrimitive],iterVertInfo->NumVertex))=iter->pLeadingVertex;
		//}
		m_isClipped=true;
	}
	void MyClassGroup2DPrimitive::UnClipVerticesNeighboursGroup()
	{
		if (!m_isClipped) return;

		unsigned int ttt=(unsigned int)ArrVerticesNeighboursGroup.size();
		for(unsigned int i=0; i<ttt; i++)
		{
			MyStrVerticesNeighboursGroup * pGroup=&(ArrVerticesNeighboursGroup[i]);
			unsigned int ttt2=(unsigned int)pGroup->groupVertex.size();
			for(unsigned int j=1; j<ttt2; j++)
			{
				MyStrVertexInfo *pVI=&(pGroup->groupVertex[j]);
				MyClass2DPrimitive * pP=arr2DPrimitive[pVI->Num2DPrimitive];
				MyClassVertex * pV=pVI->pVertex;
				//применяется дружественная функция к классу MyClass2DPrimitive
				*(GetPointPointVertex(pP,pVI->NumVertex))=pV;
				pP->MessageReseive(unClipingNow,this);
				pV->SetCoord(pGroup->pLeadingVertex->GetCoord());
				pV->SetNormal(pGroup->pLeadingVertex->GetNormal());

				pGroup->pLeadingVertex->UnRegMessReceiver(pP);
			}
			pGroup->pLeadingVertex=0;
		}

		//MyTypeArrVerticesNeighboursGroup::iterator iter, iterEnd;
		//iterEnd=ArrVerticesNeighboursGroup.end();
		//MyTypeArrVertexInfo::iterator iterVertInfo, iterVertInfoEnd;
		//for(iter=ArrVerticesNeighboursGroup.begin(); iter!=iterEnd; iter++)
		//{
		//	iterVertInfoEnd=(*iter).groupVertex.end();
		//	for(iterVertInfo=(*iter).groupVertex.begin(); iterVertInfo!=iterVertInfoEnd; iterVertInfo++)
		//	{
		//		//применяется дружественная функция к классу MyClass2DPrimitive
		//		*(GetPointPointVertex(arr2DPrimitive[iterVertInfo->Num2DPrimitive],iterVertInfo->NumVertex))=iterVertInfo->pVertex;
		//		//предупреждаем примитив о разгруппировании вершин группы
		//		(*arr2DPrimitive[iterVertInfo->Num2DPrimitive]).MessageReseive(unClipingNow,this);
		//		(*arr2DPrimitive[iterVertInfo->Num2DPrimitive])[iterVertInfo->NumVertex].SetCoord(iter->pLeadingVertex->GetCoord());
		//		(*arr2DPrimitive[iterVertInfo->Num2DPrimitive])[iterVertInfo->NumVertex].SetNormal(iter->pLeadingVertex->GetNormal());
		//		//(*arr2DPrimitive[iterVertInfo->Num2DPrimitive])[iterVertInfo->NumVertex].normal=iter->pLeadingVertex->normal;// SetNormal(iter->pLeadingVertex->normal);
		//	}
		//	iter->pLeadingVertex=0;
		//}
		m_isClipped=false;
	}
	bool MyClassGroup2DPrimitive::IsClipped()const
	{
		return m_isClipped;
	}
	//bool MyClassGroup2DPrimitive::IsFormedVerticesNeighboursGroup()const
	//{
	//	return m_isFormedVerticesNeighboursGroup;
	//}
	bool MyClassGroup2DPrimitive::IsSorted()const
	{
		return m_isSorted;
	}
	const MyTypeArrVerticesNeighboursGroup * MyClassGroup2DPrimitive::GetPointVerticesNeighboursGroups()const
	{
		return &ArrVerticesNeighboursGroup;
	}

	void MyClassGroup2DPrimitive::Sort2DPrimitive()
	{
		if (m_isSorted) return;
		std::sort(arr2DPrimitive,arr2DPrimitive+num2DPrimitives,CompareForSort);
		m_isSorted=true;
	}
	bool MyClassGroup2DPrimitive::CompareForSort(MyClass2DPrimitive * a, MyClass2DPrimitive * b)
	{
		return (a->GetType()) < (b->GetType());
	}
	MyClassGroup2DPrimitive::~MyClassGroup2DPrimitive(void)
	{
		MessageSendToAll(MyClassMessage(die,0));	//предсмертный вопль объекту класса модели
		Clear();
	}

}
