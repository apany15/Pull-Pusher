#include "MyClass2DPrimitiveBase.h"
#include "MyClassNormal.h"
#include "MyClassVector.h"

namespace MY_NS_MODEL
{
	MyClass2DPrimitiveBase::MyClass2DPrimitiveBase(MyEnumPrimitive2DType _type)
	{
		o_cleaning=false;
		o_pdieVertex=0;
		if (_type==triangle) num_vertices=3;
		if (_type==quad) num_vertices=4;
		if (_type==polygon) num_vertices=5;
		arrVertices=new MyClassVertex*[num_vertices];
		for (unsigned int i=0; i<num_vertices; i++)
		{
			arrVertices[i]=new MyClassVertex();
			arrVertices[i]->RegMessReceiver(this);
		}
		type=_type;
		direction_CCW=true;
		normalsIsAlreadyCalculated=false;
		o_unClipingNow=false;
	}
	MyClass2DPrimitiveBase::MyClass2DPrimitiveBase(unsigned int _numVertices)
	{
		o_cleaning=false;
		o_pdieVertex=0;
		if (_numVertices<4)
		{
			type=triangle;
			_numVertices=3;
		}
		if (_numVertices==4) type=quad;
		if (_numVertices>4) type=polygon;
		num_vertices=_numVertices;
		arrVertices=new MyClassVertex*[num_vertices];
		for (unsigned int i=0; i<num_vertices; i++)
		{
			arrVertices[i]=new MyClassVertex();
			arrVertices[i]->RegMessReceiver(this);
		}
		direction_CCW=true;
		normalsIsAlreadyCalculated=false;
		o_unClipingNow=false;
	}
	//конструктор копирования
	MyClass2DPrimitiveBase::MyClass2DPrimitiveBase(const MyClass2DPrimitiveBase & _MyClass2DPrimitiveBaseObj)
		//:MyClassMessageSender(_MyClass2DPrimitiveBaseObj)
	{
		o_cleaning=false;
		o_pdieVertex=0;
		type=_MyClass2DPrimitiveBaseObj.type;
		num_vertices=_MyClass2DPrimitiveBaseObj.num_vertices;
		arrVertices=new MyClassVertex*[num_vertices];
		for (unsigned int i=0; i<num_vertices; i++)
		{
			arrVertices[i]=new MyClassVertex(*(_MyClass2DPrimitiveBaseObj.arrVertices[i]));
			arrVertices[i]->RegMessReceiver(this);
		}
		oldValidInfo=_MyClass2DPrimitiveBaseObj.GetOldValidInfo();
		direction_CCW=_MyClass2DPrimitiveBaseObj.direction_CCW;
		normalsIsAlreadyCalculated=_MyClass2DPrimitiveBaseObj.NormalsIsCalculated();
		o_unClipingNow=false;
	}
	//оператор копирования
	MyClass2DPrimitiveBase & MyClass2DPrimitiveBase::operator = (const MyClass2DPrimitiveBase & _MyClass2DPrimitiveBaseObj)
	{
		if (type!=_MyClass2DPrimitiveBaseObj.GetType()) return *this;
		if (num_vertices!=_MyClass2DPrimitiveBaseObj.GetNumVertices()) return *this;
		//копирование информации о том кто владеет примитивом не нужно! :)
		//*((MyClassMessageSender *)this) =_MyClass2DPrimitiveBaseObj;

		for(unsigned int i=0; i<num_vertices; i++)
		{
			//копирование информации о вершинах
			(*arrVertices[i])=_MyClass2DPrimitiveBaseObj[i];
			//информация о владельце у вершины уже есть
		}

		oldValidInfo=_MyClass2DPrimitiveBaseObj.GetOldValidInfo();
		direction_CCW=_MyClass2DPrimitiveBaseObj.direction_CCW;
		normalsIsAlreadyCalculated=_MyClass2DPrimitiveBaseObj.NormalsIsCalculated();
		//changed();
		o_unClipingNow=false;
		return *this;
	}
	////оператор сравнения
	//bool MyClass2DPrimitiveBase::EquialToVerticesCoord(const MyClass2DPrimitiveBase & _MyClass2DPrimitiveBaseObj)
	//{
	//	bool result = (type==_MyClass2DPrimitiveBaseObj.GetType() && num_vertices==_MyClass2DPrimitiveBaseObj.GetNumVertices());
	//	if (result)
	//		for (unsigned int i=0; i<num_vertices; i++)
	//			if (!arrVertices[i]->EquialCoord(_MyClass2DPrimitiveBaseObj[i]))
	//			{
	//				result=false;
	//				break;
	//			}
	//	return result;
	//}

	MyEnumPrimitive2DType MyClass2DPrimitiveBase::GetType() const
	{
		return type;
	}

