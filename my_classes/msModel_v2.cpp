#include"gl/glew.h"
#include "msModel_v2.h"

#include"il/ilut.h"
#include"il/il.h"
#include"il/ilu.h"

//#include"my_prgOGL.h"

//#include"glee.h"

msModel_v2::msModel_v2(void)
{
	model_loaded=false;
	TexturesToOGL_loaded=false;
	scaleCoefficient=1;
	scaleCoefficientCalculated=false;
	static bool devilInited=false;
	if (!devilInited)
	{
		ilInit();
		//iluInit();
		//ilutInit();

		//ilutRenderer(ILUT_OPENGL);
		devilInited=true;

	}

	currentState=0;
//	currentState.firstFrame=1;
//	currentState.lastFrame=1;
//	currentState.state=behavior::stand;

}
void msModel_v2::calcScaleCoefficient(float frame,float dx, float dy, float dz)
{
	if (!model_loaded) return;
	if (scaleCoefficientCalculated && frameCalculatedScale==frame) return;


	float currentFrame=this->GetCurrentFrame();
	this->SetFrame(frame);


	float mins[3], maxs[3];
	ClearBounds(mins, maxs);

	for (int i = 0; i < this->GetNumVertices(); i++)
	{
		ms3d_vertex_t *vertex = this->GetVertex(i);
		//AddPointToBounds(vertex->vertex, mins, maxs);
		float pos[3];
		this->TransformVertex(vertex, pos);
		AddPointToBounds(pos, mins, maxs);
		
	}
	float dim[3];
	dim[0] = maxs[0] - mins[0];
	dim[1] = maxs[1] - mins[1];
	dim[2] = maxs[2] - mins[2];

	//в фмзике ось Y и Z перевернуты
	float max_priv[3];
	max_priv[0]=dx;
	max_priv[1]=dz;
	max_priv[2]=dy;

	if (dim[0]>max_priv[0] || dim[1]>max_priv[1] || dim[2]>max_priv[2])
	{

		float t=max_priv[0]/dim[0];
		float scaleCoef=t;
		t=max_priv[1]/dim[1];
		if (t<scaleCoef)
			scaleCoef=t;
		t=max_priv[2]/dim[2];
		if (t<scaleCoef)
			scaleCoef=t;
		scaleCoefficient=scaleCoef;
	}

	float center[3];
	center[0] = mins[0] + dim[0] / 2.0f;
	center[1] = mins[1] + dim[1] / 2.0f;
	center[2] = mins[2] + dim[2] / 2.0f;

	scaleCoefficientCalculated=true;

	this->SetFrame(currentFrame);
	frameCalculatedScale=frame;
}
float msModel_v2::GetScaleCoefficient()
{
	return scaleCoefficient;
}
bool msModel_v2::ScaleCoefficientIsCalculated()
{
	return scaleCoefficientCalculated;
}

bool msModel_v2::Load(const char *filename, int anisotropyLevel)
{
	model_loaded=((msModel *)this)->Load(filename);
	this->SetupJoints();
	this->SetFrame(-1);
	this->loadTexturesToOGL(anisotropyLevel);
	return model_loaded;
}
//загрузка текстур
void msModel_v2::loadTexturesToOGL(int anisotropyLevel)
{
	if (!model_loaded) return;
	if (TexturesToOGL_loaded) return;

	
	
	int num_mat=this->GetNumMaterials();
	for(int i=0; i<num_mat; i++)
	{
		ms3d_material_t *mat=this->GetMaterial(i);

		if (mat->texture)
		{
			GLuint id;
		    ILuint texId;
			ilGenImages(1,&texId);
			ilBindImage(texId);
			if (ilLoadImage(mat->texture))
			{
				//iluMirror();
				//iluRotate(180);
				int  width  = ilGetInteger ( IL_IMAGE_WIDTH  );
				int  height = ilGetInteger ( IL_IMAGE_HEIGHT );
				int  type   = ilGetInteger ( IL_IMAGE_TYPE   );
				int  fmt    = ilGetInteger ( IL_IMAGE_FORMAT );
				glGenTextures ( 1, &id );
				glBindTexture ( GL_TEXTURE_2D, id );
				glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB,/* fmt,*/ width, height, 0, fmt, type, ilGetData () );
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT );
				if (anisotropyLevel>0)
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);

				glBindTexture( GL_TEXTURE_2D, 0 );
				ilDeleteImages ( 1, &texId );
			}
			else
				id=0;
			mat->id=id;
		}



