#pragma once
#include"classCharacterWithBodyODE.h"
#include<vector>

using namespace std;

class classGameEnv
{
private:
	msModelRenderer modelRenderer;
	msModel_v2 firstPersonageModel;
public:
	classCharacterWithBodyODE_safe firstPersonage;
	vector<classCharacterWithBodyODE_unsafe> otherObjects;

	void clear();
	
	classGameEnv(void);
	~classGameEnv(void);
};
