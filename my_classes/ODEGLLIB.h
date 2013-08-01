#include <ode/ode.h>
//#include "windows.h"
#include "GL/glew.h"
#include "msModelRenderer.h"
#include "msModel_v2.h"

void ODE_TranslateRotate_2_OpenGL(GLfloat *local_matrix, dGeomID geom);

struct str_PModelANDRender
{
	msModelRenderer *pRenderer;
	msModel_v2 *pModel;
	str_PModelANDRender()
		: pRenderer(0), pModel(0)
	{}
	msModel_v2 * GetPModel() {return pModel;}
	msModelRenderer * GetPRenderer() {return pRenderer;}

};




class classGameEnv;
class classPModelANDRenderer
{
private:
	msModelRenderer *pRenderer;
	msModel_v2 *pModel;
public:
	classPModelANDRenderer()
		: pRenderer(0), pModel(0)
	{}
	msModel_v2 * GetPModel() {return pModel;}
	msModelRenderer * GetPRenderer() {return pRenderer;}

	/*void renderModel(bool withMaterial = true, bool flatShaded = false) {
		if (this->pRenderer && this->pModel) {
			this->pRenderer->Render(this->pModel, withMaterial, flatShaded);
		}
	}*/

	friend classGameEnv;
};
