#include "CSAApp.h"
#include "ChsBullet.h"

ChsBullet::ChsBullet(void)
{
}

ChsBullet::~ChsBullet(void)
{
}

//! ���ϰ� 0: ���θ� ����, 1: ����
int ChsBullet::Init(void *pParam)
{	
	irr::scene::CBulletObjectAnimatorParams physicsParams;
	irr::scene::ISceneManager *pSmgr = CSAApp::GetPtr()->m_pSmgr;
	irr::IrrlichtDevice *pDevice = CSAApp::GetPtr()->m_pDevice;
	irr::scene::CBulletAnimatorManager* pBulletPhysicsFactory = CSAApp::GetPtr()->m_pBulletPhysicsFactory; 
	irr::scene::CBulletWorldAnimator* pWorldAnimator = CSAApp::GetPtr()->m_pWorldAnimator;

	ChsBullet::Params *pbulletPram = (ChsBullet::Params *)pParam;

	if(pbulletPram) {			
		irr::scene::CBulletObjectAnimatorGeometry geom;
		irr::scene::CBulletObjectAnimatorParams physicsParams;

		irr::scene::ISceneNode *pNode;
		pNode = pSmgr->addSphereSceneNode(.25f,8);
		pNode->setMaterialFlag(irr::video::EMF_LIGHTING,false);
		pNode->setMaterialFlag(irr::video::EMF_WIREFRAME,true);				
		pNode->setPosition(irr::core::vector3df(pbulletPram->vInitPos));

		geom.type = irr::scene::CBPAGT_SPHERE;
		geom.sphere.radius = .5f;

		//���� ��Ʈ����
		physicsParams.mass = pbulletPram->mass;// 0.1f;
		physicsParams.gravity = core::vector3df(0, 0, 0);
		physicsParams.friction = .6f; //������
		physicsParams.restitution = 0.0f; //ź��

		irr::scene::CBulletObjectAnimator *pAnim = pBulletPhysicsFactory->createBulletObjectAnimator(
			pSmgr,
			pNode,
			pWorldAnimator->getID(),
			&geom,
			&physicsParams
			);

		pAnim->applyImpulse(pbulletPram->Energy * pbulletPram->vAim);
		//�ͳθ�����
		pAnim->setCCD(1,.2f * (.05f));
		
	
		pNode->addAnimator(pAnim);					
		
		//��ü���
		m_pNode = pNode;
		m_pAnim = pAnim;	
		
		
		pAnim->drop();	

		SetStatus(ChsBullet::FSM_READY);

		return 1;
	}

	return 0;
}

void ChsBullet::Signal(std::string strMsg,void *pParam)
{
	if(isDie() == false)
	{
		if(strMsg == "hit")
		{
			m_pAnim->zeroForces();		
			SetStatus(FSM_HIT);
		}
	}
}

void ChsBullet::Update(irr::f32 fTick)
{

	irr::scene::ISceneManager *pSmgr = CSAApp::GetPtr()->m_pSmgr;

	switch(GetStatus())
	{
	case ChsBullet::FSM_READY:
		{
			SetStatus(ChsBullet::FSM_MOVE);
		}
		break;
	case ChsBullet::FSM_MOVE:
		break;
	case ChsBullet::FSM_HIT:
		if(GetStep() == 0)
		{
			m_pAnim->RemoveFromWorld();
			irr::scene::ISceneNodeAnimator *pAniDel = pSmgr->createDeleteAnimator(0);
			m_pNode->addAnimator(pAniDel);
			pAniDel->drop();
			IncStep();			
		}		
		else
		{
			SetStatus(ChsBullet::FSM_DIE);
		}
		break;
	case ChsBullet::FSM_DIE:
		
		break;	
	}

}
