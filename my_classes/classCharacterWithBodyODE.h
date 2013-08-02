#pragma once

//#include "msModelRenderer.h"

#include <ode/ode.h>
#include<vector>

#define CHARDRAWOGL

#ifdef CHARDRAWOGL
	//#include "windows.h"
	//#include "GL/gl.h"
	//#include "GL/glu.h"
	#include <gl\glew.h>
	#include <GL/glaux.h>
	//#include<glut.h>

#endif
#include "ODEGLLIB.h"



#define PARALLELEPIPED 0
#define CYLINDER 1
#define SHAPE 2

//характеристики взаимодействия с телом
struct str_partner_def
{
	dReal step;				//шаг "пружины"
	dReal k_elasticity;		//коеффициент "упругости"
	dReal k_braking;		//коеффициент "торможения"
	dReal mu;				//сила трения с телом
};
struct str_partner
{
	dGeomID partner_geomID;	//id геометрии тела
	dReal step;				//шаг "пружины"
	dReal k_elasticity;		//коеффициент "упругости"
	dReal k_braking;		//коеффициент "торможения"
	dReal mu;				//сила трения с телом
	bool setFdir1;
};
typedef std::vector<str_partner> typeArrayPartners;

#define classCharacterWithBodyODE_unsafe classCharacterWithBodyODE<str_PModelANDRender>
#define classCharacterWithBodyODE_safe classCharacterWithBodyODE<classPModelANDRenderer>


template <class _PModelAndPRenderer>
class classCharacterWithBodyODE
{
protected:
	float sizeX;
	float sizeY;
	float sizeZ;
	dBodyID bodyID;
	dGeomID geomID;
	bool bodyIsCreated;
	
	dReal total_mass;
	
	float ay;	//угол поворота вдоль оси y
	int zahvatX;
	int zahvatY;



	dJointID motorX;
	typeArrayPartners collidePartners;
	
	typeArrayPartners::iterator GetPointInfoCollidePartners(dGeomID partner_geomID);

	int BackTypeFig;
	
	bool rotLim;	//ограничение поворациваемости при колизиях (необходимо для персонажа от первого лица)

public:
	_PModelAndPRenderer pModelANDRenderer;

	void (*DrawBackFigPoint)(classCharacterWithBodyODE *pCharacter);	//указатель на функцию для самостоятельной прорисовки фигуры-клетки
	bool opora;
	dGeomID oporaGeomID;
	

	dReal MaxForceToMove;
	float vel;
	classCharacterWithBodyODE();
	~classCharacterWithBodyODE();
	bool CreateBody(dSpaceID space,  dWorldID world, float xx, float yy, float zz);
	void DestroyBody();

	bool SetTotalMass(float totalMass);

	dReal GetTotalMass();
	dBodyID GetBodyID();
	dGeomID GetGeomID();
	bool SetGeomSize(float dx, float dy, float dz);
	float GetGeomSizeX();
	float GetGeomSizeY();
	float GetGeomSizeZ();
	bool BodyIsCreated();
	bool AddForceForward(dReal force);
	bool AddForceBack(dReal force);
	bool AddForceLeft(dReal force);
	bool AddForceRight(dReal force);
	bool AddForceUp(dReal force);


#ifdef CHARDRAWOGL
	bool openEyesOGL(bool global=false);
	void renderModel(bool withMaterial = true, bool flatShaded = false);
#endif

	void transferASightOnAMousePush(int x, int y);
	bool mouseMove(int x, int y, int windowWidht, int windowHeight, int *oldx, int *oldy);

	str_partner_def defaultCollidePartner;
	//-1 - меня тут вообще нет, 0 - я есть, но данным нет, 1 - установлено 
	int SetCollideOptions(dContact &contact, dGeomID geom1, dGeomID geom2);
	void AddCollidePartner(dGeomID partner_geomID, dReal step, dReal k_elasticity, dReal k_braking, dReal mu, bool setFdir1);
	bool RemoveCollidePartner(dGeomID partner_geomID);
	bool SetBackTypeFig(int type);
	int GetBackTypeFig();
	bool DrawBackFig(bool externalFunc=false);

