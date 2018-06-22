#pragma once
#ifndef _FOOD_H
#define _FOOD_H
#include"Entity.h"

class Food:public Entity
{
public:
	Food();
	~Food();
    //创建食物
	static Food * create(const std::string& filename);
	//初始化食物
	bool init(const std::string& filename);

};



#endif // !1



