#pragma once
#ifndef __END_SCENE_H
#define __END_SCENE_H

#include"cocos2d.h"
class EndScene: public cocos2d::Layer
{
public:
	
	
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	//creat the scene
	virtual bool init();
	//initialization
	void MenuButtonRetry(cocos2d::Ref*pSender);
	void MenuButtonExit(cocos2d::Ref*pSender);
	

	CREATE_FUNC(EndScene);
};

#endif __END_SCENE_H