//		if (mat->texture)
//			mat->id=(unsigned char) ilutGLLoadImage(mat->texture);
	}
	//glBindTexture(GL_TEXTURE_2D, 0);
	TexturesToOGL_loaded=true;
}
void msModel_v2::unloadTexturesFromOGL()
{
	if (!TexturesToOGL_loaded) return;

	int num_mat=this->GetNumMaterials();
	for(int i=0; i<num_mat; i++)
	{
		ms3d_material_t *mat=this->GetMaterial(i);
		if (mat->texture)
			glDeleteTextures(1,(const GLuint *) &(mat->id));
	}
	TexturesToOGL_loaded=false;


}
void msModel_v2::Clear()
{
	scaleCoefficientCalculated=false;
	scaleCoefficient=1;

	if (!model_loaded) 
	{
		((msModel *)this)->Clear();
		return;
	}
	model_loaded=false;

	unloadTexturesFromOGL();

	((msModel *)this)->Clear();
}


bool msModel_v2::AddBehaviorState(behaviorInfo behaviorState)
{
	vector<behaviorInfo>::iterator iter=this->BehaviorStates.begin();
	vector<behaviorInfo>::iterator iterEnd=this->BehaviorStates.end();
	bool find=false;
	
	while(iter!=iterEnd)
	{
		if (iter->state==behaviorState.state)
		{
			find=true;
			break;
		}
		iter++;
	}
	if (find) return false;
	this->BehaviorStates.push_back(behaviorInfo(behaviorState));
	return true;
}
bool msModel_v2::ReplaceBehaviorState(behaviorInfo behaviorState)
{
	vector<behaviorInfo>::iterator iter=this->BehaviorStates.begin();
	vector<behaviorInfo>::iterator iterEnd=this->BehaviorStates.end();
	bool find=false;
	
	while(iter!=iterEnd)
	{
		if (iter->state==behaviorState.state)
		{
			find=true;
			break;
		}
		iter++;
	}
	if (find) 
	{
		iter->firstFrame=behaviorState.firstFrame;
		iter->lastFrame=behaviorState.lastFrame;
	}
	return find;
}
void msModel_v2::SetCurrentBehaviorState(behavior currentState)
{
	vector<behaviorInfo>::iterator iter=this->BehaviorStates.begin();
	vector<behaviorInfo>::iterator iterEnd=this->BehaviorStates.end();
	bool find=false;
	while(iter!=iterEnd)
	{
		if (iter->state==currentState)
		{
			find=true;
			break;
		}
		iter++;
	}
	if (find)
	{
		if (!this->currentState)
			this->currentState=new behaviorInfo();
		this->currentState->firstFrame=iter->firstFrame;
		this->currentState->lastFrame=iter->lastFrame;
		this->currentState->state=currentState;
	}
}

float msModel_v2::GetFirstBehaviorFrame()
{
	if (this->currentState)
		return this->currentState->firstFrame;
	else
		return -1;
}
float msModel_v2::GetLastBehaviorFrame()
{

	if (this->currentState)
		return this->currentState->lastFrame;
	else
		return -1;
}

behavior msModel_v2::GetCurrentBehaviorState()
{
	if (this->currentState) return this->currentState->state;
	return behavior::noState;
}

msModel_v2::~msModel_v2(void)
{
	unloadTexturesFromOGL();
	if (this->currentState) delete currentState;

}







void ClearBounds (float *mins, float *maxs)
{
	mins[0] = mins[1] = mins[2] = 99999;
	maxs[0] = maxs[1] = maxs[2] = -99999;
}

void AddPointToBounds (const float *v, float *mins, float *maxs)
{
	int		i;
	float	val;

	for (i=0 ; i<3 ; i++)
	{
		val = v[i];
		if (val < mins[i])
			mins[i] = val;
		if (val > maxs[i])
			maxs[i] = val;
	}
}