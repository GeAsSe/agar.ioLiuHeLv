#pragma once
#ifndef _GameLayer_H_
#define _GameLayer_H_

#include "cocos2d.h"
/*#include "Header\Common.h"*/
#include "ui\CocosGUI.h"
#include<vector>
#include"proj.win32\client.h"
#include"Classes\Web\Singleton.h"


USING_NS_CC;
using namespace ui;

class Joystick;
class Player;
class Food;
class Spore;
class Prick;

class GameLayer : public Layer {
public:
	GameLayer();
	~GameLayer();

	virtual bool init();

	CREATE_FUNC(GameLayer);

	virtual void onExit();

	virtual void update(float dt);		//游戏中每帧更新

	void start();
	//void initData(int seed,);			//初始化地图数据
	void initRival(std::vector<int>playerIDs,
	int rivalNum, std::vector<std::string>playernames, std::vector<int> KeywordIDs,
	std::vector<int>divisionNum, std::vector<double>positions, std::vector<int>scores);		//初始化对手
	void initPlayer(int ID, std::string name, int KeywordID, int divisionNum, std::vector<double>positions, std::vector<int>score);		//初始化玩家
	void initFood(int seed);						//初始化豆子
	void initSpore(int sporeNum, std::vector<int>globalIDs, std::vector<double> position);		//初始化孢子
	void initPrick(int prickNum, std::vector<int> globalIDs, std::vector<double>position);		//初始化绿刺

	void resetpoller(float dt);
	/*单机调试函数*/
	void initDataDefault();
	void initRival();
	void initPlayer();
	void initFood();

	void updateView();		//更新游戏视图
	void updateFood();		//更新食物
	void updateSpore();		//更新孢子
	void updatePrick();		//更新绿刺
	void updateRival();		//更新对手

	void updateRank(float dt);		//更新排行榜信息
	void updateScore(float dt);		//更新分数

	/*单机调试函数*/
	void startAddPrick(float dt);//单机刷刺
	void addPrick(float dt);//单机加刺

	void collideFood(Player * player);		//与豆子的碰撞检测
	void collide();			//碰撞检测

	void spitSpore();		//吐孢子操作
	void dividePlayer();		//分身操作
	void resetFood(Node * node);		//重置豆子
	/*将玩家信息上传*/
	/*void synPlayerMove();*/		//同步玩家移动
	void synPlayerInfo();		//同步玩家信息
	void synSporeInfo();		//同步孢子
    /*处理服务器返回消息*/
	void playerMoveEvent(int playerID,std::vector<double>position);
	void playerDivideEvent(int playerID);
	void spitSporeResultEvent(int playerID, int globalID, int sporeCount);
	void addPrickEvent(int globalID, int x, int y);
	void enterPlayerEvent(int playerID, std::string playername, int KeywordID, int divisionNum, std::vector<double>position, std::vector<int>scores);
	void updatePlayerEvent(int playerID, int divisionNum, std::vector<double>position, std::vector<int> scores);
private:
	Node * _map;				//地图
	Player * _player;			//玩家
	Map<int, Player *> _rivalMap;		//对手列表
	Vector<Food *> _foodList;			//豆子列表
	Map<int, Prick *> _prickMap;			//绿刺
	Map<int, Spore *> _sporeMap;			//孢子
	std::vector<int> _vecSporeNeedUpdate;			//需要更新的孢子
	float _mapScale;			//地图缩放因子
	int _ID;                  //玩家ID；

};

#endif