
#include "bycle.h"

CBycleApp theApp;

void main()
{
	
	//std::cout << "Loading [" << argv[1] << "] Bme Application" << std::endl;
	//switch(CBycleApp::Get().Create(argv[1]))// pApp->Init())
	switch(CBycleApp::Get().Create("../../res/proto_carace"))
	{	
	case 0:

		CBycleApp::Get().MainLoop();			
		break;
	case 1:
		//TRACE("system.xml �Ľ̿���\n");
		break;
	case 2:				
		//TRACE("����̽����� \n");
		break;
	case 3:
		//TRACE("��Ʈ�� ���� \n");
		break;
	default:
		break;
	}
}
