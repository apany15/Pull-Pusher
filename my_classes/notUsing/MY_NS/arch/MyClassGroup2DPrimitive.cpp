#include "MyClassGroup2DPrimitive.h"
namespace MY_NS_MODEL
{
	MyClassGroup2DPrimitive::MyClassGroup2DPrimitive(void)
		:MyClassGroup2DPrimitiveBase()
	{}
	MyClassGroup2DPrimitive::MyClassGroup2DPrimitive(const MyClassGroup2DPrimitive & _MyClassGroup2DPrimitiveObj)
		:MyClassGroup2DPrimitiveBase(_MyClassGroup2DPrimitiveObj)
	{}
	//заглушки
	void MyClassGroup2DPrimitive::UnRegAllMessReceiver(){};
	void MyClassGroup2DPrimitive::UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj){};
}
