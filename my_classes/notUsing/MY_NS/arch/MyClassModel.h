#pragma once
#include "MyClassModelBase.h"
#include "MyClassMessageSenderSafe.h"


namespace MY_NS_MODEL
{
	class MyClassModel: public MyClassModelBase
	{
	public:
		MyClassModel(void);
		MyClassModel(const MyClassModel & _MyClassModelObj);
		//переопределение
		void UnRegAllMessReceiver();
		void UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj);
	};

}
