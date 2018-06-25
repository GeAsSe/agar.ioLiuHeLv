#pragma once
#ifndef  __HELPSCENE_H__
#define    __HELPSCENE_H__
#include "cocos2d.h"


class HelpScene :public cocos2d::Scene
{
public:
	HelpScene();
	~HelpScene();
	static cocos2d::Scene* createScene();
	virtual bool init();
	void BackContaoller(cocos2d::Ref*pSender);
	CREATE_FUNC(HelpScene);
};

#endif // ! __HELPSCENE_H__