#include <iostream>
#include <string>

#include <irrlicht.h>
#include "cdmkapp.h"

/*

�ʼ� ���� ��� 

�ϸ���Ʈ���� 1.5.1
��ġ : sdk/irrlicht

������ 2.5 rc7
��ġ : sdk/bullet


*/

void main()
{
	static CdmkApp App;

	App.Init();
	App.gameloop();
}