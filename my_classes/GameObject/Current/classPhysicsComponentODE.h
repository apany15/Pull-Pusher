#pragma once
#include "../Abstract/classPhysicsComponentAbstract.h"

#include <ode/ode.h>
#include<vector>

#define PARALLELEPIPED 0
#define CYLINDER 1
#define SHAPE 2

namespace MY_NS_PHYSICS_COMPONENT
{

	//�������������� �������������� � �����
	struct str_partner_def
	{
		dReal step;				//��� "�������"
		dReal k_elasticity;		//����������� "���������"
		dReal k_braking;		//����������� "����������"
		dReal mu;				//���� ������ � �����
	};

	struct str_partner
	{
		dGeomID partner_geomID;	//id ��������� ����
		dReal step;				//��� "�������"
		dReal k_elasticity;		//����������� "���������"
		dReal k_braking;		//����������� "����������"
		dReal mu;				//���� ������ � �����
		bool setFdir1;
	};

	typedef std::vector<str_partner> typeArrayPartners;

	class classPhysicsComponentODE :
		public classPhysicsComponentAbstract
	{
	protected:

		int mouseOldX;
		int mouseOldY;
		int mouseFirstCatching;
		int windowWidth;
		int windowHeight;


		float sizeX;
		float sizeY;
		float sizeZ;
		dBodyID bodyID;
		dGeomID geomID;
		bool bodyIsCreated;
		dReal total_mass;
		float ay;	//���� �������� ����� ��� y
		int zahvatX;
		int zahvatY;
		dJointID motorX;
		typeArrayPartners collidePartners;
		typeArrayPartners::iterator GetPointInfoCollidePartners(dGeomID partner_geomID);
		int BackTypeFig;
		bool rotLim;	//����������� ���������������� ��� �������� (���������� ��� ��������� �� ������� ����)

		void transferASightOnAMousePush(int x, int y);
		bool mouseMove(int x, int y, int windowWidht, int windowHeight);

	public:
		void gameCycle(void);
		classPhysicsComponentODE(void);
		~classPhysicsComponentODE(void);

		bool opora;
		dGeomID oporaGeomID;
		dReal MaxForceToMove;
		float vel;
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
		
		

		bool mouseMove(int x, int y);

		str_partner_def defaultCollidePartner;
		//-1 - ���� ��� ������ ���, 0 - � ����, �� ������ ���, 1 - ����������� 
		int SetCollideOptions(dContact &contact, dGeomID geom1, dGeomID geom2);
		void AddCollidePartner(dGeomID partner_geomID, dReal step, dReal k_elasticity, dReal k_braking, dReal mu, bool setFdir1);
		bool RemoveCollidePartner(dGeomID partner_geomID);
		bool SetBackTypeFig(int type);
		int GetBackTypeFig();
		bool DrawBackFig(bool externalFunc=false);
		bool SetRotLim(bool b);
		bool GetRotLim();
	};
}
