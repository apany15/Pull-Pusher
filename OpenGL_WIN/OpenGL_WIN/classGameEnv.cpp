#include "classGameEnv.h"

classGameEnv::classGameEnv(void)
{
	this->firstPersonage.pModelANDRenderer.pModel=&(this->firstPersonageModel);
	this->firstPersonage.pModelANDRenderer.pRenderer=&(this->modelRenderer);
}
void classGameEnv::clear()
{
	this->otherObjects.clear();

	this->firstPersonage.DestroyBody();
	this->firstPersonageModel.Clear();
	//на случай если модель главного персонажа привязана снаружи другая
	this->firstPersonage.pModelANDRenderer.pModel->Clear();
}

classGameEnv::~classGameEnv(void)
{
}
