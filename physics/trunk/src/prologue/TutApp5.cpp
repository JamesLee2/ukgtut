//#include "StdAfx.h"

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <comutil.h>


#include <irrlicht.h>
#include <windows.h>
#include <string>

#include <d3d9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 


#include "Singleton.h"
#include "irrbaseApp.h"

#include "TutApp5.h"

#include "CFollowAnimator.h"
#include "CBulletAnimatorManager.h"
#include "CBulletWorldAnimator.h"

//#pragma comment(lib,"irrlicht.lib")
//#pragma comment(lib,"comsuppw.lib")



CTutApp5::CTutApp5(void)
{
}

CTutApp5::~CTutApp5(void)
{
}

bool CTutApp5::OnEvent(const irr::SEvent& event)
{
	
	
	if(event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		if(event.KeyInput.PressedDown)
		{
			irr::scene::ISceneManager *pSmgr = m_pDevice->getSceneManager();
			irr::video::IVideoDriver *pVideo = m_pDevice->getVideoDriver();

			{
				irr::scene::CBulletObjectAnimatorGeometry geom;
				irr::scene::CBulletObjectAnimatorParams physicsParams;

				irr::scene::ISceneNode *pNode = pSmgr->addCubeSceneNode(5.f);
				pNode->setPosition(irr::core::vector3df(3,20,0));
				pNode->setMaterialTexture(0,pVideo->getTexture("t351sml.jpg"));

				geom.type = irr::scene::CBPAGT_BOX;
				geom.box.X = 2.5f;//.sphere.radius = radius;
				geom.box.Y = 2.5f;
				geom.box.Z = 2.5f;

				physicsParams.mass = .5f;
				physicsParams.gravity = core::vector3df(0, 0, 0);
				physicsParams.friction = 0.6f;

				irr::scene::CBulletObjectAnimator *pAnim = m_pBulletPhysicsFactory->createBulletObjectAnimator(
					pSmgr,
					pNode,
					m_pWorldAnimator->getID(),
					&geom,
					&physicsParams
					);
				pNode->addAnimator(pAnim);	
				pAnim->drop();
			}

			
		}
	}
	return false;
}

