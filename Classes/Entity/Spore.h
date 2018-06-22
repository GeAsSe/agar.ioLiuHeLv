#pragma once
#ifndef _Spore_H_
#define _Spore_H_
#include"Entity.h"

class Spore : public Entity {
public:
	Spore();
	~Spore();
	//创建孢子
	static Spore * create(const std::string& filename);
	//初始化孢子
	bool init(const std::string& filename);
	//创建孢子--指定类型
	static Spore * create(int id);
	bool init(int id);

	int getID();
private:
	int _skinID;
};



#endif