#ifndef _SPORE_H_
#define _SPORE_H_
#include"Entity.h"

class Spore :public Entity {
public:
	Spore();
	~Spore();
	static Spore * create(double x, double y);
	bool init(double x, double y);

};

#endif