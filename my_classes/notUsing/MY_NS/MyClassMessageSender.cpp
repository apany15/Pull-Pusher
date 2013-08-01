#include "MyClassMessageSender.h"
#include"MyNSModel.h"
namespace MY_NS_MODEL
{
	MyClassMessageSender::MyClassMessageSender(void){}
	MyClassMessageSender::MyClassMessageSender(const MyClassMessageSender & _MyClassMessageSenderObj)
	{
		const MyTypeArrPointReseiver *tP=_MyClassMessageSenderObj.GetPointArrReceiver();
		unsigned int size=(unsigned int) tP->size();
		for(unsigned int i=0; i<size; i++)
			arrReceiver.push_back((*tP)[i]);
	}
	void MyClassMessageSender::RegMessReceiver(MyClassMessageReceiver * _MyClassMessageReceiverObj)
	{
		if (_MyClassMessageReceiverObj)	
		{
			bool present=false;
			unsigned int size=(unsigned int)arrReceiver.size();
			for(unsigned int i=0; i<size; i++)
				if (arrReceiver[i]==_MyClassMessageReceiverObj){present=true; break;}
			if (!present) arrReceiver.push_back(_MyClassMessageReceiverObj);
		}
	}
	//void MyClassMessageSender::UnRegMessReceiver(int pos)
	//{
	//	unsigned int tek_size=(unsigned int)arrReceiver.size();
	//	if (tek_size==0) return;
	//	if (pos>=(int)tek_size || pos==-1) pos=tek_size-1;
	//	MyTypeArrPointReseiver tempArr;
	//	for (unsigned int i=0; i<tek_size; i++)
	//		if (i!=pos) tempArr.push_back(arrReceiver[i]);
	//	tek_size--;
	//	arrReceiver.clear();
	//	for (unsigned int i=0; i<tek_size; i++)
	//		arrReceiver.push_back(tempArr[i]);
	//}

	void MyClassMessageSender::UnRegAllMessReceiver()
	{
		arrReceiver.clear();
	}
	void MyClassMessageSender::UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj)
	{
		
		MyTypeArrPointReseiver::iterator iter;
		MyTypeArrPointReseiver::iterator iterEnd=arrReceiver.end();
		for(iter=arrReceiver.begin(); iter!=iterEnd; iter++)
			if ((*iter)==_MyClassMessageReceiverObj)
			{
				arrReceiver.erase(iter);
				break;
			}
	}
	void MyClassMessageSender::MessageSendTo(MyClassMessage _message, const MyClassMessageReceiver * _MyClassMessageReceiverObj)
	{
		unsigned int size=(unsigned int)arrReceiver.size();
		for(unsigned int i=0; i<size; i++)
			if (arrReceiver[i]==_MyClassMessageReceiverObj) 
			{
				arrReceiver[i]->MessageReseive(_message, this);
				return;
			}

	}
	void MyClassMessageSender::MessageSendToAll(MyClassMessage _message)
	{
		MyTypeArrPointReseiver CopyArrReceiver=arrReceiver;
		unsigned int size=(unsigned int)CopyArrReceiver.size();
		//for(unsigned int i=0; i<size; i++)
		//	CopyArrReceiver.push_back(arrReceiver[i]);
		
		unsigned int i=0;
		for(unsigned int i=0; i<size; i++)
			CopyArrReceiver[i]->MessageReseive(_message, this);



		//unsigned int i=0;
		//while (i<size)
		//{
		//	//if (arrReceiver[i]) 
		//		arrReceiver[i]->MessageReseive(_message, this);
		//	size=(unsigned int)arrReceiver.size();
		//	i++;
		//}
		////for(unsigned int i=0; i<size; i++)
		////	if (arrReceiver[i]) 
		////		arrReceiver[i]->MessageReseive(_message, this);
	}
	void MyClassMessageSender::operator = (const MyClassMessageSender & _MyClassMessageSenderObj)
	{
		arrReceiver.clear();
		const MyTypeArrPointReseiver *tP=_MyClassMessageSenderObj.GetPointArrReceiver();
		unsigned int size=(unsigned int) tP->size();
		for(unsigned int i=0; i<size; i++)
			arrReceiver.push_back((*tP)[i]);
	}
	const MyTypeArrPointReseiver * MyClassMessageSender::GetPointArrReceiver()const
	{
		return &arrReceiver;
	}
}
