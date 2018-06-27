#ifndef _FOOD_H_
#define _FOOD_H_
#include"Entity.h"

class Food:public Entity {
public:
	Food();
	~Food();

	static Food * create(double x, double y);
	bool init(double x, double y);
};

#endif