#pragma once
#include"MyClassModel.h"
#include"MyClassMessageReceiver.h"

namespace MY_NS_MODEL
{
	class MyClassModelRenderer: public MyClassMessageReceiver
	{
	protected:
		MyClassModel * pModel;
	public:
		MyClassModelRenderer(MyClassModel * _pModel=0);
		void AttachModel(MyClassModel * _pModel);
		MyClassModel * GetPointAttachedModel();
		const MyClassModel * GetPointAttachedModel()const;
		void MessageReseive(MyClassMessage _message, MyClassMessageSender * fromWho);
		virtual void Render()=0;
		~MyClassModelRenderer(void);
	};
}
