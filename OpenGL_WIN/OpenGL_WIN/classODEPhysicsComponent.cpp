#include "classODEPhysicsComponent.h"
#define pi 3.1415926535897932384626433832795

namespace MY_NS_PHYSICS_COMPONENT
{
	
	void classODEPhysicsComponent::gameCycle(void)
	{
	}

	classODEPhysicsComponent::classODEPhysicsComponent(void)
	{
		this->sizeX = 0;
		this->sizeY = 0;
		this->sizeZ = 0;

		this->total_mass = 70;
		this->MaxForceToMove = 3000;
		this->bodyIsCreated = false;

		this->defaultCollidePartner.step = 1;				//��� "�������"
		this->defaultCollidePartner.k_elasticity = 0.001;	//����������� "���������"
		this->defaultCollidePartner.k_braking = 2;			//����������� "����������"
		this->defaultCollidePartner.mu = dInfinity;			//���� ������ � �����
		this->BackTypeFig = PARALLELEPIPED;
		this->rotLim = true;
	}

	classODEPhysicsComponent::~classODEPhysicsComponent(void)
	{
		this->DestroyBody();
	}

	bool classODEPhysicsComponent::CreateBody(dSpaceID space,  dWorldID world, float xx, float yy, float zz)
	{
		if (this->bodyIsCreated) return false;
		if (this->sizeX == 0 || this->sizeY == 0 || this->sizeZ == 0) return false;

		dMass m;
		switch (this->BackTypeFig)
		{
		case CYLINDER:
			{
				this->geomID = dCreateCylinder(space,this->sizeX / 2, this->sizeZ);
				dMassSetCylinderTotal(&m, this->total_mass, 2, this->sizeX / 2, this->sizeZ);
				break;
			}
		case SHAPE:
			{
				this->geomID = dCreateSphere(space, this->sizeX / 2);
				dMassSetSphereTotal(&m, this->total_mass, this->sizeX / 2);
				break;
			}
		default:
			{
				this->geomID = dCreateBox(space, this->sizeX, this->sizeY, this->sizeZ);
				dMassSetBoxTotal(&m, this->total_mass, this->sizeX, this->sizeY, this->sizeZ);
				break;
			}
		}

		this->bodyID = dBodyCreate(world);
		dBodySetMass(this->bodyID, &m);
		dGeomSetBody(this->geomID, this->bodyID);

		dMatrix3 matr;
		dRFromAxisAndAngle(matr, 1, 0, 0, pi / 2);
		dGeomSetRotation(this->geomID, matr);


		if (this->rotLim)	//���� ���� ����������� �� ���������������� (���������� ��� ��������� �� ������� ����)
		{
			this->motorX = dJointCreateAMotor(world, 0);
			dJointAttach(this->motorX, this->bodyID, 0);
			dJointSetAMotorMode(this->motorX, dAMotorEuler);

			dJointSetAMotorAxis(this->motorX, 0, 0, 0, 0, -1);
			dJointSetAMotorAxis(this->motorX, 2, 0, 1, 0, 0);

			dJointSetAMotorParam(this->motorX, dParamHiStop, 0);
			dJointSetAMotorParam(this->motorX, dParamHiStop3, 0);
			dJointSetAMotorParam(this->motorX, dParamHiStop2, 0);
		}


		dBodySetPosition(this->bodyID, xx, yy, zz);

		this->bodyIsCreated = true;
		return true;
	}

	void classODEPhysicsComponent::DestroyBody()
	{
		if (this->bodyIsCreated)
		{
			dBodyDestroy(this->bodyID);
			dGeomDestroy(this->geomID);
			if (this->rotLim) dJointDestroy(this->motorX);
			this->collidePartners.clear();
			this->bodyIsCreated=false;
		}
	}

	bool classODEPhysicsComponent::SetTotalMass(float totalMass)
	{
		if (!this->bodyIsCreated && totalMass > 0)
		{
			this->total_mass = totalMass;
			return true;
		}
		return false;
	}

	dReal classODEPhysicsComponent::GetTotalMass()
	{
		return this->total_mass;
	}

	dBodyID classODEPhysicsComponent::GetBodyID()
	{
		return this->bodyID;
	}

	dGeomID classODEPhysicsComponent::GetGeomID()
	{
		return this->geomID;
	}

	bool classODEPhysicsComponent::SetGeomSize(float dx, float dy, float dz)
	{
		if (!this->bodyIsCreated)
		{
			if (dx <= 0) dx = 0.3;
			if (dy <= 0) dy = 0.3;
			if (dz <= 0) dz = 0.3;
			this->sizeX = dx;
			this->sizeY = dy;
			this->sizeZ = dz;
			return true;
		}
		return false;
	}

	float classODEPhysicsComponent::GetGeomSizeX()
	{
		return this->sizeX;
	}

	float classODEPhysicsComponent::GetGeomSizeY()
	{
		return this->sizeY;
	}

	float classODEPhysicsComponent::GetGeomSizeZ()
	{
		return this->sizeZ;
	}

	bool classODEPhysicsComponent::BodyIsCreated()
	{
		return this->bodyIsCreated;
	}

