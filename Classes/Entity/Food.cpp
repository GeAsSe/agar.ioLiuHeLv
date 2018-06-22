#include "Food.h"
#include"Header\AppMacros.h"

Food::Food()
{
}


Food::~Food()
{
}

Food * Food::create(const std::string& filename)
{
	Food * food = new Food();
	if (food && food->init(filename))
	{
		food->autorelease();
		return food;
	}
	CC_SAFE_DELETE(food);
	return nullptr;
}

bool Food::init(const std::string& filename)
{
	if (!Entity::initWithSpriteFrameName(filename))
	{
		return false;
	}

	_score = FOOD_SCORE;
	_radius = FOOD_RADIUS;

	Size size = this->getContentSize();
	float scale = (_radius * 2) / size.width;
	this->setScale(scale);

	return true;
}
