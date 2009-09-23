#pragma once
#include "esenframe.h"
#include "bmeObjs.h"



class CGameFrame :
	public CEsenFrame
{
public:
	CGameFrame(void);
	~CGameFrame(void);

	CGameFrame(LPCSTR strName);	


	enum {
		PH_SLEEP,
		PH_READY,
		PH_PLAY,
		PH_PAUSE,
		PH_END,
		PH_TESTPLAY
	};

	enum {
		COLT_NONE=0,//�浹����
		COLT_ROAD,
		COLT_ICE,
		COLT_WATER,
		COLT_MUD,
		COLT_GROUND
	};

	//int m_nStage;
	virtual void Init();
	virtual void Release();
	virtual void Apply(irr::u32 timeMs);
	virtual void Render();
	virtual void LoadRes();
	virtual bool OnEvent(irr::SEvent ev);
	virtual void DecodePacket(irr::s32 nCode,void *pBuf);

	//static CGameFrame* ms_pGameFrame;
	//static int Create();
	//static void Terminate();
	//static CGameFrame* Get();

	irr::scene::ISceneNode *m_pSpawnNode;

	//������
	irr::scene::CBulletAnimatorManager* m_pBulletPhysicsFactory;
	irr::scene::ISceneNode*  m_pWorldNode;
	irr::scene::CBulletWorldAnimator* m_pWorldAnimator;
	//irr::scene::CBulletObjectAnimator* m_pObjectAnimator;	//�׽�Ʈ��


	//�浹����
	irr::scene::IMetaTriangleSelector* m_pmsWorlSelecter;   //���� ��ü �빰 �浹����
	irr::scene::IMetaTriangleSelector* m_pmsRoadSelecter; //���� �빰�� �浹����
	irr::scene::IMetaTriangleSelector* m_pmsGroundSelecter; //���� �빰�� �浹����
	irr::scene::IMetaTriangleSelector* m_pmsWallSelecter; //���� �빰�� �浹����
	//irr::scene::IMetaTriangleSelector* m_pmsTestObject; //�浹�˻��׽�Ʈ�� ������Ʈ
	//irr::scene::ITriangleSelector *m_pTsTerrain; //
	//irr::scene::ITriangleSelector *m_pTsFinishLine; //�ǴϽ����ο�
	irr::scene::jz3d::CCollusionMngNode *m_pColMFinishLine;

	bool m_bCollGround;
	int m_nCollType;
	irr::core::triangle3df m_triIntersecWall;
	irr::core::triangle3df m_triIntersecGround;
	irr::core::line3df m_RayfrontColl;

	bool m_bPauseKey;
	bool m_bResume; //�����ϴٰ� �ߴ��������ֳ�? 
	
	CObj_ThisBycle m_ThisBycle;	

	
	//��ġ�� �ΰ���������� �浹 ���̱��ϱ�
	bool GetCollisionHeight(float x,float y,float &z);
	bool GetCollisionWall(irr::scene::ISceneNode *pNode,irr::core::vector3df vtRayPosDiff,irr::f32 fRayLength, irr::f32 fYaw, irr::f32 &fDist, irr::core::triangle3df *pTri);

	void Octree2Bullet(irr::scene::ISceneNode *pOctRoot);

};

//inline CGameFrame *CGameFrame::Get()
//{
//	return ms_pGameFrame;
//}