	bool classODEPhysicsComponent::AddForceForward(dReal force)
	{
		if (this->bodyIsCreated)
		{
			dBodyAddRelForce(this->bodyID, 0, force,0);
			return true;
		}
		return false;
	}

	bool classODEPhysicsComponent::AddForceBack(dReal force)
	{
		if (this->bodyIsCreated)
		{
			dBodyAddRelForce(this->bodyID, 0, -force, 0);
			return true;
		}
		return false;
	}

	bool classODEPhysicsComponent::AddForceLeft(dReal force)
	{
		if (this->bodyIsCreated)
		{
			dBodyAddRelForce(this->bodyID, force, 0, 0);
			return true;
		}
		return false;
	}

	bool classODEPhysicsComponent::AddForceRight(dReal force)
	{
		if (this->bodyIsCreated)
		{
			dBodyAddRelForce(this->bodyID, -force, 0, 0);
			return true;
		}
		return false;
	}

	bool classODEPhysicsComponent::AddForceUp(dReal force)
	{
		if (this->bodyIsCreated)
		{
			dBodyAddRelForce(this->bodyID, 0, 0, -force);
			return true;
		}
		return false;
	}

	bool classODEPhysicsComponent::mouseMove(int x, int y, int windowWidht, int windowHeight, int *oldx, int *oldy)
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


			float ax_ = ((*this).zahvatX-x) * (2 * pi / windowWidht);

			if (ax_ >= pi) ax_ -= -2 * pi;
			if (ax_ <= (-pi)) ax_ += 2 * pi;

			(*this).ay -= ((*this).zahvatY - y) * (pi / windowHeight);
			if ((*this).ay >= (pi / 2)) (*this).ay = pi / 2;
			if ((*this).ay <= (-pi / 2)) (*this).ay= -pi / 2;

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

	//-1 - ���� ��� ������ ���, 0 - � ����, �� ������ ���, 1 - �����������
	int classODEPhysicsComponent::SetCollideOptions(dContact &contact, dGeomID geom1, dGeomID geom2)
	{
		if (!this->bodyIsCreated) return false;
		dGeomID myGeom = this->geomID;
		if ((geom1 != myGeom && geom2 != myGeom)||(geom1 == myGeom && geom2 == myGeom) || geom1 == 0 || geom2 == 0) return -1;
		dGeomID partnterGeomID = (myGeom == geom1 ? geom2 : geom1);

		typeArrayPartners::iterator iter = this->GetPointInfoCollidePartners(partnterGeomID);
		if (iter != this->collidePartners.end())
		{
			contact.surface.mode = dContactSoftCFM | dContactSoftERP;
			contact.surface.soft_cfm = (iter->step * iter->k_elasticity) / (iter->step * iter->k_elasticity+iter->k_braking);
			contact.surface.soft_erp = 1 / (iter->step * iter->k_elasticity + iter->k_braking);
			contact.surface.mu = iter->mu;
			if (iter->setFdir1)
			{
				contact.surface.mode |= dContactFDir1;
				dBodyVectorToWorld(this->bodyID, 0, 1, 0, contact.fdir1);
			}
		}
		else 
		{
			return 0;
		}
		return 1;
	}

	void classODEPhysicsComponent::AddCollidePartner(dGeomID partner_geomID, dReal step, dReal k_elasticity, dReal k_braking, dReal mu, bool setFdir1)
	{
		str_partner newParnter;
		newParnter.partner_geomID = partner_geomID;
		newParnter.step = step;
		newParnter.k_elasticity = k_elasticity;
		newParnter.k_braking = k_braking;
		newParnter.mu = mu;
		newParnter.setFdir1 = setFdir1;
		this->collidePartners.push_back(newParnter);
	}

	bool classODEPhysicsComponent::RemoveCollidePartner(dGeomID partner_geomID)
	{
		typeArrayPartners::iterator iter = this->GetPointInfoCollidePartners(partner_geomID);
		if (iter == this->collidePartners.end()) return false;
		this->collidePartners.erase(iter);
		return true;

	}

	typeArrayPartners::iterator classODEPhysicsComponent::GetPointInfoCollidePartners(dGeomID partner_geomID)
	{
		typeArrayPartners::iterator iter;
		const typeArrayPartners::iterator iter_end = this->collidePartners.end();
		for(iter = this->collidePartners.begin(); iter != iter_end; iter++)
			if (iter->partner_geomID == partner_geomID) break;
		return iter;
	}

	bool classODEPhysicsComponent::SetBackTypeFig(int type)
	{
		if (this->bodyIsCreated) return false;
		if (type != PARALLELEPIPED && type != CYLINDER && type != SHAPE) return false;
		this->BackTypeFig = type;
		return true;
	}

	int classODEPhysicsComponent::GetBackTypeFig()
	{
		return this->BackTypeFig;
	}


	bool classODEPhysicsComponent::SetRotLim(bool b)
	{
		if (this->bodyIsCreated) return false;
		this->rotLim = b;
		return true;
	}

	bool classODEPhysicsComponent::GetRotLim()
	{
		return this-> rotLim;
	}

}