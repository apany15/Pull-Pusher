#pragma once
namespace MY_NS_MODEL
{
	enum MyEnumMessageType
	{
		change2DPrimitive=0,
		//changeVertex=1,
		changeVertexCoord=2,
		//changeVertexTextureCoord=3,
		//changeVertexNormalCoord=4
		die=5,
		unClipingNow=6,
		changeGroup2DPrimitive=7,
		dieCompl=8


	};
	//enum coord_name
	//{
	//	X=0,
	//	Y=1,
	//	Z=2
	//};
	class MyClassMessage
	{
	public:
		MyEnumMessageType message;
		void * data;
		MyClassMessage(MyEnumMessageType _message, void * _data=0)
			:message(_message),
			data(_data)
		{}
	};
}