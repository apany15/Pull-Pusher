#include"my_prgOGL.h"
#include"classCharacterWithBodyODE.h"
#include "classLight.h"
#include "msModel_v2.h"
#include "msModelRenderer.h"
#include "classTimeControl.h"
#include <ode/ode.h>

#include<stdlib.h>	//для рандома
#include<time.h>	//для рандома

#include<vector>

#include "classGameEnv.h"


#define MAX_CONTACTS 8


using namespace std;

////массив физических объектов (включая персонаж)
//vector<classCharacterWithBodyODE*> arrayPhObject;


/*#include "classGameObjectODEMS.h"

classGameObjectODEMS a;*/


classTimeControl timer;

classLight *light=NULL;

//игровое окружение
classGameEnv gameEnv;
bool pressed_global_watch=false;



//рендерер моделей
msModelRenderer *model_render=0;
////модель персонажа
//msModel_v2 *myModel_pers=0;
classTimeControl timerAnimation;
classTimeControl timerAnimationStand;
bool CurrentModelBehStand=false;		//в настоящее время персонаж стоит
float CurrentFrame;
double maxTimeStand0=10;
double timeStand=0;
//модель сцены
msModel_v2 *myModel_testPol=0;





dWorldID world;
dSpaceID space;
dJointGroupID contactgroup;


dGeomID idPlanePol;



void ODE_collide(void *data, dGeomID o1, dGeomID o2)
{
    dBodyID b1=dGeomGetBody(o1);
    dBodyID b2=dGeomGetBody(o2);

	if (b1 && b2 && dAreConnectedExcluding(b1,b2,dJointTypeContact)) return;

	dContact contact[MAX_CONTACTS];
	dContact contactT;
	
	//ищем кто с кем взаимодействет и выставляем параметры взаимодействия
	bool find=(gameEnv.firstPersonage.SetCollideOptions(contactT,o1,o2)==1);
	for(int j=0; j<gameEnv.otherObjects.size() && !find; j++)
		find=(gameEnv.otherObjects[j].SetCollideOptions(contactT,o1,o2)==1);
	//если данных о взаимодействии нет
	if (!find)
	{
		//есди нет данных о колизиях этих объектов - выставление по умолчанию
		contactT.surface.mode = dContactSoftCFM | dContactSoftERP;
		float h=1;			//шаг
		float kp=0.001;		//коэф упругости
		float kd=2;			//коэф торможения
		contactT.surface.soft_cfm=(h*kp)/(h*kp+kd);
		contactT.surface.soft_erp=1/(h*kp+kd);
		contactT.surface.mu=dInfinity;
	}



	//int num_obj=arrayPhObject.size();
	//for(int j=0; j<num_obj; j++)
	//{
	//	//каждый объект проверяет является ли он объектом коллизии, если да - проверяет в своей базе партнера по коллизии, и если такой найден - выставляет параметры коллизии
	//	if (!arrayPhObject[j]->SetCollideOptions(contactT,o1,o2))
	//	{
	//		//есди нет данных о колизиях этих объектов - выставление по умолчанию
	//		contactT.surface.mode = dContactSoftCFM | dContactSoftERP;
	//		float h=1;			//шаг
	//		float kp=0.001;		//коэф упругости
	//		float kd=2;			//коэф торможения
	//		contactT.surface.soft_cfm=(h*kp)/(h*kp+kd);
	//		contactT.surface.soft_erp=1/(h*kp+kd);
	//		contactT.surface.mu=dInfinity;
	//	}
	//	else break;//find=true;
	//}
	for (int i=0; i<MAX_CONTACTS; i++) 
	{
		contact[i]=contactT;
	}

    if (int numc = dCollide (o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact))) 
	{
		dGeomID characGeom=gameEnv.firstPersonage.GetGeomID();
		//если один из объектов - главный персонаж
		if (o1==characGeom || o2==characGeom) 
		{
			dGeomID oporaTID=(o1==characGeom ? o2 : o1);
			bool opora_p=(oporaTID==idPlanePol);
			for(int i=0; i<gameEnv.otherObjects.size() && !opora_p; i++)
				opora_p=(gameEnv.otherObjects[i].GetGeomID()==oporaTID);
			if (opora_p)
			{
				gameEnv.firstPersonage.oporaGeomID=(oporaTID==idPlanePol ? 0 : oporaTID);
				gameEnv.firstPersonage.opora=true;
			}
		}
		
		//dGeomID characGeom=arrayPhObject[0]->GetGeomID();
		//if (o1==characGeom || o2==characGeom) 
		//{
		//	dGeomID oporaTID=(o1==characGeom ? o2 : o1);
		//	bool opora_p=(oporaTID==idPlanePol);
		//	for(int i=1; i<arrayPhObject.size() && !opora_p; i++)
		//		opora_p=(arrayPhObject[i]->GetGeomID()==oporaTID);
		//	if (opora_p)
		//	{
		//		arrayPhObject[0]->oporaGeomID=(oporaTID==idPlanePol ? 0 : oporaTID);
		//		arrayPhObject[0]->opora=true;
		//	}
		//}

		for(int i=0; i<numc; i++)
		{
			dJointID c=dJointCreateContact(world,contactgroup,contact+i);
			dJointAttach(c,b1,b2);
		}
    }

}


