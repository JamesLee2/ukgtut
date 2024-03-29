#pragma once



class IFSMObject
{
private:
	irr::s32 m_nStatus;
	irr::s32 m_nStep;	
	
public:
	IFSMObject(void)
		:m_nStatus(0),m_nStep(0)
	{	
	}

	virtual ~IFSMObject(void)
	{
	}

	std::string m_strTypeName; //객체 타입이름

	inline void SetStatus(irr::s32 status)
	{
		m_nStep = 0; //스탭초기화
		m_nStatus = status;
	}
	inline void SetStep(irr::s32 step)
	{
		m_nStep = step;
	}

	inline irr::s32 GetStep()
	{
		return m_nStep;
	}

	inline irr::s32 GetStatus()
	{
		return m_nStatus;
	}

	inline void IncStep()
	{
		m_nStep++;
	}
	inline void DecStep()
	{
		m_nStep--;
	}	

	inline virtual irr::core::vector3df getPosition() {return irr::core::vector3df(0,0,0);}
	inline virtual bool isDie() {return true;}

	virtual void Signal(std::string strSignal,void *pParam) = 0;	//상태변환등을처리한다.
	virtual void Update(irr::f32 fTick) =0; //게임오브잭트로직처리한다.
	
	
};

typedef std::tr1::shared_ptr<IFSMObject> SP_IFSMObject;
typedef IFSMObject* LP_IFSMObject;
typedef IFSMObject** LPLP_IFSMObject;
