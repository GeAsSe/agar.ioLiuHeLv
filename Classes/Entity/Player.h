#pragma once
#ifndef _PLAYER_H
#define _PLAYER_H

#include "cocos2d.h"

USING_NS_CC;

class Prick;
class PlayerDivision;
class Food;
class Spore;

class Player :public Node {
public:

	enum class State
	{
		NORMAL, //正常状态
		DIVIDE,  //分身状态,130行
		SPIT,  //吐球状态
	};

	Player();
	~Player();


	//创建玩家并导入玩家基本信息；
	static Player * create(std::string & name,  int keywordID,/* Vec2 velocity,*/ Node * map);
	bool init(std::string & name, int keywordID, /*Vec2 velocity,*/ Node * map);
	//退出；
	virtual void onExit();

	//创建一个玩家分身；需要位置、速度、得分参数；
	PlayerDivision * createDivision(Vec2 position, int score);
	//计算玩家一次可以吐出几个孢子
	int countSpitSporeNum();		
	//吐孢子,需要Map及sporeMap传入和globalID;
	void spitSpore(Node * map, Map<int, Spore *> & sporeMap, int globalID);		
	//分身,并进入分身态；
	void dividePlayer();		
	
	bool collideFood(Food * food);		//与食物的碰撞检测
	bool collideSpore(Spore * spore);		//与孢子的碰撞检测
	bool collidePrick(Prick *prick);		//与绿刺的碰撞检测
	bool collideRival(Player *rival);		//与对手的碰撞检测,判断谁吃谁；
	int collideDivision(PlayerDivision * division);		//与对手分身的碰撞并返回结果值供collideRival判断谁吃谁；

	void updateDivision();		//玩家信息更新
	void GameOver();			//玩家复活

	void divideFinish();		//分身后回到Normal状态；

	void collidePrickFinish();	//碰到刺回到中合状态；

	

	Rect getPlayerRect();		//获取围住玩家所有分身的长方形；
	void setCombine(float dt);  //设置融合状态,即不在融合或者分裂状态；
	int getDivisionNum();	    //获取当前分身数量
	std::string getPlayerName();//设置玩家名称；
	int getTotalScore();        //得到玩家分身分数之和即游戏分数；
	Vector<PlayerDivision *> & getDivisionList();//获取分身列表；
	void clearDivisionList();
private:
	Vector<PlayerDivision *> _divisionList;		//玩家分身列表
	int _divisionNum;							//分身数量

	Node * _map;			//地图指针
	std::string _playerName;		//玩家名
	int _keywordID;			//关键字id

	int _color;			//颜色,没有关键字时可用

	State _state;		//玩家状态
	bool _combineEnable; //分身是否可以合体
};

#endif