void ODE_init()
{

	dInitODE();
    world=dWorldCreate();
    space=dHashSpaceCreate(0);
    dWorldSetGravity(world,0,0,0);
    dWorldSetCFM(world,1e-5);

	//Устанавливает и получает глубину слоя поверхности вокруг геометрических 
	//объектов. Контакты могyт погружаться на глубину слоя поверхности прежде чем остановиться.
	//может помочь решить проблему с дрожанием
	dWorldSetContactSurfaceLayer(world,0.001);

	//dWorldSetContactMaxCorrectingVel (world, 1000);

	
	dGeomID geomBox=dCreateBox(space,1,1,1);
	dGeomSetPosition(geomBox,3.0f,0.6f,0.0f);
	dGeomSetBody(geomBox,0);
	


	contactgroup = dJointGroupCreate(0);

	//dCreatePlane(space,0,0,1,-5);
	//dCreatePlane(space,0,0,-1,-5);
	//
	idPlanePol=dCreatePlane(space,0,1,0,0);
	//dCreatePlane(space,0,-1,0,-5);
	//
	//dCreatePlane(space,-1,0,0,-5);
	//dCreatePlane(space,1,0,0,-5);
	

	//инит физики игрового окружения
	//инит физики основного персонажа
	gameEnv.firstPersonage.SetTotalMass(70);
	gameEnv.firstPersonage.SetBackTypeFig(CYLINDER);
	gameEnv.firstPersonage.SetGeomSize(0.4,0.4,0.8);
	gameEnv.firstPersonage.CreateBody(space,world,0,2,-4);
	gameEnv.firstPersonage.MaxForceToMove=5500;
	gameEnv.firstPersonage.vel=4;
	gameEnv.firstPersonage.AddCollidePartner(idPlanePol,5,0.000001,10,800,true);
	gameEnv.firstPersonage.AddCollidePartner(geomBox,1,0.001,3,0,true);

	

	

	dBodySetGravityMode (gameEnv.firstPersonage.GetBodyID(), 2);


	//инит физики дополнительного персонажа(объекта)
	gameEnv.otherObjects[0].SetTotalMass(80);
	gameEnv.otherObjects[0].SetGeomSize(0.4,0.4,0.8);
	gameEnv.otherObjects[0].SetBackTypeFig(PARALLELEPIPED);
	gameEnv.otherObjects[0].SetRotLim(false);
	gameEnv.otherObjects[0].CreateBody(space,world,1,1,0);


	dJointID motor=dJointCreateAMotor(world,0);
	dJointAttach(motor,gameEnv.otherObjects[0].GetBodyID(),0);
	dJointSetAMotorMode(motor,dAMotorEuler);
	dJointSetAMotorAxis(motor,0,0,0,0,-1);
	dJointSetAMotorAxis(motor,2,0,1,0,0);
	dJointSetAMotorParam(motor,dParamHiStop,0);
	dJointSetAMotorParam(motor,dParamLoStop,-0);
	dJointSetAMotorParam(motor,dParamHiStop2,pi/64);
	dJointSetAMotorParam(motor,dParamLoStop2,-pi/64);
	dJointSetAMotorParam(motor,dParamHiStop3,pi/64);
	dJointSetAMotorParam(motor,dParamLoStop3,-pi/64);


	//инит физики дополнительного персонажа(объекта)
	gameEnv.otherObjects[1].SetTotalMass(80);
	gameEnv.otherObjects[1].SetGeomSize(0.4,0.4,0.8);
	gameEnv.otherObjects[1].SetBackTypeFig(PARALLELEPIPED);
	gameEnv.otherObjects[1].SetRotLim(false);
	gameEnv.otherObjects[1].CreateBody(space,world,2,1,0);

	dJointID motor2=dJointCreateAMotor(world,0);
	dJointAttach(motor2,gameEnv.otherObjects[1].GetBodyID(),0);
	dJointSetAMotorMode(motor2,dAMotorEuler);
	dJointSetAMotorAxis(motor2,0,0,0,0,-1);
	dJointSetAMotorAxis(motor2,2,0,1,0,0);
	dJointSetAMotorParam(motor2,dParamHiStop,0);
	dJointSetAMotorParam(motor2,dParamLoStop,-0);
	dJointSetAMotorParam(motor2,dParamHiStop2,pi/64);
	dJointSetAMotorParam(motor2,dParamLoStop2,-pi/64);
	dJointSetAMotorParam(motor2,dParamHiStop3,pi/64);
	dJointSetAMotorParam(motor2,dParamLoStop3,-pi/64);





	////инит физики дополнительного персонажа(объекта)
	//gameEnv.otherObjects[2].SetTotalMass(70);
	//gameEnv.otherObjects[2].SetGeomSize(0.4,0.4,0.8);
	//gameEnv.otherObjects[2].SetBackTypeFig(PARALLELEPIPED);
	//gameEnv.otherObjects[2].SetRotLim(false);
	//gameEnv.otherObjects[2].CreateBody(space,world,1,3,0);


	////инит физики дополнительного персонажа(объекта)
	//gameEnv.otherObjects[3].SetTotalMass(70);
	//gameEnv.otherObjects[3].SetGeomSize(0.4,0.4,0.8);
	//gameEnv.otherObjects[3].SetBackTypeFig(PARALLELEPIPED);
	//gameEnv.otherObjects[3].SetRotLim(false);
	//gameEnv.otherObjects[3].CreateBody(space,world,1,4,0);


	






	//взаимодействие остальных объектов между собой с полом и другими объектами
	for(int i=0; i<gameEnv.otherObjects.size(); i++)
	{
		gameEnv.otherObjects[i].AddCollidePartner(idPlanePol,5,0.000001,10,1000,true);
		gameEnv.otherObjects[i].AddCollidePartner(geomBox,1,0.001,3,0,true);
		gameEnv.firstPersonage.AddCollidePartner(gameEnv.otherObjects[i].GetGeomID(),5,0.000001,10,100,false);
		//взаимодействие остальных объектов между собой
		for(int j=0; j<i; j++)
			gameEnv.otherObjects[i].AddCollidePartner(gameEnv.otherObjects[j].GetGeomID(),5,0.000001,10,100,false);
	}


	//arrayPhObject.push_back(new classCharacterWithBodyODE());
	//arrayPhObject[1]->SetTotalMass(70);
	//arrayPhObject[1]->SetGeomSize(0.4,0.4,0.8);
	////arrayPhObject[1]->SetBackTypeFig(type);
	//arrayPhObject[1]->SetBackTypeFig(PARALLELEPIPED);
	//arrayPhObject[1]->SetRotLim(false);
	//arrayPhObject[1]->CreateBody(space,world,1,1,0);
	//arrayPhObject[0]->AddCollidePartner(arrayPhObject[1]->GetGeomID(),1,0.001,3,100,false);
	//arrayPhObject[1]->AddCollidePartner(idPlanePol,1,0.0001,3,50,false);
	//arrayPhObject[1]->AddCollidePartner(geomBox,1,0.001,3,0,true);
	//arrayPhObject[1]->pModelANDRenderer.pModel=myModel_pers;
	//arrayPhObject[1]->pModelANDRenderer.pRenderer=model_render;

	//for(int i=0; i<10; i++)
	//	arrayPhObject[0]->AddCollidePartner(shapes[i].GetGeomID(),character2.defaultCollidePartner.step,character2.defaultCollidePartner.k_elasticity,character2.defaultCollidePartner.k_braking,character2.defaultCollidePartner.mu,true);
	//character2.AddCollidePartner(ODE_objects[1].geomID,1,1,1,dInfinity,false);


	//srand( (unsigned)time( NULL ) );

	//for (int i=1; i<=10; i++)
	//{
	//	dReal r=((dReal)(rand()%10+1))/10;
	//	dReal ry=((dReal)(rand()%10+1))/10;
	//	dReal rz=((dReal)(rand()%5+1))/10;

	//	dReal x=((dReal)(rand()%20+1-10));
	//	dReal y=((dReal)(rand()%10+1))/5;
	//	dReal z=((dReal)(rand()%20+1-10));

	//	int type=rand()%3;

	//	arrayPhObject.push_back(new classCharacterWithBodyODE());
	//	arrayPhObject[i]->SetTotalMass(100*r);
	//	arrayPhObject[i]->SetGeomSize(r,ry,rz);
	//	arrayPhObject[i]->SetBackTypeFig(type);
	//	//arrayPhObject[i]->SetBackTypeFig(PARALLELEPIPED);
	//	arrayPhObject[i]->SetRotLim(false);
	//	arrayPhObject[i]->CreateBody(space,world,x,y,z);

	//	arrayPhObject[0]->AddCollidePartner(arrayPhObject[i]->GetGeomID(),1,0.001,3,100,false);
	//	arrayPhObject[i]->AddCollidePartner(idPlanePol,1,0.0001,3,50,false);
	//}
	
	//MY_NS_V2::classMS3DModel m,m2;
	//m.LoadFromFile("MyFirstModel.ms3d");
	////MY_NS_V2::classModelRenderer rawmr;
	//MY_NS_V2::classMS3DModelOGLRenderer mr;
	//mr.ConnectModel(&m);
	//mr.Render(0,0);
	//m2=m;


	//MY_NS_V2::classMS3DModel kkk;
	//kkk.LoadFromFile("MyFirstModel.ms3d");
	//const std::vector<MY_NS_V2::classMS3DModel::ms3d_vertex_t> *p=kkk.GetPoint_m_vertices();
	//char t=(*p)[0].boneId;
	
	//bool l=kkk.IsModelLoaded();
	//MY_NS_MESSAGE::classMessageSender<int> s;
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//glEnable(GL_CULL_FACE);
	
	

}
void ModelLightInit()
{

	model_render=new msModelRenderer();

	//myModel_pers=new msModel_v2();
	//myModel_pers->Load("dwarf2.ms3d",TEXTURES_ANISOTROPY_LEVEL);
	//myModel_pers->AddBehaviorState(behaviorInfo(behavior::stand,1,1));
	//myModel_pers->AddBehaviorState(behaviorInfo(behavior::stand_2,292,325));
	//myModel_pers->AddBehaviorState(behaviorInfo(behavior::stand_3,327,360));
	//myModel_pers->AddBehaviorState(behaviorInfo(behavior::go,16,26));
	//myModel_pers->AddBehaviorState(behaviorInfo(behavior::go_back,16,26));
	//myModel_pers->SetCurrentBehaviorState(behavior::stand);


	
	//заполнение игрового окружения - модели и анимация
	//основной персонаж
	gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->Load("dwarf2.ms3d",TEXTURES_ANISOTROPY_LEVEL);
	gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->AddBehaviorState(behaviorInfo(behavior::stand,1,1));
	gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->AddBehaviorState(behaviorInfo(behavior::stand_2,292,325));
	gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->AddBehaviorState(behaviorInfo(behavior::stand_3,327,360));
	gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->AddBehaviorState(behaviorInfo(behavior::go,16,26));
	gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->AddBehaviorState(behaviorInfo(behavior::go_back,16,26));
	gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->SetCurrentBehaviorState(behavior::stand);
	//дополнительный объект
	gameEnv.otherObjects.push_back(classCharacterWithBodyODE_unsafe());
	gameEnv.otherObjects[0].pModelANDRenderer.pModel=new msModel_v2();
	gameEnv.otherObjects[0].pModelANDRenderer.pModel->Load("box.ms3d",TEXTURES_ANISOTROPY_LEVEL);
	gameEnv.otherObjects[0].pModelANDRenderer.pRenderer=gameEnv.firstPersonage.pModelANDRenderer.GetPRenderer();

	//еще объект
	gameEnv.otherObjects.push_back(classCharacterWithBodyODE_unsafe());
	gameEnv.otherObjects[1].pModelANDRenderer.pModel=new msModel_v2();
	gameEnv.otherObjects[1].pModelANDRenderer.pModel->Load("box.ms3d",TEXTURES_ANISOTROPY_LEVEL);
	gameEnv.otherObjects[1].pModelANDRenderer.pRenderer=gameEnv.firstPersonage.pModelANDRenderer.GetPRenderer();

	////еще объект
	//gameEnv.otherObjects.push_back(classCharacterWithBodyODE_unsafe());
	////3D модель использувать основного персонажа
	//gameEnv.otherObjects[2].pModelANDRenderer.pModel=gameEnv.firstPersonage.pModelANDRenderer.GetPModel();
	//gameEnv.otherObjects[2].pModelANDRenderer.pRenderer=gameEnv.firstPersonage.pModelANDRenderer.GetPRenderer();

	////еще объект
	//gameEnv.otherObjects.push_back(classCharacterWithBodyODE_unsafe());
	////3D модель использувать основного персонажа
	//gameEnv.otherObjects[3].pModelANDRenderer.pModel=gameEnv.firstPersonage.pModelANDRenderer.GetPModel();
	//gameEnv.otherObjects[3].pModelANDRenderer.pRenderer=gameEnv.firstPersonage.pModelANDRenderer.GetPRenderer();




	myModel_testPol=new msModel_v2();
	myModel_testPol->Load("s0.ms3d",TEXTURES_ANISOTROPY_LEVEL);




	
	light=new classLight();
	light->ambientColor[0]=0.4;
	light->ambientColor[1]=0.4;
	light->ambientColor[2]=0.4;
	light->ambientColor[3]=1.0f;
	light->glLightModelLocalViewer=(GLenum)GL_TRUE;
	light->setShadeModel((GLenum)GL_SMOOTH);

	light->addLamp((GLenum)GL_LIGHT0);
	(*light)[(GLenum)GL_LIGHT0]->setPosX(0);
	(*light)[(GLenum)GL_LIGHT0]->setPosY(10);
	(*light)[(GLenum)GL_LIGHT0]->setPosZ(0);
	(*light)[(GLenum)GL_LIGHT0]->setDirX(0);
	(*light)[(GLenum)GL_LIGHT0]->setDirY(-1);
	(*light)[(GLenum)GL_LIGHT0]->setDirZ(0);
	(*light)[(GLenum)GL_LIGHT0]->setSpotCutOff(20);
	(*light)[(GLenum)GL_LIGHT0]->setSpotExponent(60);
	(*light)[(GLenum)GL_LIGHT0]->setMePositional();

	(*light)[(GLenum)GL_LIGHT0]->ambientColor[0]=0.7f;
	(*light)[(GLenum)GL_LIGHT0]->ambientColor[1]=0.7f;
	(*light)[(GLenum)GL_LIGHT0]->ambientColor[2]=0.7f;
	(*light)[(GLenum)GL_LIGHT0]->ambientColor[3]=1;
	(*light)[(GLenum)GL_LIGHT0]->diffuseColor[0]=0.7f;
	(*light)[(GLenum)GL_LIGHT0]->diffuseColor[1]=0.7f;
	(*light)[(GLenum)GL_LIGHT0]->diffuseColor[2]=0.7f;
	(*light)[(GLenum)GL_LIGHT0]->diffuseColor[3]=1;
	(*light)[(GLenum)GL_LIGHT0]->specularColor[0]=0.7f;
	(*light)[(GLenum)GL_LIGHT0]->specularColor[1]=0.7f;
	(*light)[(GLenum)GL_LIGHT0]->specularColor[2]=0.7f;
	(*light)[(GLenum)GL_LIGHT0]->specularColor[3]=1;

	light->addLamp((GLenum)GL_LIGHT1);
	(*light)[(GLenum)GL_LIGHT1]->setPosX(0);
	(*light)[(GLenum)GL_LIGHT1]->setPosY(20);
	(*light)[(GLenum)GL_LIGHT1]->setPosZ(0);
	(*light)[(GLenum)GL_LIGHT1]->setDirX(0);
	(*light)[(GLenum)GL_LIGHT1]->setDirY(-1);
	(*light)[(GLenum)GL_LIGHT1]->setDirZ(0);
	(*light)[(GLenum)GL_LIGHT1]->setSpotCutOff(20);
	(*light)[(GLenum)GL_LIGHT1]->setSpotExponent(60);
	(*light)[(GLenum)GL_LIGHT1]->setMeDirectional();

	(*light)[(GLenum)GL_LIGHT1]->ambientColor[0]=0.3f;
	(*light)[(GLenum)GL_LIGHT1]->ambientColor[1]=0.3f;
	(*light)[(GLenum)GL_LIGHT1]->ambientColor[2]=0.3f;
	(*light)[(GLenum)GL_LIGHT1]->ambientColor[3]=1;
	(*light)[(GLenum)GL_LIGHT1]->diffuseColor[0]=0.3f;
	(*light)[(GLenum)GL_LIGHT1]->diffuseColor[1]=0.3f;
	(*light)[(GLenum)GL_LIGHT1]->diffuseColor[2]=0.3f;
	(*light)[(GLenum)GL_LIGHT1]->diffuseColor[3]=1;
	(*light)[(GLenum)GL_LIGHT1]->specularColor[0]=0.3f;
	(*light)[(GLenum)GL_LIGHT1]->specularColor[1]=0.3f;
	(*light)[(GLenum)GL_LIGHT1]->specularColor[2]=0.3f;
	(*light)[(GLenum)GL_LIGHT1]->specularColor[3]=1;

}

