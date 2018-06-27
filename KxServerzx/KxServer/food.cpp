#include "food.h"

Food::Food()
{
	
}

Food::~Food()
{

}

Food * Food::create(double x, double y)
{
	Food * food = new Food();
	if (food && food->init(x,y)) {
		return food;
	}
	
	do {
		delete(food);
		food = nullptr;
	} while (0);
	return nullptr;
}

bool Food::init(double x,double y)
{
	_score = 1;
	_radius = 12;
	setPosition(x, y);
	return true;
}


