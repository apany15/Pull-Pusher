#include "MyClassOGLModelRenderer.h"
#include "windows.h"
#include "GL/gl.h"
namespace MY_NS_MODEL
{
	MyClassOGLModelRenderer::MyClassOGLModelRenderer(MyClassModel * _pModel)
		:MyClassModelRenderer(_pModel)
	{
		with_material=true;
	}
	void MyClassOGLModelRenderer::Render()
	{
		if (!pModel) return;
		pModel->CalcNormals();
		
		unsigned int num_Group=pModel->GetNumGroup();
		for (unsigned int i=0; i<num_Group; i++)
			pModel->operator [](i)->CalcNormals();

		MyEnumPrimitive2DType currentType,nextType;
		bool first=true;
		bool groupClipped;
		for(unsigned int i=0; i<num_Group; i++)
		{
			MyClassGroup2DPrimitive * pGroup=(*pModel)[i];
			unsigned int num_primitives=pGroup->GetNum2DPrimitives();
			pGroup->Sort2DPrimitive();
			groupClipped=pGroup->IsClipped();
			pGroup->UnClipVerticesNeighboursGroup();

			//bind Material
			if (with_material)
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pGroup->material.color_AMBIENT);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pGroup->material.color_DIFFUSE);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pGroup->material.color_SPECULAR);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pGroup->material.shininess);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pGroup->material.color_EMISSION);
			}


			for(unsigned int j=0; j<num_primitives; j++)
			{
				const MyClass2DPrimitive * pPrim=(*((*pModel)[i]))[j];
				if (first)
				{
					first=false;
					currentType=pPrim->GetType();
					if (currentType==triangle) glBegin(GL_TRIANGLES);
					if (currentType==quad) glBegin(GL_QUADS);
					if (currentType==polygon) glBegin(GL_POLYGON);
				}
				nextType=pPrim->GetType();
				if (currentType!=nextType)
				{
					currentType=nextType;
					glEnd();
					if (currentType==triangle) glBegin(GL_TRIANGLES);
					if (currentType==quad) glBegin(GL_QUADS);
					if (currentType==polygon) glBegin(GL_POLYGON);
				}
				//отрисовка вершин примитива
				unsigned int numVertices=pPrim->GetNumVertices();
				for (unsigned int n=0; n<numVertices;n++)
				{
					const MyClassVertex * pVertex=&((*pPrim)[n]);
					const float * coord=pVertex->GetTexCoord();
					glTexCoord2f(coord[0], coord[1]);
					coord=pVertex->GetNormal()->GetCoord();
					glNormal3fv(coord);
					glVertex3fv(pVertex->GetCoord());
				}
				if (currentType==polygon) {glEnd(); first=true;}
			}
			if (!first) glEnd();
			first=true;
			if (groupClipped) pGroup->ClipVerticesNeighboursGroup();
		}
	}
	//void MyClassOGLModelRenderer::Render()
	//{
	//	if (!pModel) return;
	//	pModel->CalcNormals();
	//	
	//	unsigned int num_Group=pModel->GetNumGroup();
	//	for (unsigned int i=0; i<num_Group; i++)
	//		pModel->operator [](i)->CalcNormals();

	//	MyEnumPrimitive2DType currentType,nextType;
	//	bool first=true;
	//	bool groupClipped;
	//	for(unsigned int i=0; i<num_Group; i++)
	//	{
	//		MyClassGroup2DPrimitive * pGroup=(*pModel)[i];
	//		unsigned int num_primitives=pGroup->GetNum2DPrimitives();
	//		pGroup->Sort2DPrimitive();
	//		groupClipped=pGroup->IsClipped();
	//		pGroup->UnClipVerticesNeighboursGroup();



	//		for(unsigned int j=0; j<num_primitives; j++)
	//		{
	//			//bind Material
	//			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pGroup->material.color_AMBIENT);
	//			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pGroup->material.color_DIFFUSE);
	//			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pGroup->material.color_SPECULAR);
	//			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pGroup->material.shininess);
	//			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pGroup->material.color_EMISSION);

	//			const MyClass2DPrimitive * pPrim=(*((*pModel)[i]))[j];
	//			currentType=pPrim->GetType();
	//			if (currentType==triangle) glBegin(GL_TRIANGLES);
	//			if (currentType==quad) glBegin(GL_QUADS);
	//			if (currentType==polygon) glBegin(GL_POLYGON);
	//			//отрисовка вершин примитива
	//			for (unsigned int n=0; n<pPrim->GetNumVertices();n++)
	//			{
	//				const MyClassVertex * pVertex=&((*pPrim)[n]);
	//				const float * coord=pVertex->GetTexCoord();
	//				glTexCoord2f(coord[0], coord[1]);
	//				coord=pVertex->GetNormal()->GetCoord();
	//				glNormal3fv(coord);
	//				glVertex3fv(pVertex->GetCoord());
	//			}
	//			glEnd();
	//			float arr[]={1.0f,0.0f,0.0f,1.0f};
	//			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, arr);
	//			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, arr);
	//			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, arr);
	//			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);
	//			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, arr);
	//			glBegin(GL_LINES);
	//			for (unsigned int n=0; n<pPrim->GetNumVertices();n++)
	//			{
	//				const MyClassVertex * pVertex=&((*pPrim)[n]);
	//				const float * coord=pVertex->GetNormal()->GetCoord();
	//				float coord2[3];
	//				glVertex3fv(pVertex->GetCoord());
	//				coord2[0]=pVertex->GetCoord()[0]+coord[0];
	//				coord2[1]=pVertex->GetCoord()[1]+coord[1];
	//				coord2[2]=pVertex->GetCoord()[2]+coord[2];
	//				glVertex3fv(coord2);
	//			}
	//			glEnd();
	//			
	//		}
	//		if (groupClipped) pGroup->ClipVerticesNeighboursGroup();
	//	}
	//}

	void MyClassOGLModelRenderer::RenderLastGroup()
	{
		if (!pModel) return;
		unsigned int num_Group=pModel->GetNumGroup();
		if (num_Group==0) return;
		pModel->CalcNormals();
		
		
		pModel->operator [](num_Group-1)->CalcNormals();
		MyClassGroup2DPrimitive * pGroup=(*pModel)[num_Group-1];
		MyEnumPrimitive2DType currentType,nextType;
		bool first=true;

			unsigned int num_primitives=pGroup->GetNum2DPrimitives();
			pGroup->Sort2DPrimitive();
			bool groupClipped=pGroup->IsClipped();
			pGroup->UnClipVerticesNeighboursGroup();

			//bind Material
			if (with_material)
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pGroup->material.color_AMBIENT);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pGroup->material.color_DIFFUSE);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pGroup->material.color_SPECULAR);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pGroup->material.shininess);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pGroup->material.color_EMISSION);
			}


			for(unsigned int j=0; j<num_primitives; j++)
			{
				const MyClass2DPrimitive * pPrim=(*((*pModel)[num_Group-1]))[j];
				if (first)
				{
					first=false;
					currentType=pPrim->GetType();
					if (currentType==triangle) glBegin(GL_TRIANGLES);
					if (currentType==quad) glBegin(GL_QUADS);
					if (currentType==polygon) glBegin(GL_POLYGON);
				}
				nextType=pPrim->GetType();
				if (currentType!=nextType)
				{
					currentType=nextType;
					glEnd();
					if (currentType==triangle) glBegin(GL_TRIANGLES);
					if (currentType==quad) glBegin(GL_QUADS);
					if (currentType==polygon) glBegin(GL_POLYGON);
				}
				//отрисовка вершин примитива
				for (unsigned int n=0; n<pPrim->GetNumVertices();n++)
				{
					const MyClassVertex * pVertex=&((*pPrim)[n]);
					const float * coord=pVertex->GetTexCoord();
					glTexCoord2f(coord[0], coord[1]);
					coord=pVertex->GetNormal()->GetCoord();
					glNormal3fv(coord);
					glVertex3fv(pVertex->GetCoord());
				}
				if (currentType==polygon) {glEnd(); first=true;}
			}
			if (!first) glEnd();
			first=true;
			if (groupClipped) pGroup->ClipVerticesNeighboursGroup();
	}

	void MyClassOGLModelRenderer::Render_butLast()
	{
		if (!pModel) return;
		pModel->CalcNormals();
		
		unsigned int num_Group=pModel->GetNumGroup();
		for (unsigned int i=0; i<num_Group; i++)
			pModel->operator [](i)->CalcNormals();

		MyEnumPrimitive2DType currentType,nextType;
		bool first=true;
		bool groupClipped;
		for(unsigned int i=0; i<num_Group-1; i++)
		{
			MyClassGroup2DPrimitive * pGroup=(*pModel)[i];
			unsigned int num_primitives=pGroup->GetNum2DPrimitives();
			pGroup->Sort2DPrimitive();
			groupClipped=pGroup->IsClipped();
			pGroup->UnClipVerticesNeighboursGroup();

			//bind Material
			if (with_material)
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pGroup->material.color_AMBIENT);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pGroup->material.color_DIFFUSE);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pGroup->material.color_SPECULAR);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pGroup->material.shininess);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pGroup->material.color_EMISSION);
			}


			for(unsigned int j=0; j<num_primitives; j++)
			{
				const MyClass2DPrimitive * pPrim=(*((*pModel)[i]))[j];
				if (first)
				{
					first=false;
					currentType=pPrim->GetType();
					if (currentType==triangle) glBegin(GL_TRIANGLES);
					if (currentType==quad) glBegin(GL_QUADS);
					if (currentType==polygon) glBegin(GL_POLYGON);
				}
				nextType=pPrim->GetType();
				if (currentType!=nextType)
				{
					currentType=nextType;
					glEnd();
					if (currentType==triangle) glBegin(GL_TRIANGLES);
					if (currentType==quad) glBegin(GL_QUADS);
					if (currentType==polygon) glBegin(GL_POLYGON);
				}
				//отрисовка вершин примитива
				for (unsigned int n=0; n<pPrim->GetNumVertices();n++)
				{
					const MyClassVertex * pVertex=&((*pPrim)[n]);
					const float * coord=pVertex->GetTexCoord();
					glTexCoord2f(coord[0], coord[1]);
					coord=pVertex->GetNormal()->GetCoord();
					glNormal3fv(coord);
					glVertex3fv(pVertex->GetCoord());
				}
				if (currentType==polygon) {glEnd(); first=true;}
			}
			if (!first) glEnd();
			first=true;
			if (groupClipped) pGroup->ClipVerticesNeighboursGroup();
		}
	}


}