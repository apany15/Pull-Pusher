#include "MyClassMessageSenderSafe.h"
namespace MY_NS_MODEL
{
	MyClassMessageSenderSafe::MyClassMessageSenderSafe(void)
		:MyClassMessageSender()
	{}
	MyClassMessageSenderSafe::MyClassMessageSenderSafe(const MyClassMessageSenderSafe & _MyClassMessageSenderObj)
		:MyClassMessageSender(_MyClassMessageSenderObj)
	{}

	void MyClassMessageSenderSafe::UnRegAllMessReceiver()
	{
		arrReceiver.clear();
		this->MessageSendToAll(die);
	}
	void MyClassMessageSenderSafe::UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj)
	{
		MyTypeArrPointReseiver::iterator iter;
		MyTypeArrPointReseiver::iterator iterEnd=arrReceiver.end();
		for(iter=arrReceiver.begin(); iter!=iterEnd; iter++)
			if ((*iter)==_MyClassMessageReceiverObj)
			{
				arrReceiver.erase(iter);
				break;
			}

		this->MessageSendTo(die,_MyClassMessageReceiverObj);
	}

	//MyClassMessageSenderSafe::~MyClassMessageSenderSafe(void)
	//{
	//}
}
