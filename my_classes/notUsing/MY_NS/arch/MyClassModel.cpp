#include "MyClassModel.h"
namespace MY_NS_MODEL
{
	MyClassModel::MyClassModel(void)
		:MyClassModelBase()
	{}
	MyClassModel::MyClassModel(const MyClassModel & _MyClassModelObj)
		:MyClassModelBase(_MyClassModelObj)
	{}
	void MyClassModel::UnRegAllMessReceiver()
	{
		this->MessageSendToAll(die);
		arrReceiver.clear();
	}
	void MyClassModel::UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj)
	{
		this->MessageSendTo(die,_MyClassMessageReceiverObj);
		MyTypeArrPointReseiver::iterator iter;
		MyTypeArrPointReseiver::iterator iterEnd=this->arrReceiver.end();
		for(iter=arrReceiver.begin(); iter!=iterEnd; iter++)
			if ((*iter)==_MyClassMessageReceiverObj)
			{
				arrReceiver.erase(iter);
				break;
			}
	}
}
