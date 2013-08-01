#include "MyClassVertex.h"
#include "MyClassNormal.h"
namespace MY_NS_MODEL
{

	MyClassVertex::MyClassVertex(const float * _coord, const MyClassNormal *_MyClassNormalObj, const float * _texCoord)
		:MyClassVertexBase(_coord,_MyClassNormalObj,_texCoord)
	{}
	MyClassVertex::MyClassVertex(float _coordX, float _coordY, float _coordZ)
		:MyClassVertexBase(_coordX,_coordY,_coordZ)
	{}
	//конструктор копирования
	MyClassVertex::MyClassVertex(const MyClassVertex & _MyClassVertexObj)
		:MyClassVertexBase(_MyClassVertexObj)
	{}
	////заглушки
	//void MyClassVertex::UnRegAllMessReceiver(){};
	//void MyClassVertex::UnRegMessReceiver(const MyClassMessageReceiver * _MyClassMessageReceiverObj){};

}
