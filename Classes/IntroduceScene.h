#pragma once
#ifndef __INTRODUCE_SCENE_H
#define __INTRODUCE_SCENE_H

#include"cocos2d.h"



class IntroduceScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void MenuButtonBackToEnterScene(cocos2d::Ref*pSender);
};


#endif __INTRODUCE_SCENE_H