#include "CSAApp.h"

#include "ChsBullet.h"
#include "HeroPlayer.h"

/*

�ذ��ؾ��ҹ�����
1. ĳ���� ��Ʈ�ѷ��� �ͳθ����� - ����� ���� ������ �ͳθ��̹߻��Ҽ��ִ�.
�ذ����� �̵��������� ���� �˻縦 �����ؼ� ������ü�� ������ �������� ���߰� �ؾ��Ѵ�.

if( ĳ����Ʈ�ѷ��� �̵����� ������ ��ü�� �����ߴ°�? == false)
	m_pChracterAnimator->controlStep_Walker(WalkVelocity);

*/


CHeroPlayer::CHeroPlayer()
{
	m_strTypeName = "CHeroPlayer";	
	int i;
	for(i=0;i<irr::KEY_KEY_CODES_COUNT;i++)
	{
		m_Key[i] = false;
	}	

	SetStatus(FSM_READY);
}

CHeroPlayer::~CHeroPlayer(void)
{
			
}

bool CHeroPlayer::Init(irr::scene::ISceneNode *pNode)
{
	if(CPlayer::Init(pNode))
	{

		irr::scene::CBulletObjectAnimatorParams physicsParams;
		irr::scene::ISceneManager *pSmgr = CSAApp::GetPtr()->m_pSmgr;
		irr::IrrlichtDevice *pDevice = CSAApp::GetPtr()->m_pDevice;
		irr::scene::CBulletAnimatorManager* pBulletPhysicsFactory = CSAApp::GetPtr()->m_pBulletPhysicsFactory; 
		irr::scene::CBulletWorldAnimator* pWorldAnimator = CSAApp::GetPtr()->m_pWorldAnimator;


		m_pTrigerNode = pSmgr->getSceneNodeFromName("usr/triger");

		{
			m_pCollMngNode = (irr::scene::jz3d::CCollusionMngNode *)pSmgr->getSceneNodeFromName("col_main",m_pNode);//ĳ���ͺ������(�̵������)
			//m_pCollMngNode_Kick = (irr::scene::jz3d::CCollusionMngNode *)pSmgr->getSceneNodeFromName("col_kick",m_pNode); //������ �浹����
			//m_pCollMngNode_Body = (irr::scene::jz3d::CCollusionMngNode *)pSmgr->getSceneNodeFromName("col_body",m_pNode); //���� �浹����		

			physicsParams.mass = 70.f; //70kg
			physicsParams.gravity = core::vector3df(0, 0, 0);
			physicsParams.friction = 10.f; //������	
			
			//���� ī�޶���
			irr::scene::ISceneNode *pCamNode = pSmgr->getActiveCamera();
			
			irr::scene::CBulletFpsCamAnimator *pAnim = pBulletPhysicsFactory->createBulletFpsCamAnimator(
				pSmgr,
				pCamNode,
				pDevice->getCursorControl(),
				pWorldAnimator->getID(),
				m_pCollMngNode->getGeometryInfo(),
				&physicsParams
				);

			pCamNode->addAnimator(pAnim);	
			m_pChracterAnimator = pAnim;	
			pAnim->drop();
		}
		return true;
	}
	
	
	return false;
}



