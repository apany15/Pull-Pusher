#pragma once
#include"MyClassModelRenderer.h"

namespace MY_NS_MODEL
{
	class MyClassOGLModelRenderer: public MyClassModelRenderer
	{
	public:
		bool with_material;
		MyClassOGLModelRenderer(MyClassModel * _pModel=0);
		void Render();
		void RenderLastGroup();
		void Render_butLast();
	};
}
