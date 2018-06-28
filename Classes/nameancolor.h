#pragma once
#include"cocos2d.h"
#include"Classes/Web/Singleton.h"
#include"Classes\GameScene\GameLayer.h"
#include"Classes\GameScene\GameScene.h"
class nameandcolor {
	DECLARE_SINGLETON(nameandcolor)

	
public:
	int color;
	std::string name;
	GameLayer * layer;
	std::string message;
	GameScene *mess;
	void set() {
		layer = GameLayer::create();
	}
};