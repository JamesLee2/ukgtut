#pragma once


namespace ggf
{
	namespace object
	{

struct S_SceneElement
{
	std::string m_strName;
	std::string m_strFileName;
};

struct S_ActionElement
{
	std::string m_strName;
	int m_Start;
	int m_End;
	int m_Tick;
};

struct S_ObjectElement
{
	std::string m_strName;
	std::string m_strSceneName;
	std::map<std::string,std::tr1::shared_ptr<S_ActionElement> > m_mapAction;
};



class CObjectMngV1
{
public:
	CObjectMngV1(void);
	virtual ~CObjectMngV1(void);

	std::map<std::string,std::tr1::shared_ptr<S_SceneElement> > m_mapScene;
	std::map<std::string,std::tr1::shared_ptr<S_ObjectElement> > m_mapObject;	
	
	static const char *OBJECT_NODENAME;// = "ObjectRoot";
	static const char *CHRAROOT_NODENAME;

	bool Load(char *fn);

	int getCharacterList(irr::scene::ISceneManager *pSmgr,std::string strObjName,irr::core::list<irr::scene::ISceneNode *> &chr_list);
	irr::scene::IAnimatedMeshSceneNode* getCharacterNodeByID(irr::scene::ISceneManager *pSmgr,std::string strObjName,int id);
	

	//����� �Լ���
	//�������÷���
	void Animation_Play(irr::scene::ISceneManager *pSmgr,std::string strObjName,std::string strActName,bool bLoop = true,irr::scene::IAnimationEndCallBack *pEndCallBack=NULL);
	
	//�����Ӻ��η� ����ȭ�麸�� ��� 
	//lerp 0~1���̰� 0�� ���� 1��
	void Animation_Snap(irr::scene::ISceneManager *pSmgr,std::string strObjName,std::string strActName,irr::f32 lerp); 

	//��ĭ�������̱�
	//bPrv�� ���̸� �ڷ� �����̸� ������...
	void Animation_Tick(irr::scene::ISceneManager *pSmgr,std::string strObjName,std::string strActName,bool bPrv); 
	

	//���� ���� �ִϸ��̼�
	void Animation_TotalPlayer(irr::scene::ISceneManager *pSmgr,std::string strObjName,irr::s32 start,irr::s32 end); 
	void Animation_TotalSnap(irr::scene::ISceneManager *pSmgr,std::string strObjName,irr::f32 frame); 

	//0���������� �������·μ���
	void Animation_Reset(irr::scene::ISceneManager *pSmgr,std::string strObjName);

	//�ִϸ��̼� ĳ���ͳ����
	irr::scene::IAnimatedMeshSceneNode *Char_GetNode(irr::scene::ISceneManager *pSmgr,std::string strObjName,irr::s32 id);
	//�̸��� strObjName�� ������Ʈ ��Ʈ�����
	irr::scene::ISceneNode *Root_GetNode(irr::scene::ISceneManager *pSmgr,std::string strObjName);

	//������Ʈ�鸦 �����Ʈ���� ������Ų��.
	irr::scene::ISceneNode *ConstructSceneNode(irr::scene::ISceneManager *pSmgr);

	//������Ʈ����
	//strOriObjName �����̸�
	//strCloneObjName ���纻�̸�
	irr::scene::ISceneNode *Clone(irr::scene::ISceneManager *pSmgr,std::string strOriObjName,std::string strCloneObjName);

};


	}
}
