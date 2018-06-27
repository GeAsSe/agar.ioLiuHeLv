#ifndef _ENTITY_H_
#define _ENTITY_H_
#include<string>
#include<map>
#include<vector>
#include<math.h>
#include"AgarServer/Log.h"

class Entity {
public:
	int getScore();
	virtual void setScore(int score);
	float getRadius();
	void setRadius(float radius);
	void setPosition(double x, double y);
	double get_x();
	void set_x(double x);
	double get_y();
	void set_y(double y);
protected:
	int _score;
	float _radius;
	double _positionx;
	double _positiony;
};

#endif