#pragma once
#include "MyClassMessageSender.h"
namespace MY_NS_MODEL
{
	class MyClassMessageSenderSafe: public MyClassMessageSender
	{
	public:
		MyClassMessageSenderSafe(void);
		MyClassMessageSenderSafe(const MyClassMessageSenderSafe & _MyClassMessageSenderObj);
		void UnRegAllMessReceiver();
		void UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj);

		//~MyClassMessageSenderSafe(void);
	};
}
