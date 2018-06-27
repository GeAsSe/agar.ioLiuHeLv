#include "Entity.h"

int Entity::getScore()
{	
	return _score;
}

void Entity::setScore(int score)
{
	_score = score;
}

float Entity::getRadius()
{
	return _radius;
}

void Entity::setRadius(float radius) {
	_radius = radius;
}

void Entity::setPosition(double x, double y)
{
	_positionx = x;
	_positiony = y;
}

double Entity::get_x()
{
	return _positionx;
}

void Entity::set_x(double x)
{
	_positionx = x;
}

double Entity::get_y()
{
	return _positiony;
}

void Entity::set_y(double y)
{
	_positiony = y;
}
