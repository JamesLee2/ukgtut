
#include "CSAApp.h"

#include "HeroPlayer.h"
#include "aiPlayer.h"


void recursivePhysicsReg(irr::scene::ISceneManager *pSmgr,irr::s32 worldID,irr::scene::ISceneNode *pStartNode)
{
	if(pStartNode == 0)
	{
		pStartNode = pSmgr->getRootSceneNode();
	}

	irr::core::list<irr::scene::ISceneNode *>::ConstIterator it = pStartNode->getChildren().begin();

	for(;it != pStartNode->getChildren().end();it++)
	{
		recursivePhysicsReg(pSmgr,worldID,*it);


		switch( (*it)->getType())
		{
		case irr::scene::jz3d::CCollusionMngNode::TypeID:
			{				
			}
			break;
		case irr::scene::jz3d::CBulletPhysicsObjectNode::TypeID:
			{
				irr::scene::CBulletAnimatorManager *ani_factory =
					static_cast<irr::scene::CBulletAnimatorManager *>(ggf::util::findAnimatorFactory(pSmgr,
					irr::scene::ESNAT_BULLET_OBJECT));

				irr::scene::jz3d::CBulletPhysicsObjectNode *pNode = irr::scene::jz3d::CBulletPhysicsObjectNode::upcast((*it));	
				pNode->register2BulletPhysicsWorld(ani_factory->getBulletWorldByID(worldID)->getWorld());
			}
			break;
		case irr::scene::ESNT_MESH:
		case irr::scene::jz3d::CTiledPlaneNode::TypeID:			
			break;
		}

		
	}
}



CSAApp::CSAApp(void)
:m_pDevice(0),
m_pVideo(0),
m_pSmgr(0),
m_pGuiEnv(0)	
{
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //�޸𸮸� Ž��
	//�޸𸮸� �߻����� Ž��.
	//_CrtSetBreakAlloc(10930);
}


CSAApp::~CSAApp(void)
{
	m_pDevice->drop();

	//�߰������� �����ÿ���ġ�� �������� �������־���Ѵ�.
	m_pWorldAnimator->drop();
	m_pBulletPhysicsFactory->drop();	
}

