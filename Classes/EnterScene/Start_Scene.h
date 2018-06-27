#pragma once
#ifndef  __START_SCENE_H__
#define    __START_SCENE_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"  
USING_NS_CC;
using namespace ui;


class Start_Scene :public cocos2d::Scene
{
public:
	Start_Scene();
	~Start_Scene();
	static cocos2d::Scene* createScene();
	virtual bool init();

	void put1(cocos2d::Ref*pSender);
	void put2(cocos2d::Ref*pSender);
	void put3(cocos2d::Ref*pSender);
	void put4(cocos2d::Ref*pSender);
	CREATE_FUNC(Start_Scene);
private:


};

#endif // ! __START_SCENE_H__