#include "PD.h"

PD::PD()
{
	DEBUG_LOG("PlayerDivision created");
}

PD::~PD()
{
	DEBUG_LOG("PlayerDivision destroyed");
}

PD * PD::create(const std::string & name, int keyID, int score)
{
	PD * pd = new PD();
	
	pd->_name = name;
	pd->_keyID = keyID;
	pd->_score = score;
	pd->calculateData();

	return pd;
}

bool PD::collideFood(Food * food)
{
	double distance = sqrt((_positionx - food->get_x())*(_positionx - food->get_x()) + (_positiony - food->get_y())*(_positiony - food->get_y()));
	if (distance <= _radius) {
		eatFood(food);
		return true;
	}
	return false;
}

bool PD::collideSpore(Spore * spore)
{
	if (_score < 18) {
		return false;
	}
	double distance = sqrt((_positionx - spore->get_x())*(_positionx - spore->get_x()) + (_positiony - spore->get_y())*(_positiony - spore->get_y()));
	if (distance <= _radius) {
		eatSpore(spore);
		return true;
	}
	return false;
}

bool PD::collidePrick(Prick * prick)
{
	float prickScore = prick->getScore();
	if (_score > prickScore*1.20) {
		double distance = sqrt((_positionx - prick->get_x())*(_positionx - prick->get_x()) + (_positiony - prick->get_y())*(_positiony - prick->get_y()));
		if (distance <= abs(_radius - prick->getRadius())) {
			return true;
		}
	}
	return false;
}

void PD::eatFood(Food * food)
{
	_score += food->getScore();
	calculateData();
}

void PD::eatSpore(Spore * spore)
{
	_score += spore->getScore();
	calculateData();
}

void PD::eatPrick(int score)
{
	_score = score;
	calculateData();
}

void PD::eatRival(int score)
{
	_score += score;
	calculateData();
}

float PD::getSpeed()
{
	return _speed;
}

void PD::calculateData()
{
	_radius = sqrt(_score * 21 * 21 / 10);
	_speed = (21 / _radius)*(6 - 1) + 1;
}

void PD::setScore(int score)
{
	_score = score;
	calculateData();
}

void PD::spitSpore()
{
	_score -= 14;
	calculateData();
}

void PD::divide()
{
	_score /= 2;
	calculateData();
}

void PD::setV(double x, double y)
{
	_V_x = x;
	_V_y = y;
}

double PD::getV_x()
{
	return _V_x;
}

double PD::getV_y()
{
	return _V_y;
}

void PD::setPrePosition(double x, double y)
{
	_PrePosition_x = x;
	_PrePosition_y = y;
}

double PD::getPrePosition_x()
{
	return _PrePosition_x;
}

double PD::getPrePosition_y()
{
	return _PrePosition_y;
}