void my_drawResizeWindowOGL(GLsizei width, GLsizei height)
{
	if (height==0)              // Предотвращение деления на ноль
	{
		height=1;
	}
	glViewport(0,0,width,height);          // Сброс текущей области вывода 
	glMatrixMode(GL_PROJECTION);            // Выбор матрицы проекций
	glLoadIdentity();              // Сброс матрицы проекции
	// Вычисление соотношения геометрических размеров для окна
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);            // Выбор матрицы вида модели
	glLoadIdentity();              // Сброс матрицы вида модели
}

void my_keyboardTest(const bool *keyStatus)
{
	static bool pressed=false;
	if (keyStatus['C'])
	{
		
		if (!pressed)
		{
			pressed=true;
			pressed_global_watch=!pressed_global_watch;
		}
	}
	else
	{
		pressed=false;
	}
	

	bool run=false;
	if (keyStatus['W'] && gameEnv.firstPersonage.opora) 
	{

		behavior tB=gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetCurrentBehaviorState();
		if (tB!=behavior::go)
		{
			gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->SetCurrentBehaviorState(behavior::go);
			CurrentFrame=gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetFirstBehaviorFrame();
		}
		run=true;	

		

		//вектор скорости в мировых координатах
		const dReal *linearVel=dBodyGetLinearVel(gameEnv.firstPersonage.GetBodyID());
		dVector3 velInBody;
		//преобразовуем вектор скорости в координаты объекта
		dBodyVectorFromWorld(gameEnv.firstPersonage.GetBodyID(),linearVel[0],linearVel[1],linearVel[2],velInBody);
		dReal vel=velInBody[1];
		dReal nadaForce=(gameEnv.firstPersonage.vel-vel)*gameEnv.firstPersonage.MaxForceToMove/gameEnv.firstPersonage.vel;
		
		gameEnv.firstPersonage.AddForceForward(nadaForce);

		if (gameEnv.firstPersonage.oporaGeomID)
		{
			//сила к опоре
			dVector3 oporaForceVector;
			dBodyVectorToWorld(gameEnv.firstPersonage.GetBodyID(),0,-nadaForce,0,oporaForceVector);
			dBodyAddForce(dGeomGetBody(gameEnv.firstPersonage.oporaGeomID),oporaForceVector[0],oporaForceVector[1],oporaForceVector[2]);
		}
	}
	if (keyStatus['S'] && gameEnv.firstPersonage.opora) 
	{

		behavior tB=gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetCurrentBehaviorState();
		if (tB!=behavior::go_back)
		{
			gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->SetCurrentBehaviorState(behavior::go_back);
			CurrentFrame=gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetLastBehaviorFrame();
		}
		run=true;	



		const dReal *linearVel=dBodyGetLinearVel(gameEnv.firstPersonage.GetBodyID());
		dVector3 velInBody;
		dBodyVectorFromWorld(gameEnv.firstPersonage.GetBodyID(),linearVel[0],linearVel[1],linearVel[2],velInBody);
		dReal vel=-velInBody[1];
		dReal nadaForce=(gameEnv.firstPersonage.vel-vel)*gameEnv.firstPersonage.MaxForceToMove/gameEnv.firstPersonage.vel;
		gameEnv.firstPersonage.AddForceBack(nadaForce);

		if (gameEnv.firstPersonage.oporaGeomID)
		{
			//сила к опоре
			dVector3 oporaForceVector;
			dBodyVectorToWorld(gameEnv.firstPersonage.GetBodyID(),0,nadaForce,0,oporaForceVector);
			dBodyAddForce(dGeomGetBody(gameEnv.firstPersonage.oporaGeomID),oporaForceVector[0],oporaForceVector[1],oporaForceVector[2]);
		}
	}

	if (keyStatus['A'] && gameEnv.firstPersonage.opora) 
	{
		const dReal *linearVel=dBodyGetLinearVel(gameEnv.firstPersonage.GetBodyID());
		dVector3 velInBody;
		dBodyVectorFromWorld(gameEnv.firstPersonage.GetBodyID(),linearVel[0],linearVel[1],linearVel[2],velInBody);
		dReal vel=velInBody[0];
		dReal nadaForce=(gameEnv.firstPersonage.vel-vel)*gameEnv.firstPersonage.MaxForceToMove/gameEnv.firstPersonage.vel;
		gameEnv.firstPersonage.AddForceLeft(nadaForce);

		if (gameEnv.firstPersonage.oporaGeomID)
		{
			//сила к опоре
			dVector3 oporaForceVector;
			dBodyVectorToWorld(gameEnv.firstPersonage.GetBodyID(),-nadaForce,0,0,oporaForceVector);
			dBodyAddForce(dGeomGetBody(gameEnv.firstPersonage.oporaGeomID),oporaForceVector[0],oporaForceVector[1],oporaForceVector[2]);
		}

	}

	if (keyStatus['D'] && gameEnv.firstPersonage.opora) 
	{
		const dReal *linearVel=dBodyGetLinearVel(gameEnv.firstPersonage.GetBodyID());
		dVector3 velInBody;
		dBodyVectorFromWorld(gameEnv.firstPersonage.GetBodyID(),linearVel[0],linearVel[1],linearVel[2],velInBody);
		dReal vel=-velInBody[0];
		dReal nadaForce=(gameEnv.firstPersonage.vel-vel)*gameEnv.firstPersonage.MaxForceToMove/gameEnv.firstPersonage.vel;
		gameEnv.firstPersonage.AddForceRight(nadaForce);

		if (gameEnv.firstPersonage.oporaGeomID)
		{
			//сила к опоре
			dVector3 oporaForceVector;
			dBodyVectorToWorld(gameEnv.firstPersonage.GetBodyID(),nadaForce,0,0,oporaForceVector);
			dBodyAddForce(dGeomGetBody(gameEnv.firstPersonage.oporaGeomID),oporaForceVector[0],oporaForceVector[1],oporaForceVector[2]);
		}

	}
	
	static bool isJumping=false;
	static classTimeControl timerJumping(true);

	if (keyStatus[' ']) 
	{
		if (!gameEnv.firstPersonage.opora) return;
		if (!isJumping) 
		{
			gameEnv.firstPersonage.AddForceUp(2*gameEnv.firstPersonage.MaxForceToMove);
			isJumping=true;
			timerJumping.SetDifferenceTimeZero();
		}
	}
	if (isJumping && timerJumping.GetDifferenceTime()>0.5) isJumping=false;


	if (keyStatus['I']) dWorldSetGravity(world,0,0,-9.8);
	if (keyStatus['K']) dWorldSetGravity(world,0,0,9.8);
	if (keyStatus['J']) dWorldSetGravity(world,-9.8,0,0);
	if (keyStatus['L']) dWorldSetGravity(world,9.8,0,0);

	if (keyStatus['Y']) dWorldSetGravity(world,0,9.8,0);
	if (keyStatus['H']) dWorldSetGravity(world,0,-9.8,0);
	if (keyStatus['U']) dWorldSetGravity(world,0,0,0);


	if (!run)
	{
		behavior curBeh=gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetCurrentBehaviorState();
		if (curBeh!=behavior::stand && curBeh!=behavior::stand_2 && curBeh!=behavior::stand_3)
		{
			gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->SetCurrentBehaviorState(behavior::stand);
			CurrentFrame=gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetFirstBehaviorFrame();
			maxTimeStand0=10;
			timeStand=0;
		}
	}
}

