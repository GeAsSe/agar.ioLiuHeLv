#pragma once
#ifndef __ENTER_SCENE_H
#define __ENTER_SCENE_H


#include"cocos2d.h"

class EnterScene: public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	//creat the scene
	virtual bool init();
	//initialization
	void MenuButtonBegin(cocos2d::Ref*pSender);
	void MenuButtonIntroduce(cocos2d::Ref*pSender);
	void MenuButtonOut(cocos2d::Ref*pSender);

	CREATE_FUNC(EnterScene);
};



#endif __ENTER_SCENE_H