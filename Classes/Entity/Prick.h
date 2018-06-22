#pragma once
#ifndef _PRICK_H
#define _PRICK_H
#include"Entity.h"


class Prick:public Entity
{
public:
	Prick();
	~Prick();
	//创建刺
	static Prick * create(const std::string& filename);
	//初始化刺
	bool init(const std::string& filename);
};

#endif 



