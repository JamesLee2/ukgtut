#pragma once

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
};
