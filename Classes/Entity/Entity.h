#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include"cocos2d.h"


USING_NS_CC;

class Entity:public Sprite
{
public:
	int getScore();
	virtual void setScore(int score);
	float getRadius();
	void setRadius(float radius);
protected:
	float _radius;
	int _score;
};


#endif