#pragma once
#include"MyClass2DPrimitiveBase.h"
namespace MY_NS_MODEL
{

	class MyClass2DPrimitive: public MyClass2DPrimitiveBase
	{
	public:
		MyClass2DPrimitive(MyEnumPrimitive2DType _type=triangle);
		MyClass2DPrimitive(unsigned int _numVertices);
		//конструктор копирования
		MyClass2DPrimitive(const MyClass2DPrimitive & _MyClass2DPrimitiveObj);
	private:
		//заглушки
		void UnRegAllMessReceiver();
		void UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj);
	};
}
