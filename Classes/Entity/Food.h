#pragma once
#ifndef _FOOD_H
#define _FOOD_H
#include"Entity.h"

class Food:public Entity
{
public:
	Food();
	~Food();
    //����ʳ��
	static Food * create(const std::string& filename);
	//��ʼ��ʳ��
	bool init(const std::string& filename);

};



#endif // !1



