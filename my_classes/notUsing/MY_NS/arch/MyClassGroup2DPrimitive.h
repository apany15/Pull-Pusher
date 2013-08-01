#pragma once
#include"MyClassGroup2DPrimitiveBase.h"
namespace MY_NS_MODEL
{
	class MyClassGroup2DPrimitive: public MyClassGroup2DPrimitiveBase
	{
	public:
		MyClassGroup2DPrimitive(void);
		MyClassGroup2DPrimitive(const MyClassGroup2DPrimitive & _MyClassGroup2DPrimitiveObj);

	private:
		//заглушки
		virtual void UnRegAllMessReceiver();
		virtual void UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj);
	};
}
