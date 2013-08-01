#pragma once
#include "msmodel.h"

#include<vector>

using namespace std;

enum behavior
{
	noState,
	stand,
	stand_2,
	stand_3,
	go,
	go_back,
	run,
	jump
};
struct behaviorInfo
{
	behavior state;
	float firstFrame;
	float lastFrame;
	behaviorInfo()
		:state(stand),firstFrame(-1),lastFrame(-1)
	{}
	behaviorInfo(behavior state, float firstFrame, float lastFrame)
	{
		this->state=state;
		this->firstFrame=firstFrame;
		this->lastFrame=lastFrame;
	}
};
class msModel_v2 :
	public msModel
{
private:
	bool model_loaded;
	bool TexturesToOGL_loaded;
	void loadTexturesToOGL(int anisotropyLevel);
	void unloadTexturesFromOGL();
	float scaleCoefficient;			//коэффициент масштабирования модели для рендеринга
	bool scaleCoefficientCalculated;
	float frameCalculatedScale;

	behaviorInfo *currentState;
	vector<behaviorInfo> BehaviorStates;
public:
	msModel_v2(void);
	void calcScaleCoefficient(float frame,float dx, float dy, float dz);
	float GetScaleCoefficient();
	bool ScaleCoefficientIsCalculated();
	bool Load(const char *filename,int anisotropyLevel=0);
	void Clear();

	bool AddBehaviorState(behaviorInfo behaviorState);
	bool ReplaceBehaviorState(behaviorInfo behaviorState);
	void SetCurrentBehaviorState(behavior currentState);
	float GetFirstBehaviorFrame();
	float GetLastBehaviorFrame();
	behavior GetCurrentBehaviorState();

	~msModel_v2(void);
	
};
void ClearBounds (float *mins, float *maxs);
void AddPointToBounds (const float *v, float *mins, float *maxs);