#include <iostream>
#include <string>

#include <irrlicht.h>
#include "cdmkapp.h"

/*

제목 : 프로토타입 또는 데마크식 게임프레임웍
버전 : 2009. 8월 30일( v1.0 )

필수 공유 모듈 

일리히트엔진 1.5.1
위치 : sdk/irrlicht

블릿엔진 2.5 rc7
위치 : sdk/bullet

로그:
2009.8.30 :  v 1.0 최종수정
*/

void main()
{
	static CdmkApp App;

	App.Init();
	App.gameloop();
}