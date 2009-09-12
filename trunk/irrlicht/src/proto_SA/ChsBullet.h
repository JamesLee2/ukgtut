#pragma once
#include "FSMObject.h"
//ź ������Ʈ
class ChsBullet : public IFSMObject
{
public:

	struct Params
	{
		irr::core::vector3df vInitPos; // ������
		irr::f32 Energy;		//ź�� ������
		irr::core::vector3df vAim; //�ѱ� ����
		irr::f32 mass;//����		
	};

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
	irr::scene::CBulletObjectAnimator *m_pAnim;

	//! ���ϰ� 0: ���θ� ����, 1: ����
	virtual int Init(void *pParam);

	inline virtual irr::core::vector3df getPosition() {return m_pNode->getPosition();}
	inline virtual bool isDie() {if(GetStatus() == FSM_DIE)return true;else return false;}

	virtual void Signal(std::string strSignal,void *pParam);// {}	//���º�ȯ����ó���Ѵ�.
	virtual void Update(irr::f32 fTick); //���ӿ�����Ʈ����ó���Ѵ�.
	
};