	bool SetRotLim(bool b);
	bool GetRotLim();
};




//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************


#define pi 3.1415926535897932384626433832795

#include "msModel_v2.h"

template <class _PModelAndPRenderer>
classCharacterWithBodyODE<_PModelAndPRenderer>::classCharacterWithBodyODE()
{
	
	sizeX=0;
	sizeY=0;
	sizeZ=0;

	total_mass=70;
	MaxForceToMove=3000;
	bodyIsCreated=false;

	defaultCollidePartner.step=1;				//шаг "пружины"
	defaultCollidePartner.k_elasticity=0.001;	//коеффициент "упругости"
	defaultCollidePartner.k_braking=2;			//коеффициент "торможения"
	defaultCollidePartner.mu=dInfinity;			//сила трения с телом
	BackTypeFig=PARALLELEPIPED;
	DrawBackFigPoint=0;
	rotLim=true;

	//pModelRenderer=0;
	//pModel=0;
}
template <class _PModelAndPRenderer>
classCharacterWithBodyODE<_PModelAndPRenderer>::~classCharacterWithBodyODE()
{
	DestroyBody();
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::CreateBody(dSpaceID space,  dWorldID world, float xx, float yy, float zz)
{
	if (bodyIsCreated) return false;
	if (sizeX==0 || sizeY==0 || sizeZ==0) return false;

	
	dMass m;
	switch (BackTypeFig)
	{
	case CYLINDER:
		{
			geomID=dCreateCylinder(space,sizeX/2,sizeZ);
			dMassSetCylinderTotal(&m,total_mass,2,sizeX/2,sizeZ);
			break;
		}
	case SHAPE:
		{
			geomID=dCreateSphere(space,sizeX/2);
			dMassSetSphereTotal(&m,total_mass,sizeX/2);
			break;
		}
	default:
		{
			geomID=dCreateBox(space,sizeX,sizeY,sizeZ);
			dMassSetBoxTotal(&m,total_mass,sizeX,sizeY,sizeZ);
			break;
		}
	}

	bodyID=dBodyCreate(world);
	dBodySetMass(bodyID,&m);
	dGeomSetBody(geomID,bodyID);

	dMatrix3 matr;
	dRFromAxisAndAngle(matr,1,0,0,pi/2);
	dGeomSetRotation(geomID,matr);


	if (rotLim)	//если есть ограничение на поворачиваемость (необходимо для персонажа от первого лица)
	{
		motorX=dJointCreateAMotor(world,0);
		dJointAttach(motorX,bodyID,0);
		dJointSetAMotorMode(motorX,dAMotorEuler);

		dJointSetAMotorAxis(motorX,0,0,0,0,-1);
		//dJointSetAMotorAxis(motorX,1,0,0,0,0);
		dJointSetAMotorAxis(motorX,2,0,1,0,0);

		dJointSetAMotorParam(motorX,dParamHiStop,0);
		dJointSetAMotorParam(motorX,dParamHiStop3,0);
		dJointSetAMotorParam(motorX,dParamHiStop2,0);
	}


	dBodySetPosition(bodyID,xx,yy,zz);

	bodyIsCreated=true;
	return true;
}
template <class _PModelAndPRenderer>
void classCharacterWithBodyODE<_PModelAndPRenderer>::DestroyBody()
{
	if (bodyIsCreated)
	{
		dBodyDestroy(bodyID);
		dGeomDestroy(geomID);
		if (rotLim) dJointDestroy(motorX);
		collidePartners.clear();
		bodyIsCreated=false;
	}
}

template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::SetTotalMass(float totalMass)
{
	if (!bodyIsCreated && totalMass>0)
	{
		total_mass=totalMass;
		return true;
	}
	return false;
}


template <class _PModelAndPRenderer>
dReal classCharacterWithBodyODE<_PModelAndPRenderer>::GetTotalMass()
{
	return total_mass;
}
template <class _PModelAndPRenderer>
dBodyID classCharacterWithBodyODE<_PModelAndPRenderer>::GetBodyID()
{
	return bodyID;
}
template <class _PModelAndPRenderer>
dGeomID classCharacterWithBodyODE<_PModelAndPRenderer>::GetGeomID()
{
	return geomID;
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::SetGeomSize(float dx, float dy, float dz)
{
	if (!bodyIsCreated)
	{
		if (dx<=0) dx=0.3;
		if (dy<=0) dy=0.3;
		if (dz<=0) dz=0.3;
		sizeX=dx;
		sizeY=dy;
		sizeZ=dz;
		return true;
	}
	return false;
}
template <class _PModelAndPRenderer>
float classCharacterWithBodyODE<_PModelAndPRenderer>::GetGeomSizeX()
{
	return sizeX;
}
template <class _PModelAndPRenderer>
float classCharacterWithBodyODE<_PModelAndPRenderer>::GetGeomSizeY()
{
	return sizeY;
}
template <class _PModelAndPRenderer>
float classCharacterWithBodyODE<_PModelAndPRenderer>::GetGeomSizeZ()
{
	return sizeZ;
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::BodyIsCreated()
{
	return bodyIsCreated;
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::AddForceForward(dReal force)
{
	if (bodyIsCreated)
	{
		//const dReal *orientBody=dBodyGetRotation(bodyID);
		dBodyAddRelForce(bodyID,0,force,0);
		return true;
	}
	return false;
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::AddForceBack(dReal force)
{
	if (bodyIsCreated)
	{
		dBodyAddRelForce(bodyID,0,-force,0);
		return true;
	}
	return false;
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::AddForceLeft(dReal force)
{
	if (bodyIsCreated)
	{
		dBodyAddRelForce(bodyID,force,0,0);
		return true;
	}
	return false;
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::AddForceRight(dReal force)
{
	if (bodyIsCreated)
	{
		dBodyAddRelForce(bodyID,-force,0,0);
		return true;
	}
	return false;
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::AddForceUp(dReal force)
{
	if (bodyIsCreated)
	{
		dBodyAddRelForce(bodyID,0,0,-force);
		return true;
	}
	return false;
}
#ifdef CHARDRAWOGL
	template <class _PModelAndPRenderer>
	bool classCharacterWithBodyODE<_PModelAndPRenderer>::openEyesOGL(bool global)
	{
		dReal posCam[3];
		dReal posLook[3];
		dReal posUP[3];
		const dReal *posBody;
		const dReal *orientBody;
		if (rotLim && bodyIsCreated)
		{
			posBody = dBodyGetPosition(bodyID);
			orientBody = dBodyGetRotation(bodyID);
		}
		if (global || !(rotLim && bodyIsCreated)) 
		{
			if (rotLim && bodyIsCreated)
			{
				posCam[0] = posBody[0];
				posCam[1] = 10;
				posCam[2] = posBody[2];

				posLook[0] = posBody[0];
				posLook[1] = posBody[1];
				posLook[2] = posBody[2];
				posUP[0] = orientBody[1];
				posUP[1] = orientBody[5];
				posUP[2] = orientBody[9];
			}
			else
			{
				posCam[0] = 0;
				posCam[1] = 10;
				posCam[2] = 0;
				posLook[0] = 0;
				posLook[1] = 0;
				posLook[2] = 0;
				posUP[0] = 0;
				posUP[1] = 0;
				posUP[2] = -1;
			}
		}
		else
		{
			if (!rotLim || !bodyIsCreated) return false;
			posCam[0] = posBody[0] - orientBody[1] * 2;
			posCam[1] = posBody[1] - orientBody[5] * 2 + (1 - sin((*this).ay));
			posCam[2] = posBody[2] - orientBody[9] * 2;
			posLook[0] = posBody[0];
			posLook[1] = posBody[1];
			posLook[2] = posBody[2];
			posUP[0] = -orientBody[2];
			posUP[1] = -orientBody[6];
			posUP[2] = -orientBody[10];
		}


		gluLookAt(posCam[0], posCam[1], posCam[2], posLook[0], posLook[1], posLook[2], posUP[0], posUP[1], posUP[2]);
		return true;
	}
template <class _PModelAndPRenderer>
void classCharacterWithBodyODE<_PModelAndPRenderer>::renderModel(bool withMaterial = true, bool flatShaded = false)
{
	GLfloat local_matrix[16];
	ODE_TranslateRotate_2_OpenGL(local_matrix, this->geomID);
	glPushMatrix();
	glMultMatrixf(local_matrix);
	glRotatef(180, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	msModel_v2 * pModel = this->pModelANDRenderer.GetPModel();
	glTranslatef(0, -this->sizeZ/2, 0);
	pModel->calcScaleCoefficient(1, this->sizeX, this->sizeY, this->sizeZ);
	float scaleCoef = pModel->GetScaleCoefficient();
	glScalef(scaleCoef, scaleCoef, scaleCoef);
	this->pModelANDRenderer.GetPRenderer()->Render(pModel, withMaterial, flatShaded);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

#endif
template <class _PModelAndPRenderer>
void classCharacterWithBodyODE<_PModelAndPRenderer>::transferASightOnAMousePush(int x, int y)
{
	(*this).zahvatX = x;
	(*this).zahvatY = y;
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::mouseMove(int x, int y, int windowWidht, int windowHeight, int *oldx, int *oldy)
{

	if (oldx) (*oldx) = (*this).zahvatX;
	if (oldy) (*oldy) = (*this).zahvatY;
	
	if (!bodyIsCreated) return false;

	if (x >= 0 && x <= windowWidht && y >= 0 && y <= windowHeight)
	{
		if (rotLim)
		{
			dJointSetAMotorAxis(motorX, 0, 0, 0, 0, -1);
			//dJointSetAMotorAxis(motorX,1,0,0,0,0);
			dJointSetAMotorAxis(motorX, 2, 0, 1, 0, 0);
		}


		float ax_ = ((*this).zahvatX - x) * (2 * pi / windowWidht);
		
		if (ax_ >= pi) ax_ -= -2 * pi;
		if (ax_ <= (-pi)) ax_ += 2 * pi;

		(*this).ay -= ((*this).zahvatY - y) * (pi / windowHeight);
		if ((*this).ay >= (pi / 2)) (*this).ay = pi / 2;
		if ((*this).ay <= (-pi / 2)) (*this).ay = -pi / 2;

		(*this).zahvatX = x;
		(*this).zahvatY = y;

		if (rotLim)
		{
			dJointSetAMotorParam(motorX, dParamLoStop1, ax_);
			dJointSetAMotorParam(motorX, dParamHiStop1, ax_);
			dJointSetAMotorParam(motorX, dParamLoStop2, -0);
			dJointSetAMotorParam(motorX, dParamHiStop2, 0);
			dJointSetAMotorParam(motorX, dParamLoStop3, -0);
			dJointSetAMotorParam(motorX, dParamHiStop3, 0);
		}

		return true;
	}
	return false;
}

//-1 - меня тут вообще нет, 0 - я есть, но данных нет, 1 - установлено
template <class _PModelAndPRenderer>
int classCharacterWithBodyODE<_PModelAndPRenderer>::SetCollideOptions(dContact &contact, dGeomID geom1, dGeomID geom2)
{
	
	if (!bodyIsCreated) return false;
	dGeomID myGeom=GetGeomID();
	if ((geom1 != myGeom && geom2 != myGeom)||(geom1 == myGeom && geom2 == myGeom) || geom1 == 0 || geom2 == 0) return -1;
	dGeomID partnterGeomID = (myGeom == geom1 ? geom2 : geom1);

	typeArrayPartners::iterator iter = GetPointInfoCollidePartners(partnterGeomID);
	if (iter != collidePartners.end())
	{
		contact.surface.mode = dContactSoftCFM | dContactSoftERP;
		contact.surface.soft_cfm = (iter->step*iter->k_elasticity) / (iter->step*iter->k_elasticity+iter->k_braking);
		contact.surface.soft_erp = 1 / (iter->step*iter->k_elasticity+iter->k_braking);
		contact.surface.mu = iter->mu;
		if (iter->setFdir1)
		{
			contact.surface.mode |= dContactFDir1;
			dBodyVectorToWorld(bodyID, 0, 1, 0, contact.fdir1);
		}
	}
	else
		return 0;
	return 1;
}
template <class _PModelAndPRenderer>
void classCharacterWithBodyODE<_PModelAndPRenderer>::AddCollidePartner(dGeomID partner_geomID, dReal step, dReal k_elasticity, dReal k_braking, dReal mu, bool setFdir1)
{
	str_partner newParnter;
	newParnter.partner_geomID = partner_geomID;
	newParnter.step = step;
	newParnter.k_elasticity = k_elasticity;
	newParnter.k_braking = k_braking;
	newParnter.mu = mu;
	newParnter.setFdir1 = setFdir1;
	collidePartners.push_back(newParnter);
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::RemoveCollidePartner(dGeomID partner_geomID)
{
	typeArrayPartners::iterator iter = GetPointInfoCollidePartners(partner_geomID);
	if (iter == collidePartners.end()) return false;
	collidePartners.erase(iter);
	return true;

}
template <class _PModelAndPRenderer>
typeArrayPartners::iterator classCharacterWithBodyODE<_PModelAndPRenderer>::GetPointInfoCollidePartners(dGeomID partner_geomID)
{
	typeArrayPartners::iterator iter;
	const typeArrayPartners::iterator iter_end = collidePartners.end();
	for(iter = collidePartners.begin(); iter != iter_end; iter++)
		if (iter->partner_geomID == partner_geomID) break;
	return iter;
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::SetBackTypeFig(int type)
{
	if (bodyIsCreated) return false;
	if (type != PARALLELEPIPED && type != CYLINDER && type != SHAPE) return false;
	BackTypeFig = type;
	return true;
}
template <class _PModelAndPRenderer>
int classCharacterWithBodyODE<_PModelAndPRenderer>::GetBackTypeFig()
{
	return BackTypeFig;
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::DrawBackFig(bool externalFunc)
{
	if (!bodyIsCreated) return false;
	if (externalFunc)
	{
		if (DrawBackFigPoint) (*DrawBackFigPoint)(this);
		return true;
	}
#ifdef CHARDRAWOGL
	GLfloat local_matrix[16];
	ODE_TranslateRotate_2_OpenGL(local_matrix, this->geomID);
	glPushMatrix();
	glMultMatrixf(local_matrix);


	msModelRenderer *r = pModelANDRenderer.GetPRenderer();
	msModel_v2 *m = pModelANDRenderer.GetPModel();
#if 0
	if (r && m)
	{
		glPushMatrix();
		glRotatef(180,0,0,1);
		glRotatef(-90,1,0,0);
		glTranslatef(0,-this->sizeZ/2,0);
		m->calcScaleCoefficient(1,this->sizeX,this->sizeY, this->sizeZ);
		float scaleCoef=m->GetScaleCoefficient();		
		glScalef(scaleCoef,scaleCoef,scaleCoef);
		r->Render(m,true,false);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glPopMatrix();

		//glPopMatrix();
		//return true;
	}
#endif

#endif
	switch (BackTypeFig)
	{
	case CYLINDER:
		{
#ifdef CHARDRAWOGL
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glTranslated(0, 0, -sizeZ/2);
			GLUquadric* gluq = gluNewQuadric();
			gluCylinder(gluq, sizeX / 2, sizeX / 2, sizeZ, 40, 40);
			gluDeleteQuadric(gluq);
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
#endif
			break;
		}
	case SHAPE:
		{
#ifdef CHARDRAWOGL
			GLUquadricObj* sphere=gluNewQuadric();
			gluSphere(sphere,sizeX/2,30,30);
			gluDeleteQuadric(sphere);
#endif
			break;
		}
	default:
		{
#ifdef CHARDRAWOGL

			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			auxSolidBox(sizeX,sizeY,sizeZ);
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

#endif
			break;
		}
	}
#ifdef CHARDRAWOGL
	glPopMatrix();
#endif
	return true;

}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::SetRotLim(bool b)
{
	if (bodyIsCreated) return false;
	rotLim=b;
	return true;
}
template <class _PModelAndPRenderer>
bool classCharacterWithBodyODE<_PModelAndPRenderer>::GetRotLim()
{
	return rotLim;
}
