#include "MyClass2DPrimitive.h"

namespace MY_NS_MODEL
{
	MyClass2DPrimitive::MyClass2DPrimitive(MyEnumPrimitive2DType _type)
		:MyClass2DPrimitiveBase(_type)
	{}
	MyClass2DPrimitive::MyClass2DPrimitive(unsigned int _numVertices)
		:MyClass2DPrimitiveBase(_numVertices)
	{}
	//����������� �����������
	MyClass2DPrimitive::MyClass2DPrimitive(const MyClass2DPrimitive & _MyClass2DPrimitiveObj)
		:MyClass2DPrimitiveBase(_MyClass2DPrimitiveObj)
	{}

	//��������
	void MyClass2DPrimitive::UnRegAllMessReceiver(){};
	void MyClass2DPrimitive::UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj){};

}
