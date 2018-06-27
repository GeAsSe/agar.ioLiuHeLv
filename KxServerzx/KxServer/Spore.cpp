#include "Spore.h"

Spore::Spore()
{

}

Spore::~Spore()
{

}

Spore * Spore::create(double x, double y)
{
	Spore * spore = new Spore();

	spore->_radius = 23;
	spore->_score = 14;
	spore->setPosition(x, y);
	return spore;
}