bool CSAApp::Init()
{
	m_pDevice = irr::createDevice(
		irr::video::EDT_DIRECT3D9,
		//irr::core::dimension2du(640,480), //1.6������
		irr::core::dimension2di(640,480), //1.5������
		32,
		false, false, false,
		this
		);
	m_pVideo = m_pDevice->getVideoDriver();
	m_pSmgr = m_pDevice->getSceneManager();
	m_pGuiEnv = m_pDevice->getGUIEnvironment();	

	//����� �߰� Ȯ�� ���丮���
	{
		irr::scene::jz3d::CJZ3DSceneNodeFactory *factory =
			new irr::scene::jz3d::CJZ3DSceneNodeFactory(m_pSmgr);
		m_pSmgr->registerSceneNodeFactory(factory);
		factory->drop();	
	}

	//�����ִϸ����� ���丮���
	m_pBulletPhysicsFactory = new irr::scene::CBulletAnimatorManager(m_pSmgr);

	//���ҽ����� ����
	m_pDevice->getFileSystem()->changeWorkingDirectoryTo("../../res");

	//�ʱ�ȭ �ڵ� ����...	

	//������ε�
	m_pSmgr->loadScene("proto_SA/stage0.irr");	
	
	{	
		//���� �ִϸ����� �߰�
		irr::scene::ISceneNode *pNode = m_pSmgr->addEmptySceneNode();
		pNode->setName("usr/scene/physics/world/1");
		//  �⺻�߷� -9.8
		irr::scene::CBulletWorldAnimatorParams worldParams;		
		worldParams.bSoft = false;

		m_pWorldAnimator =
			m_pBulletPhysicsFactory->createBulletWorldAnimator(
			m_pSmgr,
			pNode,
			&worldParams
			);
		//�߷��� �⺻���� y ������ -9.8			
		pNode->addAnimator(m_pWorldAnimator);

		//�ı��ڿ��� ������ġ�� �������� �����ֱ����ؼ�.m_pWorldAnimator->drop() ���� �ʴ´�.		
	}

	{//��������ó���ʱ�ȭ
		irr::scene::ISceneNode *pNode;
		pNode = m_pSmgr->getSceneNodeFromName("usr/bld");
		recursivePhysicsReg(m_pSmgr,m_pWorldAnimator->getID(),pNode);				
	}

	//������Ʈ���ε�
	//m_pSmgr->loadScene("proto_sa/obj_ninja.irr");
	m_pSmgr->loadScene("proto_sa/obj_fpsbody1.irr");
	m_pSmgr->loadScene("proto_sa/obj_zombie.irr");

//	m_pSmgr->getRootSceneNode()->OnAnimate(0);

	irr::scene::ICameraSceneNode *pCam = m_pSmgr->addCameraSceneNode(0,irr::core::vector3df(0,58,-30),irr::core::vector3df(0,0,0));	

	//���ΰ�
	{
		CHeroPlayer *pHero = new CHeroPlayer();
		m_spHeroPlayer = SP_PLAYER(pHero);		

		irr::scene::ISceneNode *pNode = m_pSmgr->getSceneNodeFromName("usr/obj/fpsbody/1");

		if( pNode && pHero->Init(pNode) != true)
			return false;
	}

	{
		CAIPlayer *pPlayer = new CAIPlayer();		
		m_spZombie1 = SP_PLAYER(pPlayer);

		irr::scene::ISceneNode *pNode = m_pSmgr->getSceneNodeFromName("usr/obj/b3d/zombie/1");

		if( pNode && pPlayer->Init(pNode) != true)
		{
			return false;
		}
	}

	////����� ������Ʈ�߰�
	//{			
	//	irr::scene::CBulletObjectAnimatorGeometry geom;
	//	irr::scene::CBulletObjectAnimatorParams physicsParams;

	//	irr::scene::ISceneNode *pNode = m_pSmgr->addCubeSceneNode(10.f);
	//	pNode->setPosition(irr::core::vector3df(0,15,-2));
	//	//pNode->setMaterialTexture(0,pVideo->getTexture("../res/irr_exam/t351sml.jpg"));
	//	pNode->setMaterialFlag(irr::video::EMF_LIGHTING,false);
	//	pNode->setMaterialFlag(irr::video::EMF_WIREFRAME,true);

	//	geom.type = irr::scene::CBPAGT_BOX;
	//	geom.box.X = 5.f;//.sphere.radius = radius;
	//	geom.box.Y = 5.f;
	//	geom.box.Z = 5.f;			

	//	physicsParams.mass = 0.5f;
	//	physicsParams.gravity = core::vector3df(0, 0, 0);
	//	physicsParams.friction = .6f; //������		


	//	irr::scene::CBulletObjectAnimator *pAnim = m_pBulletPhysicsFactory->createBulletObjectAnimator(
	//		m_pSmgr,
	//		pNode,
	//		m_pWorldAnimator->getID(),
	//		&geom,
	//		&physicsParams
	//		);
	//	pNode->addAnimator(pAnim);	
	//	pAnim->drop();
	//}
	//

	

	//������ ����Ʈ ǥ�ÿ� ������
	irr::gui::IGUIStaticText *pstextFPS = 
		m_pGuiEnv->addStaticText(L"Frame rate",
		irr::core::rect<irr::s32>(0,0,100,20),true,true,0,100,true);

	//���׶��� ��ü ��������
	m_pSmgr->getRootSceneNode()->OnAnimate(0);

	return true;

}

bool CSAApp::OnEvent(const irr::SEvent &event)
{

	//�ִϸ����͵鿡�� �̹�Ʈ�� �����Ѵ�.
	if(m_pSmgr)
	{
		//ggf::utils::RecursiveEvent(m_pSmgr->getRootSceneNode(),event);
		
	}
	if(m_spHeroPlayer.get())
	{
		((CHeroPlayer *)m_spHeroPlayer.get())->OnEvent(event);
		
	}

	switch(event.EventType)
	{
	case irr::EET_GUI_EVENT:
		{

		}
		break;
	case irr::EET_KEY_INPUT_EVENT:
		{
		}
		break;
	case irr::EET_MOUSE_INPUT_EVENT:
		{

		}
		break;
	case irr::EET_USER_EVENT:
		break;
	}
	return false;
}

void CSAApp::Update()
{
	static irr::u32 uLastTick=0;
	irr::u32 uTick = m_pDevice->getTimer()->getTime();      
	irr::f32 fDelta = ((float)(uTick - uLastTick)) / 1000.f; //��Ÿ�� ���ϱ�
	uLastTick = uTick;

	//�����ӷ���Ʈ ����
	{
		wchar_t wszbuf[256];
		swprintf(wszbuf,L"Frame rate : %d\n TriAngle: %d",m_pVideo->getFPS(),
			m_pVideo->getPrimitiveCountDrawn());			
		m_pGuiEnv->getRootGUIElement()->getElementFromId(100)->setText(wszbuf);
	}

	//���ӷ��� & ���� & ���浹ó��
	{
		m_spHeroPlayer->Update(fDelta);
		m_spZombie1->Update(fDelta);
		//����ó��
		{
			irr::u32 dt = (irr::u32)(fDelta * 1000000.f); //�鸸���� ���ʴ����� ƽ��					
			m_pBulletPhysicsFactory->OnUpdate(dt);
		}
		
	}
}

void CSAApp::Render()
{
	m_pVideo->beginScene(true, true, irr::video::SColor(255,0,0,0));

	m_pSmgr->drawAll();
	m_pGuiEnv->drawAll();

	m_pVideo->endScene();
}