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
#ifndef _MSMODELRENDERER_H_
#define _MSMODELRENDERER_H_

class msModel;
class msModelRenderer
{
public:
	enum
	{
		eJointLines,
		eJointPoints
	};

public:
	msModelRenderer(/*msModel *model*/);
	virtual ~msModelRenderer();

public:
	void Render(msModel *m_model, bool withMaterial, bool flatShaded);
	void RenderJoints(msModel *m_model, int what);
	void BindMaterial(msModel *m_model, int materialIndex);

	//msModel /*const*/ * GetPointModel() const;

//public:
//	msModel *m_model;

private:
	msModelRenderer(const msModelRenderer& rhs);
	msModelRenderer& operator=(const msModelRenderer& rhs);
};

#endif // _MSMODELRENDERER_H_
