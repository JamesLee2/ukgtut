#pragma once
#include "FSMObject.h"
//ź ������Ʈ
class ChsBullet : public IFSMObject
{
public:

	enum {
		FSM_READY,
		FSM_MOVE,
		FSM_HIT,
		FSM_DIE,		
		FSM_COUNT
	};

	ChsBullet(void);
	virtual ~ChsBullet(void);

	irr::scene::ISceneNode *m_pNode;
	irr::scene::ISceneNodeAnimator *m_pAnim;

	int Init();
};
