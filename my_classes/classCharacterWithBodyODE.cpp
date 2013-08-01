#include"classCharacterWithBodyODE.h"
//#include "windows.h"
//#include "GL/gl.h"
//#include "GL/glu.h"
//#include <GL/glaux.h>
//#include "ODEGLLIB.h"
#define pi 3.1415926535897932384626433832795

#include "msModel_v2.h"

classCharacterWithBodyODE::classCharacterWithBodyODE()
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
classCharacterWithBodyODE::~classCharacterWithBodyODE()
{
	DestroyBody();
}
bool classCharacterWithBodyODE::CreateBody(dSpaceID space,  dWorldID world, float xx, float yy, float zz)
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
void classCharacterWithBodyODE::DestroyBody()
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

bool classCharacterWithBodyODE::SetTotalMass(float totalMass)
{
	if (!bodyIsCreated && totalMass>0)
	{
		total_mass=totalMass;
		return true;
	}
	return false;
}


dReal classCharacterWithBodyODE::GetTotalMass()
{
	return total_mass;
}
dBodyID classCharacterWithBodyODE::GetBodyID()
{
	return bodyID;
}
dGeomID classCharacterWithBodyODE::GetGeomID()
{
	return geomID;
}
bool classCharacterWithBodyODE::SetGeomSize(float dx, float dy, float dz)
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
float classCharacterWithBodyODE::GetGeomSizeX()
{
	return sizeX;
}
float classCharacterWithBodyODE::GetGeomSizeY()
{
	return sizeY;
}
float classCharacterWithBodyODE::GetGeomSizeZ()
{
	return sizeZ;
}
bool classCharacterWithBodyODE::BodyIsCreated()
{
	return bodyIsCreated;
}
bool classCharacterWithBodyODE::AddForceForward(dReal force)
{
	if (bodyIsCreated)
	{
		//const dReal *orientBody=dBodyGetRotation(bodyID);
		dBodyAddRelForce(bodyID,0,force,0);
		return true;
	}
	return false;
}
bool classCharacterWithBodyODE::AddForceBack(dReal force)
{
	if (bodyIsCreated)
	{
		dBodyAddRelForce(bodyID,0,-force,0);
		return true;
	}
	return false;
}
bool classCharacterWithBodyODE::AddForceLeft(dReal force)
{
	if (bodyIsCreated)
	{
		dBodyAddRelForce(bodyID,force,0,0);
		return true;
	}
	return false;
}
bool classCharacterWithBodyODE::AddForceRight(dReal force)
{
	if (bodyIsCreated)
	{
		dBodyAddRelForce(bodyID,-force,0,0);
		return true;
	}
	return false;
}
bool classCharacterWithBodyODE::AddForceUp(dReal force)
{
	if (bodyIsCreated)
	{
		dBodyAddRelForce(bodyID,0,0,-force);
		return true;
	}
	return false;
}
#ifdef CHARDRAWOGL
	bool classCharacterWithBodyODE::openEyesOGL(void)
	{
		if (!rotLim || !bodyIsCreated) return false;
		const dReal *posBody=dBodyGetPosition(bodyID);
		const dReal *orientBody=dBodyGetRotation(bodyID);

		dReal posCam[3];
		posCam[0]=posBody[0]-orientBody[1]*2;//+orientBody[1];//*sizeY;
		posCam[1]=posBody[1]-orientBody[5]*2+(1-sin((*this).ay));//+orientBody[5];//*sizeY;
		posCam[2]=posBody[2]-orientBody[9]*2;//+orientBody[9];//*sizeY+2;

		//float dx=1;
		//float nX=posCam[0]+dx;
		//float nY=((nX-posBody[0])/(posCam[0]-posBody[0]))*(posCam[1]-posBody[1])+posBody[1];
		//posCam[0]=nX;
		//posCam[1]=nY;

		////posCam[1]+=10*sin((*this).ay);
		//
		//if ((*this).ay<0.1 && (*this).ay>-0.1)
		//{
		//	bool l;
		//	l=true;
		//}
		//float t=sqrt((posCam[0]-posBody[0])*(posCam[0]-posBody[0])+(posCam[1]-posBody[1])*(posCam[1]-posBody[1])+(posCam[2]-posBody[2])*(posCam[2]-posBody[2]));


		dReal posLook[3];
		posLook[0]=posBody[0];
		posLook[1]=posBody[1];
		posLook[2]=posBody[2];

		dReal posUP[3];
		posUP[0]=-orientBody[2];
		posUP[1]=-orientBody[6];
		posUP[2]=-orientBody[10];


		gluLookAt(posCam[0],posCam[1],posCam[2],posLook[0],posLook[1]/*-sin((*this).ay)*/,posLook[2],posUP[0],posUP[1],posUP[2]);
		return true;
	}

#endif

void classCharacterWithBodyODE::transferASightOnAMousePush(int x, int y)
{
	(*this).zahvatX=x;
	(*this).zahvatY=y;
}

