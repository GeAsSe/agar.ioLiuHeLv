#pragma once
#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"  
USING_NS_CC;
using namespace ui;
#define MAX_COUNT 5

class GameScene :public cocos2d::Layer
{
	cocos2d::Vector<cocos2d::Label*> Labels;
public:
	GameScene();
	~GameScene();
	
	static cocos2d::Scene* createScene();
	virtual bool init();
	
	Label *newsLabel;
	Label *newsLabe2;
	Label *newsLabe3;
	Label *newsLabe4;
	Label *newsLabe5;
	virtual void onExit();
	

    EditBox *editBox ;
	
    void Key_rect();
	void pushmessage(std::string s);
	CREATE_FUNC(GameScene);
private:
	std::string content1;
	std::string	content2;
	std::string content3;
	std::string content4;
	std::string content5;
};

#endif // !