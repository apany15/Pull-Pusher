#pragma once
#include"MyClassModelRenderer.h"

namespace MY_NS_MODEL
{
	class MyClassOGLModelRenderer: public MyClassModelRenderer
	{
	public:
		MyClassOGLModelRenderer(MyClassModel * _pModel=0);
		void Render();
	};
}