extern bool  fullscreen;
void my_mouseTest(int x, int y, bool lButton, bool rButton, bool mButton)
{

	static bool setcur = false;
	static int oldx, oldy;
	static bool fstart = true;
	if (fstart)
	{
		fstart = false;
		//character.transferASightOnAMousePush(0,0,x,y);
		gameEnv.firstPersonage.transferASightOnAMousePush(x, y);
	}
	if (setcur) 
	{
		//character.transferASightOnAMousePush(0,0,oldx,oldy);
		gameEnv.firstPersonage.transferASightOnAMousePush(oldx, oldy);
		setcur = false;
		return;
	}
	//character.mouseMove(x,y,MYPRGOGLMAINWINDOWWIDTH,MYPRGOGLMAINWINDOWHEIGHT,&oldx,&oldy);
	gameEnv.firstPersonage.mouseMove(x, y, MYPRGOGLMAINWINDOWWIDTH, MYPRGOGLMAINWINDOWHEIGHT, &oldx, &oldy);
	//if (fullscreen)
	ShowCursor(false);
	//{
		SetCursorPos(oldx, oldy);
		setcur = true;
	//}
}

void my_initOGL(void)
{
	glewInit();
	//GLenum initGlew=glewInit();
	//if (initGlew!=GLEW_OK)
	//{
	//	bool l;
	//	l=false;
	//}
	glEnable(GL_MULTISAMPLE_ARB);
	//if (GLEW_ARB_vertex_program)
	//{
	//	bool l;
	//	l=false;
	//}
	//1.3 - 2.1
	//bool l=glewIsSupported("GL_EXT_texture_filter_anisotropic");
	//if (l)
	//{
	//	int g_nMaxAnisotropy;
	//	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&g_nMaxAnisotropy);
	//	bool l;
	//	l=false;
	//}
	//bool l=glewIsSupported("GL_ARB_multisample");

	


	glClearColor(0.3f,0.0f,0.3f,0.0f);          // Очистка экрана в черный цвет
	glClearDepth(1.0f);              // Разрешить очистку буфера глубины
	glEnable(GL_DEPTH_TEST);            // Разрешить тест глубины
	glDepthFunc(GL_LEQUAL);            // Тип теста глубины
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);      // Улучшение в вычислении перспективы

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	//полигоны отрисовуются сплошными с обеих сторон
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	

	//glEnable(GL_FOG);
	{
		GLfloat fogColor[4]={0.3, 0.3, 0.3, 1.0};
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogi(GL_FOG_MODE, GL_EXP);
		glFogf(GL_FOG_DENSITY, 0.03);
		glHint(GL_FOG_HINT, GL_NICEST);
		glFogf(GL_FOG_START, 1.0);
		glFogf(GL_FOG_END, 30.0);
	}

	ModelLightInit();
	light->init();
	light->on();
	(*light)[(GLenum)GL_LIGHT0]->init();
	(*light)[(GLenum)GL_LIGHT0]->turnOn();
	(*light)[(GLenum)GL_LIGHT1]->init();
	(*light)[(GLenum)GL_LIGHT1]->turnOn();

	//(*light)[(GLenum)GL_LIGHT2]->init();
	//(*light)[(GLenum)GL_LIGHT2]->turnOn();
	
	ODE_init();

}



