#include <iostream>
#include <string>

#include <irrlicht.h>
#include "CSAApp.h"

/*

���� : �����ð���  FPS ���� �����ӿ�
���� : 2009. 8�� 30��( v1.0 )

�ʼ� ���� ��� 

�ϸ���Ʈ���� 1.5.1
��ġ : sdk/irrlicht

������ 2.5 rc7
��ġ : sdk/bullet

�α�:
2009.8.30 :  v 1.0 ��������
*/

void main()
{
	static CSAApp App;

	App.Init();
	App.gameloop();
}