int CTutApp5::Init(LPCWSTR lpclassName)
{
	irr::IrrlichtDevice *pDevice = irr::createDevice(					
			irr::video::EDT_DIRECT3D9,
			irr::core::dimension2du(640,480), 32,
			false, true, false,
			this
			);
	m_pDevice = pDevice;		

	irr::scene::ISceneManager *pSmgr = m_pDevice->getSceneManager();
	irr::video::IVideoDriver *pVideo = m_pDevice->getVideoDriver();

	m_pDevice->getFileSystem()->changeWorkingDirectoryTo("../res/irr_exam");

	{
		irr::scene::ICameraSceneNode *pCam = pSmgr->addCameraSceneNode(0,
			irr::core::vector3df(0,20,-50),irr::core::vector3df(0,0,0));		
	}

	// Initialize bullet physics
	m_pBulletPhysicsFactory = new scene::CBulletAnimatorManager(pSmgr);

	m_pWorldNode = pSmgr->addEmptySceneNode();
	m_pWorldNode->setName("WorldNode");
	{
		irr::scene::CBulletWorldAnimator::Params worldParams;
		

		//��Ʈ�ִϸ�����
		//irr::scene::CBulletWorldAnimator* bulletWorldAnimator = 
		m_pWorldAnimator =
			m_pBulletPhysicsFactory->createBulletWorldAnimator(
			pSmgr,
			m_pWorldNode,
			&worldParams
			);
		m_pWorldAnimator->setGravity(irr::core::vector3df(0,-5,0));
		m_pWorldNode->addAnimator(m_pWorldAnimator);
		//bulletWorldAnimator->drop();	

		

		//����Ʈ �߰�
		pSmgr->addLightSceneNode(0,irr::core::vector3df(20,20,0));
		pSmgr->addLightSceneNode(0,irr::core::vector3df(20,-20,0));

		{
			irr::scene::CBulletObjectAnimatorGeometry geom;
			irr::scene::CBulletObjectAnimatorParams physicsParams;

			irr::scene::ISceneNode *pNode = pSmgr->addCubeSceneNode(5.f);
			pNode->setPosition(irr::core::vector3df(3,20,0));
			pNode->setMaterialTexture(0,pVideo->getTexture("t351sml.jpg"));

			geom.type = irr::scene::CBPAGT_BOX;
			geom.box.X = 2.5f;//.sphere.radius = radius;
			geom.box.Y = 2.5f;
			geom.box.Z = 2.5f;

			physicsParams.mass = .5f;
			physicsParams.gravity = core::vector3df(0, 0, 0);
			physicsParams.friction = 0.6f;

			irr::scene::CBulletObjectAnimator *pAnim = m_pBulletPhysicsFactory->createBulletObjectAnimator(
				pSmgr,
				pNode,
				m_pWorldAnimator->getID(),
				&geom,
				&physicsParams
				);
			pNode->addAnimator(pAnim);	
			pAnim->drop();
		}

		{
			irr::scene::CBulletObjectAnimatorGeometry geom;
			irr::scene::CBulletObjectAnimatorParams physicsParams;

			irr::scene::IMeshSceneNode *pNode = pSmgr->addCubeSceneNode(5.f);
			
			pNode->setMaterialTexture(0,pVideo->getTexture("t351sml.jpg"));

			geom.type = irr::scene::CBPAGT_BOX;
			geom.box.X = 2.5f;//.sphere.radius = radius;
			geom.box.Y = 2.5f;
			geom.box.Z = 2.5f;

			physicsParams.mass = 0.0f;
			physicsParams.gravity = core::vector3df(0, 0, 0);
			physicsParams.friction = 0.6f;

			irr::scene::CBulletObjectAnimator *pAnim = m_pBulletPhysicsFactory->createBulletObjectAnimator(
				pSmgr,
				pNode,
				m_pWorldAnimator->getID(),
				&geom,
				&physicsParams
				);
			pNode->addAnimator(pAnim);	

			pAnim->drop();
		}

		{			
			irr::scene::CBulletObjectAnimatorGeometry geom;
			irr::scene::CBulletObjectAnimatorParams physicsParams;

			irr::scene::ISceneNode* pNode = 0;
			irr::scene::IAnimatedMesh* pMesh;			

			pMesh = pSmgr->addHillPlaneMesh("myHill",
				irr::core::dimension2d<irr::f32>(40,40),
				irr::core::dimension2d<irr::u32>(10,10), 
				0,0,
				irr::core::dimension2d<irr::f32>(0,0),
				irr::core::dimension2d<irr::f32>(10,10));

			pNode = pSmgr->addAnimatedMeshSceneNode(pMesh);
			pNode->setPosition(irr::core::vector3df(0,-20,0));
			pNode->setMaterialTexture(0, pVideo->getTexture("wall.jpg"));	

			// add level static mesh
			geom.type = scene::CBPAGT_CONCAVE_MESH;
			//geom.type = scene::CBPAGT_CONCAVE_GIMPACT_MESH;
			geom.mesh.irrMesh = pMesh->getMesh(0);
			geom.mesh.irrMesh = NULL;
			geom.meshFile = "myHill";//maps/20kdm2.bsp";

			physicsParams.mass = 0.0f;

			scene::CBulletObjectAnimator* levelAnim = 
				m_pBulletPhysicsFactory->createBulletObjectAnimator(
				pSmgr,
				pNode,
				m_pWorldAnimator->getID(),
				&geom,
				&physicsParams
				);
			pNode->addAnimator(levelAnim);
			levelAnim->drop();
		}
	}

	return 0;
}

bool CTutApp5::Apply(float fElapsedTime)
{
	static irr::u32 uLastTick=0;
	irr::u32 uTick = m_pDevice->getTimer()->getTime();						
	irr::f32 fDelta = ((float)(uTick - uLastTick) * 0.0001f); //��Ÿ�� ���ϱ�
	uLastTick = uTick;

	//�ִϸ����ͳ� ĳ���� �ִϸ��̼��� ��Ű���� tick�� ȣ���ؾ���
	m_pDevice->getTimer()->tick();

	{
		//irr::u32 dt = (irr::u32)(fDelta * 1000000.f); //�鸸���� ���ʴ����� ƽ��
		irr::u32 dt = 1666; //�� 1/60�� ���� ���� �鸸���� ���� ��Ȯ��(1000000�� ������)
		m_pBulletPhysicsFactory->OnUpdate(dt);
	}
	//m_pBulletPhysicsFactory->OnUpdate(fDelta);

	return true;
}

void CTutApp5::Render(float fElapsedTime)
{
	irr::video::IVideoDriver *pVideo = m_pDevice->getVideoDriver();

	if(pVideo->beginScene(true, true, irr::video::SColor(255,100,101,140)) )
	{	
		irr::scene::ISceneManager *pSmgr = m_pDevice->getSceneManager();

		pSmgr->drawAll();		
		
		pVideo->endScene();
	}	
}