void my_drawOGL(void)
{

	//const dReal *tM=dBodyGetTorque (gameEnv.otherObjects[0].GetBodyID());
	//
	//dReal tM2[3];
	//tM2[0]=tM[0];
	//tM2[1]=tM[1];
	//tM2[2]=tM[2];
	//tM2[0]=0;
	//tM2[1]=0;
	//
	//dBodySetTorque (gameEnv.otherObjects[0].GetBodyID(), tM2[0], tM2[1], tM2[2]);

	gameEnv.firstPersonage.opora=false;
	//arrayPhObject[0]->oporaGeomID=0;

	dSpaceCollide(space, 0, &ODE_collide);


	//dWorldQuickStep(world, timer.GetChangeSafeDifferenceTime(0.01));
	dWorldStep(world, timer.GetChangeSafeDifferenceTime(0.01));

	dJointGroupEmpty(contactgroup);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);      // Очистить экран и буфер глубины
	glLoadIdentity();              // Сбросить текущую матрицу

	gameEnv.firstPersonage.openEyesOGL(pressed_global_watch);

	//gameEnv.otherObjects[0].openEyesOGL(pressed_global_watch);

	(*light).putAll();
	

	if (gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetCurrentBehaviorState()==behavior::stand)
	{
		timeStand+=timerAnimationStand.GetChangeDifferenceTime();
		if (timeStand>maxTimeStand0)
		{
			maxTimeStand0=rand()%5+5;
			timeStand=0;
				int c=(rand()%100);
				if (c>48)
					gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->SetCurrentBehaviorState(behavior::stand_2);
				else
					gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->SetCurrentBehaviorState(behavior::stand_3);
				CurrentModelBehStand=true;
				CurrentFrame=gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetFirstBehaviorFrame();
				timerAnimation.GetChangeDifferenceTime();
		}
	}


	float delta=timerAnimation.GetChangeDifferenceTime()*gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetAnimationFps();
	if (gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetCurrentBehaviorState()==behavior::go_back)
	{
		CurrentFrame-=delta/2;
		if (CurrentFrame<(gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetFirstBehaviorFrame()))
			CurrentFrame=gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetLastBehaviorFrame();
	}
	else
	{
		CurrentFrame+=delta/2;
		if (CurrentFrame>(gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetLastBehaviorFrame()))
		{
			if (CurrentModelBehStand)
			{
				gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->SetCurrentBehaviorState(behavior::stand);
				CurrentModelBehStand=false;
			}
			CurrentFrame=gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->GetFirstBehaviorFrame();
		}

	}

	gameEnv.firstPersonage.pModelANDRenderer.GetPModel()->SetFrame(CurrentFrame);


	//gameEnv.otherObjects[0].pModelANDRenderer.pModel->SetFrame(CurrentFrame);

	//myModel->SetFrame(-1);


	//GLfloat color[4];
	//GLfloat shininess;
	//vector<classCharacterWithBodyODE*>::iterator iterEnd;



	//**************************************************************************************
	//****************************рисование объектов под полом******************************
	//glScalef(1.0f,-1.0f,1.0f);
	//glFrontFace(GL_CW);
	//(*light)[(GLenum)GL_LIGHT0]->setPosY(-(*light)[(GLenum)GL_LIGHT0]->getPosY());
	//light->putAll();

	//color[0]=0.65f;
	//color[1]=0.4f;
	//color[2]=0.96f;
	//color[3]=1.0f;
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	//color[0]=1;
	//color[1]=1;
	//color[2]=1;
	//color[3]=1;
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	//shininess=2;
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	//iterEnd=arrayPhObject.end();
	//for(vector<classCharacterWithBodyODE*>::iterator iter=arrayPhObject.begin(); iter!=iterEnd ; iter++)
	//	(*iter)->DrawBackFig();

	//glScalef(1.0f,-1.0f,1.0f);
	//glFrontFace(GL_CCW);
	//(*light)[(GLenum)GL_LIGHT0]->setPosY(-(*light)[(GLenum)GL_LIGHT0]->getPosY());
	//light->putAll();

	//**************************************************************************************
	//**************************************************************************************
	//**************************************************************************************

	//рисование пола
	//glPushMatrix();
	//glScalef(0.02,0.02,0.02);
	//renderPol.Render(true,false);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glPopMatrix();
	

	//**************************************************************************************
	//*********************************рисование теней**************************************
	//MyClassGroup2DPrimitive * pointGroupPol=m2->operator [](3);

	//if (pointGroupPol)
	//{
	//glDisable(GL_DEPTH_TEST);
	//M3DVector4f vPlaneEquation;
	//M3DVector3f point1,	point2, point3;
	//MyClass2DPrimitive * pointPrimitive1=pointGroupPol->operator [](1);
	//point1[0]=pointPrimitive1->operator [](0).GetCoord()[0];
	//point1[1]=pointPrimitive1->operator [](0).GetCoord()[1];
	//point1[2]=pointPrimitive1->operator [](0).GetCoord()[2];
	//point2[0]=pointPrimitive1->operator [](1).GetCoord()[0];
	//point2[1]=pointPrimitive1->operator [](1).GetCoord()[1];
	//point2[2]=pointPrimitive1->operator [](1).GetCoord()[2];
	//point3[0]=pointPrimitive1->operator [](2).GetCoord()[0];
	//point3[1]=pointPrimitive1->operator [](2).GetCoord()[1];
	//point3[2]=pointPrimitive1->operator [](2).GetCoord()[2];
	//m3dGetPlaneEquation(vPlaneEquation, point1, point2, point3);

	//
	//color[0]=0.0f;
	//color[1]=0.0f;
	//color[2]=0.0f;
	//shininess=128;
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	//GLfloat L_x=light->operator []((GLenum)GL_LIGHT0)->getPosX();
	//GLfloat L_y=light->operator []((GLenum)GL_LIGHT0)->getPosY();
	//GLfloat L_z=light->operator []((GLenum)GL_LIGHT0)->getPosZ();
	////коеф. регулирования прозрачности тени
	//GLfloat d1=0.1;
	//GLfloat c1=0.1;
	//GLfloat d2=100;
	//GLfloat c2=0.0001;
	//GLfloat k=(c1-c2)/(d1-d2);
	//GLfloat a=c2-k*d2;


	//GLfloat lightPos[4];
	//lightPos[1]=L_y;
	//lightPos[3]=1.0f;
	//const dReal *bp;
	//M3DMatrix44f shadowMat;
	//iterEnd=arrayPhObject.end();
	//for(vector<classCharacterWithBodyODE*>::iterator iter=arrayPhObject.begin(); iter!=iterEnd ; iter++)
	//{
	//	bp=dBodyGetPosition((*iter)->GetBodyID());
	//	lightPos[0]=-bp[0]+L_x;
	//	lightPos[2]=-bp[2]+L_z;
	//	GLfloat d=sqrt((L_x-bp[0])*(L_x-bp[0])+(L_y-bp[1])*(L_y-bp[1])+(L_z-bp[2])*(L_z-bp[2]));

	//	//регулирование прозрачности тени
	//	color[3]=k*d+a;
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);


	//	m3dMakePlanarShadowMatrix(shadowMat, vPlaneEquation, lightPos); 
	//	glPushMatrix();
	//	glMultMatrixf((GLfloat *)shadowMat);
	//	(*iter)->DrawBackFig();
	//	glPopMatrix();
	//}
	//glEnable(GL_DEPTH_TEST);
	//}
	//**************************************************************************************
	//**************************************************************************************
	//**************************************************************************************


	glPushMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//glTranslatef(0,arrayPhObject[0]->GetGeomSizeZ()/2,0);
	glTranslatef(3,0.6,0);
	auxSolidBox(1,1,1);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glPopMatrix();


	

	/*glPushMatrix();
	msModel_v2 * pModel=gameEnv.firstPersonage.pModelANDRenderer.GetPModel();
	pModel->calcScaleCoefficient(1,gameEnv.firstPersonage.GetGeomSizeX(),gameEnv.firstPersonage.GetGeomSizeY(), gameEnv.firstPersonage.GetGeomSizeZ());
	float scaleCoef=pModel->GetScaleCoefficient();		
	glScalef(scaleCoef,scaleCoef,scaleCoef);
	model_render->Render(pModel,true,false);
	glPopMatrix();*/



	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	//**************************************************************************************
	//****************************рисование объектов над полом*******************************
	//float me[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, me);
	//color[0]=0.55f;
	//color[1]=0.4f;
	//color[2]=0.96f;
	//color[3]=1.0f;
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	//color[0]=1;
	//color[1]=1;
	//color[2]=1;
	//color[3]=1;
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	//shininess=100;
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	
	model_render->Render(myModel_testPol,true,true);

	glTranslatef(0,0.01,0);
	vector<classCharacterWithBodyODE_unsafe>::iterator iterEnd=gameEnv.otherObjects.end();
	vector<classCharacterWithBodyODE_unsafe>::iterator iter=gameEnv.otherObjects.begin();
	for(; iter!=iterEnd ; iter++) {
		(*iter).renderModel();
		(*iter).DrawBackFig();
	}
		

	
	gameEnv.firstPersonage.renderModel();
	gameEnv.firstPersonage.DrawBackFig();


	
	//**************************************************************************************
	//**************************************************************************************
	//**************************************************************************************

	//rend.RenderLastGroup();


}
void my_beforeExit()
{
	if (model_render)
		delete model_render;
	if (light)
		delete light;
	if (myModel_testPol)
		delete myModel_testPol;



 //   dJointGroupDestroy(contactgroup);

	//dWorldDestroy(world);
 //   dSpaceDestroy(space);
 //   dCloseODE();
}
