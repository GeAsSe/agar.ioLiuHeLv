#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"
#include"client.h"
#include"proj.win32\nameancolor.h"
#include"Classes\GameScene\GameScene.h"
USING_NS_CC;




int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	nameandcolor myself = CSingleton<nameandcolor>::GetInstance();

	CClient *pClient = CSingleton<CClient>::GetInstancePtr();
	pClient->ClientInit();
	
    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
