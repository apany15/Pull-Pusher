#pragma once
#include "MyClassMessageReceiver.h"
#include<vector>
#include"MyNSModel.h"
using namespace std;
namespace MY_NS_MODEL
{
	typedef vector<MyClassMessageReceiver *> MyTypeArrPointReseiver;
	class MyClassMessageSender
	{	
	protected:
		MyTypeArrPointReseiver arrReceiver;
	public:
		MyClassMessageSender(void);
		MyClassMessageSender(const MyClassMessageSender & _MyClassMessageSenderObj);
		void RegMessReceiver(MyClassMessageReceiver * _MyClassMessageReceiverObj);
		//void UnRegMessReceiver(int pos=-1);
		virtual void UnRegAllMessReceiver();
		virtual void UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj);
		void MessageSendTo(MyClassMessage _message, const MyClassMessageReceiver * _MyClassMessageReceiverObj);
		void MessageSendToAll(MyClassMessage _message);
		void operator = (const MyClassMessageSender & _MyClassMessageSenderObj);
		const MyTypeArrPointReseiver * GetPointArrReceiver()const;
		//~MyClassMessageSender(void);
	};
}
