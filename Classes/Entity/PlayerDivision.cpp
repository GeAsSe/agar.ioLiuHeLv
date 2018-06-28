#include "PlayerDivision.h"
#include "Food.h"
#include "Prick.h"
#include "Spore.h"
#include "Header\AppMacros.h"


PlayerDivision::PlayerDivision()
{
}

PlayerDivision::~PlayerDivision()
{
}

PlayerDivision * PlayerDivision::create(const std::string& filename)
{
	PlayerDivision * playerDivision = new PlayerDivision();
	if (playerDivision && playerDivision->init(filename))
	{
		playerDivision->autorelease();
		return playerDivision;
	}
	CC_SAFE_DELETE(playerDivision);
	return nullptr;
}

bool PlayerDivision::init(const std::string& filename)
{
	if (!Entity::initWithFile(filename))
	{
		return false;
	}
	return true;
}

PlayerDivision * PlayerDivision::create(const std::string &name, int keywordID, int score)
{
	PlayerDivision * playerDivision = new PlayerDivision();
	if (playerDivision && playerDivision->init(name,  keywordID, score))
	{
		playerDivision->autorelease();
		return playerDivision;
	}
	CC_SAFE_DELETE(playerDivision);
	return nullptr;
}

bool PlayerDivision::init(const std::string &name,  int keywordID, int score)
{
	_name = name;
	_keywordID = keywordID;

	if (!Entity::initWithFile(StringUtils::format("BALL%d.png", _keywordID)))
	{
		return false;
	}

	_score = score;
	calculateData();

	return true;
}

bool PlayerDivision::collideFood(Food * food)
{
	Vec2 position = this->getPosition();
	Vec2 foodPosition = food->getPosition();
	if (foodPosition.distance(position) <= _radius)
	{
		eatFood(food);
		return true;
	}

	return false;
}

void PlayerDivision::eatFood(Food * food)
{
	_score += food->getScore();
	calculateData();
}

float PlayerDivision::getSpeed()
{
	return _speed;
}

bool PlayerDivision::collideSpore(Spore * spore)
{
	if (_score<PLAYER_MIN_EAT_SPORE_SCORE)
	{
		return false;
	}

	Vec2 position = this->getPosition();
	Vec2 sporePosition = spore->getPosition();
	if (position.distance(sporePosition) <= _radius)
	{
		eatSpore(spore);
		return true;
	}
	return false;
}

void PlayerDivision::eatSpore(Spore * spore)
{
	_score += spore->getScore();
	calculateData();
}

bool PlayerDivision::collidePrick(Prick * prick)
{
	float prickScore = prick->getScore();

	if (_score>prickScore*MIN_EAT_MULTIPLE)
	{
		Vec2 prickPosition = prick->getPosition();
		Vec2 divisionPostion = this->getPosition();
		float distance = prickPosition.distance(divisionPostion);
		if (distance <= abs(_radius - prick->getRadius()))
		{
			return true;
		}
	}
	return false;
}

void PlayerDivision::eatRival(int score)
{
	_score += score;
	calculateData();
}

void PlayerDivision::eatPrick(int score)
{
	_score = score;
	calculateData();
}

void PlayerDivision::setScore(int score)
{
	_score = score;
	calculateData();
}

void PlayerDivision::calculateData()
{
	_radius = sqrt(_score * PLAYER_INITIAL_RADIUS * PLAYER_INITIAL_RADIUS / PLAYER_INITIAL_SCORE);
	_speed = (PLAYER_INITIAL_RADIUS / _radius) * (PLAYER_INITIAL_SPEED - PLAYER_MIN_SPEED) + PLAYER_MIN_SPEED;
	this->setLocalZOrder(_score);
	scaleSprite();
}

void PlayerDivision::spitSpore()
{
	_score -= SPORE_SCORE;
	calculateData();
}

void PlayerDivision::divide()
{
	_score /= 2;
	calculateData();
}

void PlayerDivision::scaleSprite()
{
	Size size = this->getContentSize();
	float scale = float(_radius * 2) / size.width;
	this->setScale(scale);
}

void PlayerDivision::setVelocity(Vec2 v)
{
	_velocity = v;
}

Vec2 PlayerDivision::getVelocity()
{
	return _velocity;
}


void PlayerDivision::setPrePosition(const Vec2 & position)
{
	_prePosition = position;
}


Vec2 PlayerDivision::getPrePosition()
{
	return _prePosition;
}

void PlayerDivision::setPlayerName(const std::string name)
{
	_nameLabel = Label::createWithTTF(name.c_str(), "fonts/STSONG.TTF", 22);//fontsÎª×ÖÌå£»
	Size size = this->getContentSize();
	_nameLabel->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(_nameLabel);
}


