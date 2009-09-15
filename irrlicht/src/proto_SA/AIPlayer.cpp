#include "CSAApp.h"
#include "HeroPlayer.h"

#include "AIPlayer.h"

CAIPlayer::CAIPlayer(void)
{
	m_strTypeName = "CAIPlayer";	
}

CAIPlayer::~CAIPlayer(void)
{
}

bool CAIPlayer::Init(irr::scene::ISceneNode *pNode)
{
	if(CPlayer::Init(pNode))
	{

		irr::scene::CBulletObjectAnimatorParams physicsParams;
		irr::scene::ISceneManager *pSmgr = CSAApp::GetPtr()->m_pSmgr;
		irr::IrrlichtDevice *pDevice = CSAApp::GetPtr()->m_pDevice;
		irr::scene::CBulletAnimatorManager* pBulletPhysicsFactory = CSAApp::GetPtr()->m_pBulletPhysicsFactory; 
		irr::scene::CBulletWorldAnimator* pWorldAnimator = CSAApp::GetPtr()->m_pWorldAnimator;

		if(m_pCollMngNode && 
			m_pTrigerNode && 			
			m_pCollMngNode_Body && //������Ʈ�� ������ ���� �˻��ϰ�
			(m_pNode->getType() == irr::scene::jz3d::CFormatedAnimationNode::TypeID) //�ƴ϶�� �ش������Ʈ�� �����Ѵ�.
			)
		{
			//m_pNode->setVisible(false);		

			m_pTrigerNode = pSmgr->getSceneNodeFromName("usr/triger");
			m_pCollMngNode = (irr::scene::jz3d::CCollusionMngNode *)pSmgr->getSceneNodeFromName("col_main",m_pNode);//ĳ���ͺ������(�̵������)
			m_pCollMngNode_Kick = (irr::scene::jz3d::CCollusionMngNode *)pSmgr->getSceneNodeFromName("col_kick",m_pNode); //������ �浹����
			m_pCollMngNode_Body = (irr::scene::jz3d::CCollusionMngNode *)pSmgr->getSceneNodeFromName("col_body",m_pNode); //���� �浹����		

			physicsParams.mass = 70.f; //70kg
			physicsParams.gravity = core::vector3df(0, 0, 0);
			physicsParams.friction = 10.f; //������		


			irr::scene::CBulletChracterAnimator *pAnim = pBulletPhysicsFactory->createBulletCharcterAnimator(
				pSmgr,
				m_pNode,
				pWorldAnimator->getID(),
				m_pCollMngNode->getGeometryInfo(),
				&physicsParams
				);

			m_pNode->addAnimator(pAnim);
			m_pChracterAnimator = pAnim;	

			//�θ����� �����Ϻ�ȯ�� ����ϱ� ���ؼ� �Ѵܰ����� ��ĳ�Ϻ�ȯ�� �����ش�.
			m_pChracterAnimator->setLocalPosition(
				-(m_pCollMngNode->getPosition() * m_pCollMngNode->getParent()->getScale())
				);

			m_pcloBody = m_pChracterAnimator->getRigidBody();

			pAnim->drop();

			return true;
		}
		else
		{
			char szBuf[256];
			sprintf_s(szBuf,256,"can not process CPlayer::Init [%s] scenenode check node format!!",m_pNode->getName());
			pDevice->getLogger()->log(szBuf,irr::ELL_ERROR);
			return false;
		}
	}

	return false;
}

void CAIPlayer::Signal(std::string strSignal,void *pParam)
{
	if(!isDie())
	{
		if(strSignal == "hit")
		{
			CPlayer *doer = (CPlayer *)pParam;
			irr::core::vector3df vdodir = doer->getPosition() - getPosition();
			vdodir.normalize();

			irr::core::vector3df vblowDir(-vdodir.X,0,-vdodir.Z);		
			m_pChracterAnimator->applyImpulse(700.f * vblowDir);
			m_pChracterAnimator->applyImpulse(70.f * irr::core::vector3df(0,1,0));

			SetStatus(FSM_ATTACKED);
		}
	}
}

void CAIPlayer::Update(irr::f32 fDelta)
{
	irr::scene::ISceneManager *pSmgr = CSAApp::GetPtr()->m_pSmgr;
	irr::IrrlichtDevice *pDevice = CSAApp::GetPtr()->m_pDevice;
	irr::scene::CBulletAnimatorManager* pBulletPhysicsFactory = CSAApp::GetPtr()->m_pBulletPhysicsFactory; 
	irr::scene::CBulletWorldAnimator* pWorldAnimator = CSAApp::GetPtr()->m_pWorldAnimator;

	switch(GetStatus())
	{
	case FSM_READY:
		{
			//������ġ ����
			m_pNode->setVisible(true);			
			irr::core::vector3df pos_Spwan = pSmgr->getSceneNodeFromName("genpos/zombie",m_pTrigerNode)->getAbsolutePosition();
			m_pChracterAnimator->setPosition(pos_Spwan);
			m_pChracterAnimator->zeroForces();
			//ĳ���� �߽����� �������ش�.
			

			SetStatus(FSM_STAND);
		}
		break;
	case FSM_STAND:		
		if(GetStep() == 0)
		{
			m_pNode->changeAction("stand");
			IncStep();
			m_accTick = 0; //�����ð� ����
		}
		else if(GetStep() == 1)
		{		
			if(m_accTick >= 2.0f) //2�ʰ��
			{
				SetStatus(FSM_WALK);
				
				m_vTargetDir.X = (rand() % 1000) / 1000.f - 0.5f;
				m_vTargetDir.Z = (rand() % 1000) / 1000.f - 0.5f;
				m_vTargetDir.Y = 0;

				m_vTargetDir.normalize();

			}
		}
		break;
	case FSM_WALK:
		if(GetStep() == 0)
		{
			m_pNode->changeAction("walk");
			IncStep();

			m_accTick = 0; //�����ð� ����
		}
		else if(GetStep() == 1)
		{
			if(m_accTick >= 3.0f) //3�ʰ��
			{
				SetStatus(FSM_STAND);
			}
			else
			{
				btVector3 WalkVelocity(0,0,0);
				btScalar speed = 0;			
				speed = btScalar(1.1) * 180.0f * fDelta;

				btVector3 btDir;
				Irrlicht2Bullet(m_vTargetDir,btDir);

				WalkVelocity = btDir * speed;
				m_pChracterAnimator->controlStep_Walker(WalkVelocity);
			}			
		}
		break;
	case FSM_KICK:
		break;
	case FSM_ATTACKED:
		if(GetStep() == 0)
		{
			m_pNode->changeAction("damage",false);			

			IncStep();
		}
		else if(GetStep() == 1)
		{	
			if(m_pNode->IsActionFinished()) 
			{	
				SetStatus(FSM_STAND);
			}
		}
		break;	
	}

	//���� Ÿ�̸�
	m_accTick += fDelta;
}