bool classCharacterWithBodyODE::mouseMove(int x, int y, int windowWidht, int windowHeight, int *oldx, int *oldy)
{

	if (oldx) (*oldx)=(*this).zahvatX;
	if (oldy) (*oldy)=(*this).zahvatY;
	
	if (!bodyIsCreated) return false;

	if (x>=0 && x<=windowWidht && y>=0 && y<=windowHeight)
	{
		if (rotLim)
		{
			dJointSetAMotorAxis(motorX,0,0,0,0,-1);
			//dJointSetAMotorAxis(motorX,1,0,0,0,0);
			dJointSetAMotorAxis(motorX,2,0,1,0,0);
		}


		float ax_=((*this).zahvatX-x)*(2*pi/windowWidht);
		
		if (ax_>=pi) ax_-=-2*pi;
		if (ax_<=(-pi)) ax_+=2*pi;

		(*this).ay-=((*this).zahvatY-y)*(pi/windowHeight);
		if ((*this).ay>=(pi/2)) (*this).ay=pi/2;
		if ((*this).ay<=(-pi/2)) (*this).ay=-pi/2;

		(*this).zahvatX=x;
		(*this).zahvatY=y;

		if (rotLim)
		{
			dJointSetAMotorParam(motorX,dParamLoStop1,ax_);
			dJointSetAMotorParam(motorX,dParamHiStop1,ax_);
			dJointSetAMotorParam(motorX,dParamLoStop2,-0);
			dJointSetAMotorParam(motorX,dParamHiStop2,0);
			dJointSetAMotorParam(motorX,dParamLoStop3,-0);
			dJointSetAMotorParam(motorX,dParamHiStop3,0);
		}

		return true;
	}
	return false;
}
//-1 - меня тут вообще нет, 0 - я есть, но данным нет, 1 - установлено
int classCharacterWithBodyODE::SetCollideOptions(dContact &contact, dGeomID geom1, dGeomID geom2)
{
	
	if (!bodyIsCreated) return false;
	dGeomID myGeom=GetGeomID();
	if ((geom1!=myGeom && geom2!=myGeom)||(geom1==myGeom && geom2==myGeom) || geom1==0 || geom2==0) return -1;
	dGeomID partnterGeomID=(myGeom==geom1 ? geom2 : geom1);

	typeArrayPartners::iterator iter=GetPointInfoCollidePartners(partnterGeomID);
	if (iter!=collidePartners.end())
	{
		contact.surface.mode = dContactSoftCFM | dContactSoftERP;
		contact.surface.soft_cfm=(iter->step*iter->k_elasticity)/(iter->step*iter->k_elasticity+iter->k_braking);
		contact.surface.soft_erp=1/(iter->step*iter->k_elasticity+iter->k_braking);
		contact.surface.mu=iter->mu;
		if (iter->setFdir1)
		{
			contact.surface.mode|=dContactFDir1;
			dBodyVectorToWorld(bodyID,0,1,0,contact.fdir1);
		}
	}
	else
		return 0;
	return 1;
}
void classCharacterWithBodyODE::AddCollidePartner(dGeomID partner_geomID, dReal step, dReal k_elasticity, dReal k_braking, dReal mu, bool setFdir1)
{
	str_partner newParnter;
	newParnter.partner_geomID=partner_geomID;
	newParnter.step=step;
	newParnter.k_elasticity=k_elasticity;
	newParnter.k_braking=k_braking;
	newParnter.mu=mu;
	newParnter.setFdir1=setFdir1;
	collidePartners.push_back(newParnter);
}
bool classCharacterWithBodyODE::RemoveCollidePartner(dGeomID partner_geomID)
{
	typeArrayPartners::iterator iter=GetPointInfoCollidePartners(partner_geomID);
	if (iter==collidePartners.end()) return false;
	collidePartners.erase(iter);
	return true;

}
typeArrayPartners::iterator classCharacterWithBodyODE::GetPointInfoCollidePartners(dGeomID partner_geomID)
{
	typeArrayPartners::iterator iter;
	const typeArrayPartners::iterator iter_end=collidePartners.end();
	for(iter=collidePartners.begin(); iter!=iter_end; iter++)
		if (iter->partner_geomID==partner_geomID) break;
	return iter;
}
bool classCharacterWithBodyODE::SetBackTypeFig(int type)
{
	if (bodyIsCreated) return false;
	if (type!=PARALLELEPIPED && type!=CYLINDER && type!=SHAPE) return false;
	BackTypeFig=type;
	return true;
}
int classCharacterWithBodyODE::GetBackTypeFig()
{
	return BackTypeFig;
}
bool classCharacterWithBodyODE::DrawBackFig(bool externalFunc)
{
	if (!bodyIsCreated) return false;
	if (externalFunc)
	{
		if (DrawBackFigPoint) (*DrawBackFigPoint)(this);
		return true;
	}
#ifdef CHARDRAWOGL
	GLfloat local_matrix[16];
	ODE_TranslateRotate_2_OpenGL(local_matrix,geomID);
	glPushMatrix();
	glMultMatrixf(local_matrix);

	
	if (pModelANDRenderer.pRenderer && pModelANDRenderer.pModel)
	{
		glPushMatrix();
		glRotatef(180,0,0,1);
		glRotatef(-90,1,0,0);
		glTranslatef(0,-this->sizeZ/2,0);
		pModelANDRenderer.pModel->calcScaleCoefficient(1,this->sizeX,this->sizeY, this->sizeZ);
		float scaleCoef=pModelANDRenderer.pModel->GetScaleCoefficient();		
		glScalef(scaleCoef,scaleCoef,scaleCoef);
		pModelANDRenderer.pRenderer->Render(pModelANDRenderer.pModel,true,false);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glPopMatrix();
		//glPopMatrix();
		//return true;
	}



#endif
	switch (BackTypeFig)
	{
	case CYLINDER:
		{
#ifdef CHARDRAWOGL
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glTranslated(0,0,-sizeZ/2);
			GLUquadric* gluq=gluNewQuadric();
			gluCylinder(gluq,sizeX/2,sizeX/2,sizeZ,40,40);
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
bool classCharacterWithBodyODE::SetRotLim(bool b)
{
	if (bodyIsCreated) return false;
	rotLim=b;
	return true;
}
bool classCharacterWithBodyODE::GetRotLim()
{
	return rotLim;
}