	unsigned int MyClass2DPrimitiveBase::GetNumVertices() const
	{
		return num_vertices;
	}
	MyClassVertex & MyClass2DPrimitiveBase::operator[](unsigned int _numVertex)
	{
		if (_numVertex>=num_vertices) _numVertex=(num_vertices-1);
		//changed();
		return *(arrVertices[_numVertex]);
	}
	const MyClassVertex & MyClass2DPrimitiveBase::operator[](unsigned int _numVertex) const
	{
		if (_numVertex>=num_vertices) _numVertex=(num_vertices-1);
		return *(arrVertices[_numVertex]);
	}

	void MyClass2DPrimitiveBase::CalcNormals()
	{
		if (normalsIsAlreadyCalculated) return;
		MyClassNormal normal;
		MyClassVector vA, vB;
		for(unsigned int j=0; j<num_vertices; j++)
		{
			vA.SetCoordA(arrVertices[j]->GetCoord());
			if (j==(num_vertices-1)) vA.SetCoordB(arrVertices[0]->GetCoord());
			else vA.SetCoordB(arrVertices[j+1]->GetCoord());
			vB.SetCoordA(arrVertices[j]->GetCoord());
			if (j==0) vB.SetCoordB(arrVertices[num_vertices-1]->GetCoord());
			else vB.SetCoordB(arrVertices[j-1]->GetCoord());
			//если направление против часовой стрелки
			normal=(direction_CCW ? vA.VectorMultiplication(vB) : vB.VectorMultiplication(vA));
			//if (direction_CCW) normal=vA.VectorMultiplication(vB);
			//else normal=vB.VectorMultiplication(vA);
			normal.Normalize();
			arrVertices[j]->SetNormal(normal);
		}
		normalsIsAlreadyCalculated=true;
	}

	bool MyClass2DPrimitiveBase::Validate()
	{
		if (oldValidInfo.isValidating) return oldValidInfo.oldValidResult;
		oldValidInfo.isValidating=true;
		for (unsigned int i=0; i<num_vertices; i++)
			for (unsigned int j=i+1; j<num_vertices; j++)
				if (arrVertices[i]->EquialCoord(*(arrVertices[j]))) 
				{
					oldValidInfo.oldValidResult=false;
					return false;
				}
		oldValidInfo.oldValidResult=true;
		return true;
	}

	const MyStrOldValidInfo & MyClass2DPrimitiveBase::GetOldValidInfo() const
	{
		return oldValidInfo;
	}

	MyClass2DPrimitiveBase::~MyClass2DPrimitiveBase(void)
	{
		o_cleaning=true;
		MessageSendToAll(MyClassMessage(die,0));
		for (unsigned int i=0; i<num_vertices; i++)
			if (o_pdieVertex!=arrVertices[i]) delete arrVertices[i];
		delete [] arrVertices;
		MessageSendToAll(MyClassMessage(dieCompl,0));
		o_cleaning=false;
	}
	//void MyClass2DPrimitiveBase::changed()
	//{
	//	this->MessageSendToAll(MyClassMessage(change2DPrimitive,0));
	//}
	void MyClass2DPrimitiveBase::MessageReseive(MyClassMessage _message, MyClassMessageSender * fromWho)
	{
		if (_message.message==unClipingNow)
		{
			o_unClipingNow=true;
		}
		if (_message.message==changeVertexCoord)
		{
			if (o_unClipingNow) {o_unClipingNow=false; return;}
			bool InMe=false;
			for (unsigned int i=0; i<num_vertices; i++)
				if (fromWho==arrVertices[i]) {InMe=true;break;}
			if (InMe)
			{
				oldValidInfo.isValidating=false;
				normalsIsAlreadyCalculated=false;
				//changed();
				this->MessageSendToAll(MyClassMessage(change2DPrimitive,fromWho));

			}
		}
		//if (_message==changeVertexTextureCoord)
		//{}
		if (_message.message==die)	//предсмертный вопль вершины
		{
			if (o_cleaning) return;
			
			//bool InMe=false;
			//for (unsigned int i=0; i<num_vertices; i++)
			//	if (fromWho==arrVertices[i]) {InMe=true;break;}
			//if (InMe) 
			//{
				o_pdieVertex=fromWho;
				delete this;
			/*}*/
		}
	}
	bool MyClass2DPrimitiveBase::NormalsIsCalculated()const
	{
		return normalsIsAlreadyCalculated;
	}


	MyClassVertex ** GetPointPointVertex(MyClass2DPrimitiveBase * _MyClass2DPrimitiveBaseObj, unsigned int _numVertex)
	{
		if (!_MyClass2DPrimitiveBaseObj) return 0;
		if (_numVertex>=_MyClass2DPrimitiveBaseObj->num_vertices) _numVertex=_MyClass2DPrimitiveBaseObj->num_vertices-1;
		return _MyClass2DPrimitiveBaseObj->arrVertices+_numVertex;
	}
}
