#ifndef _GGF_FILESYSTEM
#define _GGF_FILESYSTEM

enum GGFFSComptype {
	GGFFS_COMP_NO = 0, //������
	GGFFS_COMP_RLE, //����������
	GGFFS_COMP_ZIP,  //zip�������
	GGFFS_COMP_USER
};

enum GGFFS_Attr {
	GGFFS_ATTR_FILE=0, //����
	GGFFS_ATTR_FOLDER, //����
	GGFFS_ATTR_ROOT,   //��Ʈ
	GGFFS_ATTR_IODEVICE //�������ġ
	
};

//��������
typedef struct _S_GGFFS_FileInfo
{	
	DWORD m_nName;	       //�̸�
	DWORD m_nAttr;	       //���ϼӼ�,����,����,��Ʈ,�������ġ...
	DWORD m_nCompressType; //��������
	DWORD m_nPos;          //��ġ
	DWORD m_nSize;         //ũ��

}S_GGFFS_FileInfo, *PS_GGFFS_FileInfo;

typedef struct _S_GGFFS_FileInfo_Rec
{
	S_GGFFS_FileInfo m_fi;
	unsigned char m_rev[64 - sizeof(S_GGFFS_FileInfo)];

}S_GGFFS_FileInfo_Rec,*PS_GGFFS_FileInfo_Rec;

typedef struct _S_GGFFS_HEADER
{
	BYTE  m_szId[4];
	DWORD m_nFileNameSize;
	DWORD m_nFileNum;

}S_GGFFS_HEADER,*PS_GGFFS_HEADER;

typedef struct _S_GGFFS_HEADER_Rec
{
	S_GGFFS_HEADER m_Data;
	BYTE m_rev[256 - sizeof(S_GGFFS_HEADER)];

}S_GGFFS_HEADER_Rec,*PS_GGFFS_HEADER_Rec;

typedef struct _S_GGFFS
{
	S_GGFFS_HEADER_Rec m_Header;

	char *m_pszNameTable;
	PS_GGFFS_FileInfo_Rec m_pFAT;

}S_GGFFS,PS_GGFFS;

typedef struct _S_GGFFS_OBJ
{
	DWORD m_hHandle; //�����ڵ� ���ü�������� ���������� ����ϵ����Ѵ�.�ܺ������� ��������ʵ����Ѵ�.
	S_GGFFS m_FS;

}S_GGFFS_OBJ,*PS_GGFFS_OBJ;

#ifdef __cplusplus
extern "C" {
#endif

/*
��Ű�������� ����.
*/
DWORD GGFFS_Open(const char *szPackFn);

/*
�����̿��µȻ��¿���������
��ĳ������ �־��� �����̸��� �ִ°����� ���������͸��ű��.
*/
int GGFFS_SelectFile(const char *szFn);

/*
��ī�̺� ���Ͻ�������Ѵ�
*/
DWORD GGFFS_GetFileSize(const char *szFn);

/*
��Ű���������ݴ´�.
*/
int GGFFS_Close();

/*
	�����������̺���ġ�ε������޾Ƽ� �̸��� ������ִ� ���۸���ȯ�Ѵ�.
*/
char *GGFFS_ID2NameTablePtr(int nID);


/*
	�̸��� �޾Ƽ� �����������̺���ġ�ε�����...
*/
DWORD GGFFS_Name2ID(char *szName);


/*
�� ��Ű�� ���� �ý����� �����Ѵ�.
*/
DWORD GGFFS_Create(char *szID,DWORD nMaxFileName,DWORD nFileNum);

/*
���Ͻý��� ��ü�����Ѵ�.
*/
PS_GGFFS_OBJ	GGFFS_GetFileSystemPtr();

/*
���ϳ����� nLen ��ŭ ���ۿ� �־���ش�. ���۴� �Լ�ȣ������ �Ҵ�Ǿ���Ѵ�.
*/
void			GGFFS_Read(char *szFName,DWORD nLen,BYTE *pBuf);

/*
���Ͻý��۱������� ����� �����Ͽ� �־��������� ��ġ�� �������� ���Ѵ�.
seek�Լ��� ���� ��밡���ϴ�.
�����ϸ� 0
*/
DWORD			GGFFS_GetOffset(char *szFName);

/*
��Ŀ������ ����κн�������Ѵ�.
*/
DWORD GGFFS_GetFATSize();


#ifdef __cplusplus
}
#endif


//���� ���� ������Լ�

#define GGF_MAXFILE 5

enum GGF_FileError {
	GGFFERROR_OPEN = GGF_MAXFILE,
	GGFFERROR_ETC
};

#ifdef __cplusplus
extern "C" {
#endif

	enum GGFF_ACCESS_MODE {
		GGFFMODE_READ = 0,
		GGFFMODE_WRITE
	};	

	DWORD	GGFFile_Read(DWORD hHandle,BYTE *pBuf,DWORD nLen);
	DWORD	GGFFile_Write(DWORD hHandle,BYTE *pBuf,DWORD nLen);	
	DWORD	GGFFile_SeekSet(DWORD hHandle,int nPos);
	DWORD	GGFFile_SeekCur(DWORD hHandle,int nPos);
	DWORD	GGFFile_Close(DWORD hHandle);
	DWORD	GGFFile_GetFileSize(DWORD hHandle);
	/*
	�����̸� GGFFERROR_OPEN ������ ������ȯ�Ѵ�.
	���� �˻�� 
	if(GGFFile_Open(...) >= GGFFERROR_OPEN)
	{
	//����ó��
	}
	*/
	DWORD	GGFFile_Open(const char *szFile,DWORD nMode);
	void	GGFFile_Init();	

	/*
	0 �̸� no error
	�ƴϸ� ����
	*/
	int GGFFile_GetError(DWORD hHandle);
	
	
	void *GGFFile_GetSystemHandle(DWORD nHandle);


#ifdef __cplusplus
}
#endif



#endif

