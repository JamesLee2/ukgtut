#pragma once

#include <irrlicht.h>
#include <crtdbg.h>
#include <windows.h>
#include <memory>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <iostream>

//bullet
#include "CFollowAnimator.h"
#include "CBulletAnimatorManager.h"
#include "CBulletWorldAnimator.h"
#include "btBulletDynamicsCommon.h"

#include "BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"
#include "BulletCollision/NarrowPhaseCollision/btPointCollector.h"
#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h"

#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"


//ggf
#include "Singleton.h"
#include "util.h"

//#include "FSMObject.h"
//#include "player.h"
//#include "HeroPlayer.h"

//pulgin node
#include "jz3dplugins/JZ3DSceneNodeFactory.h"
#include "jz3dplugins/CollusionMngPluginNode.h"
#include "jz3dplugins/FormatedAnimationNode.h"
#include "jz3dplugins/TiledPlaneNode.h"
#include "jz3dplugins/BulletPhysicsObjectNode.h"


//�����÷��̾���Ʈ�� �������̽�
#include "FSMObject.h"
#include "Player.h"

typedef std::tr1::shared_ptr<CPlayer> SP_PLAYER;



class CSAApp :  
	public irr::IEventReceiver,
	public ggf::oop::Singleton<CSAApp>
{
public:
	CSAApp(void);
	~CSAApp(void);

	irr::IrrlichtDevice *m_pDevice;
	irr::video::IVideoDriver *m_pVideo;
	irr::scene::ISceneManager *m_pSmgr;
	irr::gui::IGUIEnvironment *m_pGuiEnv;

	SP_PLAYER m_spHeroPlayer;//���ΰ�(��Ʈ�ѷ���)
	SP_PLAYER m_spZombie1; //��������Ʈ

	irr::scene::CBulletAnimatorManager* m_pBulletPhysicsFactory; 
	irr::scene::CBulletWorldAnimator* m_pWorldAnimator;


	//�̺�Ʈ �ڵ鷯
	bool OnEvent(const irr::SEvent& event);	

	bool Init();	
	void Update();
	void Render();

	inline void gameloop()
	{
		while(m_pDevice->run())
		{  
			Update();
			Render();			
		}
	}

};