void CHeroPlayer::Update(irr::f32 fDelta)
{	
	irr::scene::ISceneManager *pSmgr = CSAApp::GetPtr()->m_pSmgr;
	irr::IrrlichtDevice *pDevice = CSAApp::GetPtr()->m_pDevice;
	irr::scene::CBulletAnimatorManager* pBulletPhysicsFactory = CSAApp::GetPtr()->m_pBulletPhysicsFactory; 
	irr::scene::CBulletWorldAnimator* pWorldAnimator = CSAApp::GetPtr()->m_pWorldAnimator;		

	m_pChracterAnimator->setMoveSpeed( 1.1f * 1800.f * fDelta ); // �ü� 18Ű�� ����

	switch(GetStatus())
	{
	case CHeroPlayer::FSM_READY:
		{
			//������ġ ����
			//m_pNode->setVisible(true);			
			irr::core::vector3df pos_Spwan = pSmgr->getSceneNodeFromName("start",m_pTrigerNode)->getAbsolutePosition();
			m_pChracterAnimator->setPosition(pos_Spwan);
			m_pChracterAnimator->zeroForces();
			m_pChracterAnimator->setLocalPosition(irr::core::vector3df(0,5,0));
			//m_pChracterAnimator->setMoveSpeed(1.1f);			

			SetStatus(FSM_STAND);
		}
		break;
	case CHeroPlayer::FSM_STAND:
		{
						
			if( m_Key[irr::KEY_LBUTTON] )
			{
				irr::scene::ISceneManager *pSmgr = pDevice->getSceneManager();
				irr::core::position2di mpos = pDevice->getCursorControl()->getPosition();				

				irr::core::line3df Ray;
				irr::core::vector3df vAim;

				Ray = pDevice->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(mpos);

				vAim = Ray.getVector();
				vAim.normalize(); 	

				ChsBullet *pBullet = new ChsBullet();

				ChsBullet::Params param_bullet;

				param_bullet.Energy = 10.f;
				param_bullet.mass = .1f;
				param_bullet.vAim = vAim;				
				param_bullet.vInitPos = getPosition() + (irr::core::vector3df(0,5,0) + vAim * 3); //�ѱ� ��ġ ����� �߻������ġ
				
				pBullet->Init(&param_bullet); //�Ѿ� �ʱ�ȭ

				m_mapBullets[pBullet->m_pAnim->getRigidBody()] = SP_IFSMObject(pBullet);

				m_Key[irr::KEY_LBUTTON] = false;
			}
		}		
		break;
	}


	//ź�� �浹 ����	
	{
		btDynamicsWorld *pWorld = pWorldAnimator->getWorld();
		int num = pWorld->getDispatcher()->getNumManifolds();		

		for(int i=0;i<num;i++)
		{
			btPersistentManifold *contactManifold = pWorld->getDispatcher()->getManifoldByIndexInternal(i);			
			
			if(contactManifold->getNumContacts() > 0)
			{				

				btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
				btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

				//�浹�� ������Ʈ ã��
				ChsBullet *pBullet = NULL;
				if(m_mapBullets.count(obB) != 0)
				{
					pBullet = (ChsBullet *)m_mapBullets[obB].get();
				}			
				if(m_mapBullets.count(obA) != 0)
				{
					pBullet = (ChsBullet *)m_mapBullets[obA].get();				
				}			

				if(pBullet)
				{
					pBullet->Signal("hit",NULL);

					btCollisionObject *pTar = CSAApp::Get().m_spZombie1->m_pcloBody; //���� ���� �浹������Ʈ ���

					//�ش�Ÿ���� ��������?
					if(obA == pTar || obB == pTar)
					{
						CSAApp::Get().m_spZombie1->Signal("hit",pBullet);
					}					
				}
			}

		}

		
		//�Ѿ� �����̳� ����
		{
			std::map<btCollisionObject *,SP_IFSMObject>::iterator it = m_mapBullets.begin();
			while(it != m_mapBullets.end())
			{			

				if(it->second->isDie())
				{				
					m_mapBullets.erase(it++);
					if(m_mapBullets.empty())
						break;
				}
				else
				{
					it->second->Update(fDelta);
					it++;
				}

			}
		}

	}

	

}

void CHeroPlayer::Signal(std::string strSignal, void *pParam)
{

}

bool CHeroPlayer::OnEvent(const irr::SEvent &event)
{
	switch(event.EventType)
	{
	case irr::EET_GUI_EVENT:
		{

		}
		break;
	case irr::EET_KEY_INPUT_EVENT:
		{
			if(event.KeyInput.PressedDown)
			{
				m_Key[event.KeyInput.Key] = true;
			}
			else
			{
				m_Key[event.KeyInput.Key] = false;
			}
		}
		break;
	case irr::EET_MOUSE_INPUT_EVENT:
		{
			if(event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
			{
				m_Key[irr::KEY_LBUTTON] = true;
			}
			else if(event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP)
			{
				m_Key[irr::KEY_LBUTTON] = false;
			}

		}
		break;
	case irr::EET_USER_EVENT:
		break;
	}
	return false;
}