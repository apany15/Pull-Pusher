#pragma once
#include"MyNSModel.h"
//#include "MyClassMessageSender.h"
namespace MY_NS_MODEL
{
	class MyClassMessageSender;
	class MyClassMessageReceiver
	{
	public:
		//MyClassMessageReceiver(void);
		virtual void MessageReseive(MyClassMessage _message, MyClassMessageSender * fromWho)=0;
		//~MyClassMessageReceiver(void);
	};
}
