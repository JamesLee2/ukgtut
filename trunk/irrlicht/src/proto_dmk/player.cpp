#include "CdmkApp.h"
#include "player.h"

bool CPlayer::Init(irr::scene::ISceneNode *pNode)
{
	irr::scene::CBulletObjectAnimatorParams physicsParams;
	irr::scene::ISceneManager *pSmgr = CdmkApp::GetPtr()->m_pSmgr;
	irr::IrrlichtDevice *pDevice = CdmkApp::GetPtr()->m_pDevice;
	irr::scene::CBulletAnimatorManager* pBulletPhysicsFactory = CdmkApp::GetPtr()->m_pBulletPhysicsFactory; 
	irr::scene::CBulletWorldAnimator* pWorldAnimator = CdmkApp::GetPtr()->m_pWorldAnimator;


	if(pNode->getType() != irr::scene::jz3d::CFormatedAnimationNode::TypeID)
		return false;

	m_pNode = (irr::scene::jz3d::CFormatedAnimationNode *)pNode;//pSmgr->getSceneNodeFromName("usr/obj/b3d/ninja/blue");
	//m_pNode->OnAnimate(0);		


	if(m_pCollMngNode && 
		m_pTrigerNode && 			
		m_pCollMngNode_Body && //������Ʈ�� ������ ���� �˻��ϰ�
		(m_pNode->getType() == irr::scene::jz3d::CFormatedAnimationNode::TypeID) //�ƴ϶�� �ش������Ʈ�� �����Ѵ�.
		)
	{
		m_pNode->setVisible(false);		


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

		//m_pChracterAnimator->setLocalPosition(-m_pCollMngNode->getPosition());			
		//m_pChracterAnimator->getRigidBody()->setDamping(.9,0);

		pAnim->drop();	

	}

	else
	{
		char szBuf[256];
		sprintf_s(szBuf,256,"can not process CPlayer::Init [%s] scenenode check node format!!",m_pNode->getName());
		pDevice->getLogger()->log(szBuf,irr::ELL_ERROR);
		return false;
	}

	return true;
}