///////////////////////////////////////////////////////////////////////
// title:          MilkShape 3D Model Viewer Sample
//
// copyright:      The programs and associated files contained in this
//                 distribution were developed by Mete Ciragan.  The
//                 programs are not in the public domain, but they are
//                 freely distributable without licensing fees.  These
//                 programs are provided without guarantee or warrantee
//                 expressed or implied.  Use at your own risk!
//
// email:          mciragan@gmx.net
// web:            http://www.milkshape3d.com
///////////////////////////////////////////////////////////////////////
#include "msModelRenderer.h"
#include "msModel.h"
#include <GL/freeglut.h>

msModelRenderer::msModelRenderer(/*msModel *model*/)
//:	m_model(0)
{
}

msModelRenderer::~msModelRenderer()
{
}

void msModelRenderer::Render(msModel *m_model, bool withMaterial, bool flatShaded)
{
	if (!m_model) return;

	int numGroups = m_model->GetNumGroups();
	for (int i = 0; i < numGroups; i++)
	{
		ms3d_group_t *group = m_model->GetGroup(i);

		if (withMaterial)
			BindMaterial(m_model,group->materialIndex);
		else
			BindMaterial(m_model,-1);

		glBegin(GL_TRIANGLES);
		for (size_t j = 0; j < group->triangleIndices.size(); j++)
		{
			ms3d_triangle_t *triangle = m_model->GetTriangle(group->triangleIndices[j]);
			for (int v = 0; v < 3; v++)
			{
				ms3d_vertex_t *vertex = m_model->GetVertex(triangle->vertexIndices[v]);

				glTexCoord2f(triangle->s[v], triangle->t[v]);

				float normal[3];
				if (flatShaded)
					m_model->TransformNormal(vertex, triangle->normal, normal);
				else
					m_model->TransformNormal(vertex, triangle->vertexNormals[v], normal);
				glNormal3fv(normal);

				float pos[3];
				m_model->TransformVertex(vertex, pos);
				glVertex3fv(pos);
			}
		}
		glEnd();
	}
}

void msModelRenderer::RenderJoints(msModel *m_model,int what)
{
	if (!m_model) return;

	if (what == eJointLines)
	{
		glBegin(GL_LINES);
		for (int i = 0; i < m_model->GetNumJoints(); i++)
		{
			ms3d_joint_t *joint = m_model->GetJoint(i);
			if (joint->parentIndex == -1)
			{
				glVertex3f(joint->matGlobal[0][3], joint->matGlobal[1][3], joint->matGlobal[2][3]);
				glVertex3f(joint->matGlobal[0][3], joint->matGlobal[1][3], joint->matGlobal[2][3]);
			}
			else
			{
				ms3d_joint_t *parentJoint = m_model->GetJoint(joint->parentIndex);
				glVertex3f(joint->matGlobal[0][3], joint->matGlobal[1][3], joint->matGlobal[2][3]);
				glVertex3f(parentJoint->matGlobal[0][3], parentJoint->matGlobal[1][3], parentJoint->matGlobal[2][3]);
			}
		}
		glEnd();
	}
	else if (what == eJointPoints)
	{
		glBegin(GL_POINTS);
		for (int i = 0; i < m_model->GetNumJoints(); i++)
		{
			ms3d_joint_t *joint = m_model->GetJoint(i);
			glVertex3f(joint->matGlobal[0][3], joint->matGlobal[1][3], joint->matGlobal[2][3]);
		}
		glEnd();
	}
}

void msModelRenderer::BindMaterial(msModel *m_model, int materialIndex)
{
	if (!m_model) return;

	if (materialIndex < 0 || materialIndex >= m_model->GetNumMaterials())
	{
		glDepthMask(GL_TRUE);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glColor4f(1, 1, 1, 1);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);

		float ma[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
		float md[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
		float ms[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float me[4] = { 0.0f, 0.0f, 0.0f, 1.0f };


		//float ma[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		//float md[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
		//float ms[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		//float me[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float mss = 0.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ma);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, md);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ms);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, me);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mss);		
	}
	else
	{
		ms3d_material_t *material = m_model->GetMaterial(materialIndex);

		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->ambient);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->diffuse);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->specular);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material->emissive);
		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);		
		//glTexEnvf(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		

		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);

        if (material->transparency < 1.0f || material->mode & HASALPHA)
	    {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
           	glColor4f(1.0f, 1.0f, 1.0f, material->transparency);
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

			if (m_model->GetTransparencyMode() == TRANSPARENCY_MODE_SIMPLE)
			{
				glDepthMask(GL_FALSE);
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.0f);
			}
			else 
				if (m_model->GetTransparencyMode() == TRANSPARENCY_MODE_ALPHAREF)
				{
					glDepthMask(GL_TRUE);
					glEnable(GL_ALPHA_TEST);
					glAlphaFunc(GL_GREATER, m_model->GetAlphaRef());
				}
        }
        else
        {
           	glDisable(GL_BLEND);
           	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
        }
		
		if (material->mode & SPHEREMAP)
		{
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		}
		else
		{
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
		}
		glBindTexture(GL_TEXTURE_2D, material->id);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material->emissive);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);
	}
}

//msModel /*const*/ * msModelRenderer::GetPointModel() const
//{
//	return m_model;
//}
