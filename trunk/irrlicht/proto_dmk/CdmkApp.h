#pragma once

#include <irrlicht.h>
#include <crtdbg.h>
#include <windows.h>

#include "../sdk/ggfsdk/Singleton.h"
//#include "../sdk/ggfsdk/util.h"


#include "../irredit/source/plugins/jz3dplugins/JZ3DSceneNodeFactory.h"

#include "../irredit/source/plugins/jz3dplugins/CollusionMngPluginNode.h"
#include "../irredit/source/plugins/jz3dplugins/FormatedAnimationNode.h"




class CdmkApp :  
	public irr::IEventReceiver,
	public ggf::oop::Singleton<CdmkApp>
{
public:
	CdmkApp(void);
	~CdmkApp(void);

	irr::IrrlichtDevice *m_pDevice;
	irr::video::IVideoDriver *m_pVideo;
	irr::scene::ISceneManager *m_pSmgr;
	irr::gui::IGUIEnvironment *m_pGuiEnv;


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
