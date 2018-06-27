#pragma once
#ifndef  __STARTSCENE_H__
#define    __STARTSCENE_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"  
USING_NS_CC;
using namespace ui;


class StartScene :public cocos2d::Scene
{
public:
	StartScene();
	~StartScene();
	static cocos2d::Scene* createScene();
	virtual bool init();

	EditBox *NeditBox;
	CREATE_FUNC(StartScene);


};

#endif // ! __STARTSCENE_H__