#pragma once
#ifndef _PLAYERDIVISION_H
#define _PLAYERDIVISION_H

#include"Entity.h"

class Food;
class Prick;
class Spore;

class PlayerDivision:public Entity
{
public:
	PlayerDivision();
	~PlayerDivision();

	//创建分身
	static PlayerDivision * create(const std::string& filename);
	//初始化分身
	bool init(const std::string& filename);
	//创建分身玩家分裂出的分身，并初始化其数据；
	static PlayerDivision * create(const std::string &name,  int keywordID, int score);
	bool init(const std::string &name, int keywordID, int score);

	//判断是否撞到食物等物体,并执行相应的eat函数
	bool collideFood(Food * food);
	bool collideSpore(Spore *spore);
	bool collidePrick(Prick *prick);
	//执行吃的时候改变分身的score,并调用calcuateDate更新分身数据；
	void eatFood(Food * food);
	void eatSpore(Spore * spore);
	void eatRival(int score);
	void eatPrick(int score);

	//获取分身速度；
	float getSpeed();
	//通过分身的score折算其半径和速度speed；
	void calculateData();
	
	//设定分身的分数
	virtual void setScore(int score);

	//在用户执行分身或吐孢子后更新分身的分数；在Player里调用；
	void spitSpore();
	void divide();

	//通过分身的半径重新设定分身的大小；
	void scaleSprite();

	//在分身上加上名字的标签；
	void setPlayerName(const std::string name);

	//设定和获取分身的速度；
	void setVelocity(Vec2 v);
	Vec2 getVelocity();

	//设定和获取上一帧的位置
	void setPrePosition(const Vec2 & position);
	Vec2 getPrePosition();

private:
	Label * _nameLabel;
	std::string _name;
	int _keywordID;

	float _speed;
	Vec2 _velocity;
	Vec2 _prePosition;  //上一帧的位置
};

#endif



