#pragma once
#include"MyClassVertexBase.h"
#include"MyClassNormal.h"
#include "MyClassMessageSender.h"
namespace MY_NS_MODEL
{
	class MyClassVertex: public MyClassVertexBase
	{
	public:
		
		MyClassVertex(const float * _coord=0, const MyClassNormal *_MyClassNormalObj=0, const float * _texCoord=0);
		MyClassVertex(float _coordX, float _coordY, float _coordZ);
		//конструктор копирования
		MyClassVertex(const MyClassVertex & _MyClassVertexObj);
		unsigned int numGroupInGroup;
	private:
		////заглушки
		//void UnRegAllMessReceiver();
		//void UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj);

	};
}
