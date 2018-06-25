#pragma once
#ifndef _DataLayer_H_
#define _DataLayer_H_

#include "cocos2d.h"
#include "ui\CocosGUI.h"

USING_NS_CC;
using namespace ui;

/*数据图层，负责更新和显示玩家游戏时的数据*/

class DataLayer : public Layer {
public:
	DataLayer();
	~DataLayer();

	virtual bool init();

	CREATE_FUNC(DataLayer);

	virtual void onExit();//返回；

	void updateTime(EventCustom * event);//刷新游戏时间‘
	void updateScore(EventCustom * event);//刷新游戏得分
	void updateRank(EventCustom * event);//刷新游戏排名；

	void menuRankCallback(Ref * pSender);
private:
	Label * _timeLabel;
	Label * _scoreLabel;
	Vector<Label *> _rankList;
	Sprite * _rankSprite;
	Scale9Sprite * _rankBk1;
	Scale9Sprite * _rankBk2;
	Scale9Sprite * _rankBk3;
	int _timeTotal;
};

#endif