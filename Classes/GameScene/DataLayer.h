#pragma once
#ifndef _DataLayer_H_
#define _DataLayer_H_

#include "cocos2d.h"
#include "ui\CocosGUI.h"

USING_NS_CC;
using namespace ui;

/*����ͼ�㣬������º���ʾ�����Ϸʱ������*/

class DataLayer : public Layer {
public:
	DataLayer();
	~DataLayer();

	virtual bool init();

	CREATE_FUNC(DataLayer);

	virtual void onExit();//���أ�

	void updateTime(EventCustom * event);//ˢ����Ϸʱ�䡮
	void updateScore(EventCustom * event);//ˢ����Ϸ�÷�
	void updateRank(EventCustom * event);//ˢ����Ϸ������

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