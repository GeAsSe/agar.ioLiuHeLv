#include "Player.h"
#include "PlayerDivision.h"
#include "Spore.h"
#include "Prick.h"
#include"Food.h"
#include "Header/AppMacros.h"
#include"Classes\EndScene\EndScene.h"

Player::Player()
{
	_divisionNum = 0;
}

Player::~Player()
{
	_divisionList.clear();
}

Player * Player::create(Vec2 position, Node * map)
{
	Player * player = new Player();
	if (player && player->init(position, map))
	{
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

bool Player::init(Vec2 position, Node * map)
{
	if (!Node::init())
	{
		return false;
	}


	int nameIndex = rand() % 10 + 1;
	_playerName = "Hello";
	//_playerName = CCString::createWithFormat("%s", I18N::shareI18N()->getcStringByKey(nameIndex));

	_map = map;
	_keywordID = 1;
	_state = State::NORMAL;
	_combineEnable = true;

	auto division = this->createDivision(position,  PLAYER_INITIAL_SCORE);
	_map->addChild(division, PLAYER_INITIAL_SCORE);

	return true;
}

Player * Player::create(std::string & name, int keywordID, /*Vec2 velocity,*/Node * map)
{
	Player * player = new Player();
	if (player && player->init(name,  keywordID, /*velocity, */map))
	{
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

bool Player::init(std::string & name,  int keywordID, /*Vec2 velocity,*/ Node * map)
{
	if (!Node::init())
	{
		return false;
	}

	_playerName = name;
	_map = map;
	_keywordID = keywordID;
	/*_velocity = velocity;*/
	_state = State::NORMAL;
	_combineEnable = true;

	return true;
}

PlayerDivision * Player::createDivision(Vec2 position, /*Vec2 velocity,*/ int score)
{
	/*创建一个玩家分身*/
	auto division = PlayerDivision::create(_playerName, _keywordID, score);
	division->setPosition(position);
	division->setPlayerName(_playerName);
	_divisionList.pushBack(division);
	_divisionNum++;
	return division;
}


void  Player::dividePlayer()
{
	bool divideFlag = false;
	int size = _divisionList.size();
	for (int i = 0; i<size; i++)
	{
		auto division = _divisionList.at(i);
		int score = division->getScore();
		if (score >= PLAYER_MIN_DIVIDE_SCORE)
		{
			if (_divisionNum + 1 > PLAYER_MAX_DIVISION_NUM)
			{
				break;
			}

			this->unscheduleAllCallbacks();
			_state = State::DIVIDE;
			_combineEnable = false;
			divideFlag = true;
			division->divide();
			float radius = division->getRadius();
			int score = division->getScore();
			Vec2 position = division->getPosition();
			auto newDivision = this->createDivision(position, /*_velocity,*/ score);
			_map->addChild(newDivision);

			float angle = /*_velocity*/(division->getVelocity()).getAngle();
			float divideDistance = radius + PLAYER_MIN_DIVIDE_DISTANCE;
			Vec2 newPosition = Vec2(divideDistance*cosf(angle), divideDistance*sinf(angle));

			auto sequence = Sequence::create(
				EaseOut::create(MoveBy::create(0.5f, newPosition), 1.8f),
				CallFunc::create(CC_CALLBACK_0(Player::divideFinish, this)),
				NULL);
			newDivision->runAction(sequence);
		}
	}

	if (divideFlag)
	{
		this->scheduleOnce(schedule_selector(Player::setCombine), 15);
	}


}

Rect Player::getPlayerRect()
{
	float minX, minY, maxX, maxY;

	auto division = _divisionList.at(0);
	if (division != NULL)
	{
		Vec2 divisionPosition = division->getPosition();
		float radius = division->getRadius();
		minX = divisionPosition.x - radius;
		maxX = divisionPosition.x + radius;
		minY = divisionPosition.y - radius;
		maxY = divisionPosition.y + radius;
	}

	for (auto item : _divisionList)
	{

		if (item != NULL)
		{
			Vec2 divisionPosition = item->getPosition();
			float radius = item->getRadius();
			if (minX > divisionPosition.x - radius) {
				minX = divisionPosition.x - radius; 
			}
			if (maxX <divisionPosition.x + radius) { 
				maxX = divisionPosition.x + radius; 
			}
			if (minY >divisionPosition.y - radius) {
				minY = divisionPosition.y - radius;
			}
			if (maxY < divisionPosition.y + radius) { 
				maxY = divisionPosition.y + radius;
			}
		}
	}

	Rect rect;
	rect.size = Size(maxX - minX, maxY - minY);
	rect.origin = Vec2((minX + maxX) / 2, (minY + maxY) / 2);
	return rect;
}

bool Player::collideFood(Food * food)
{
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			if (division->collideFood(food))
			{
				division->setLocalZOrder((int)ceil(division->getScore()));
				return true;
			}
		}
	}
	return false;
}

void Player::updateDivision()
{


	auto rect = this->getPlayerRect();

	/*根据速度改变每个分身的位置*/
	for (auto division : _divisionList)
	{
		
		if (division != NULL)
		{
			float speed = division->getSpeed();
	
			Vec2 velocity = division->getVelocity();
			float dx = velocity.x*speed;
			float dy = velocity.y*speed;
			Vec2 divisionVec = Vec2(dx, dy); //分身移动方向
			Vec2 oldPosition = division->getPosition();
			Vec2 newPosition = Vec2(oldPosition.x + divisionVec.x, oldPosition.y + divisionVec.y);

			float radius1 = division->getRadius();
			if (!_combineEnable)//不在合体、分身状态，分身会进行碰撞检测
			{
				for (auto division2 : _divisionList)
				{
					if (division2 != NULL && division2 != division)
					{
						Vec2 position2 = division2->getPosition();
						float radius2 = division2->getRadius();
						float oldDistance = position2.distance(oldPosition);
						if (oldDistance <= radius1 + radius2)//处理分身时可能产生碰撞的情况
						{
							/*
							将两个圆以圆心连接线分别向两边移动
							*/
							float distance = ((radius1 + radius2) - oldDistance) / 2;
							Vec2 vec = oldPosition - position2;
							float angle = vec.getAngle();

							break;
						}
					}
				}
			}

			if (newPosition.x <= radius1)
			{
				newPosition.x = radius1;
			}
			if (newPosition.x >= MAP_WIDTH - radius1)
			{
				newPosition.x = MAP_WIDTH - radius1;
			}
			if (newPosition.y <= radius1)
			{
				newPosition.y = radius1;
			}
			if (newPosition.y >= MAP_HEIGHT - radius1)
			{
				newPosition.y = MAP_HEIGHT - radius1;
			}
			division->setPrePosition(oldPosition);
			division->setPosition(newPosition);
		}
	}
	if (!_combineEnable)//分身会进行碰撞检测，移动后如果产生碰撞则取消移动
	{
		for (auto division1 : _divisionList)
		{
			if (division1 != NULL)
			{
				for (auto division2 : _divisionList)
				{
					if (division2 != NULL && division2 != division1)
					{
						Vec2 position1 = division1->getPosition();
						Vec2 position2 = division2->getPosition();
						float radius1 = division1->getRadius();
						float radius2 = division2->getRadius();
						float distance = position1.distance(position2);
						if (distance < radius1 + radius2)
						{
							Vec2 oldPosition1 = division1->getPrePosition();
							Vec2 oldPosition2 = division2->getPrePosition();
							float oldDistance = oldPosition1.distance(oldPosition2);
							float fixDistance = 0;
							if (oldDistance <= radius1 + radius2)
							{
								fixDistance = 2;
							}
							else
							{
								fixDistance = (radius1 + radius2 - distance);
							}

							Vec2 v = position2 - position1;
							float angle = v.getAngle();
							Vec2 newPosition1 = Vec2(position1.x - cosf(angle)*fixDistance, position1.y - sinf(angle)*fixDistance);
							Vec2 newPosition2 = Vec2(position2.x + sinf(angle)*fixDistance, position2.y + sinf(angle)*fixDistance);
							division1->setPosition(newPosition1);
							//division2->setPosition(newPosition2);
							//break;
						}
					}
				}
			}
		}
	}

	if (_combineEnable)  //处理分身合体
	{
		for (int i = 0; i < _divisionList.size(); i++)
		{
			auto division1 = _divisionList.at(i);
			if (division1 != NULL)
			{
				Vec2 position1 = division1->getPosition();
				float radius1 = division1->getRadius();
				for (int j = i + 1; j < _divisionList.size(); j++)
				{
					auto division2 = _divisionList.at(j);
					if (division2 != NULL)
					{
						Vec2 position2 = division2->getPosition();
						float radius2 = division2->getRadius();
						float distance = position2.distance(position1);
						if (distance<abs(radius2 - radius1))     //两个分身可以合体
						{
							_divisionNum--;

							_combineEnable = false;
							this->scheduleOnce(schedule_selector(Player::setCombine), 8);

							if (radius1>radius2)
							{
								division1->eatRival(division2->getScore());
								_divisionList.eraseObject(division2);
								division2->removeFromParentAndCleanup(true);
								return;
							}
							else
							{
								division2->eatRival(division1->getScore());
								_divisionList.eraseObject(division1);
								division1->removeFromParentAndCleanup(true);
								return;
							}
						}
					}

				}
			}
		}
	}
}

void Player::spitSpore(Node * map, Map<int, Spore *> & sporeMap, int globalID)
{
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			int score = division->getScore();
			if (score >= PLAYER_MIN_SPIT_SCORE)
			{
				division->spitSpore();
				Vec2 position = division->getPosition();
				float angle = division->getVelocity().getAngle();
				float radius = division->getRadius();
				Spore * spore = Spore::create("spore.png");
				Vec2 sporePosition = Vec2(position.x + radius * cosf(angle) * 2, position.y + radius * sinf(angle) * 2);
				spore->setPosition(sporePosition);

				Vec2 newPosition = Vec2(PLAYER_MIN_SPIT_DISTANCE*cosf(angle), PLAYER_MIN_SPIT_DISTANCE*sinf(angle));
				auto action = MoveBy::create(0.5, newPosition);
				auto action2 = EaseOut::create(action, 1.8f);
				spore->runAction(action2);
				spore->setLocalZOrder(spore->getScore());
				map->addChild(spore);
				sporeMap.insert(globalID, spore);
				globalID++;
			}
		}
	}
}

bool Player::collideSpore(Spore * spore)
{
	for (auto division : _divisionList)
	{

		if (division != NULL)
		{
			if (division->collideSpore(spore))
			{
				division->setLocalZOrder((int)ceil(division->getScore()));
				return true;
			}
		}
	}

	return false;
}

void Player::setCombine(float dt)
{
	_combineEnable = true;
}

void Player::divideFinish()
{
	_state = State::NORMAL;
}

bool Player::collidePrick(Prick *prick)
{
	bool collideFlag = false;
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			Vec2 prickPosition = prick->getPosition();
			if (division->collidePrick(prick))
			{
				int prickScore = prick->getScore();
				int divisionScore = division->getScore();
				divisionScore += prickScore;

				/*计算与刺碰撞后生成小球的大小*/
				int splitScore = (prickScore + division->getScore()) / (MAX_EAT_PRICK_DIVISION_NUM + 1);
				if (splitScore >= MAX_EAT_PRICK_SCORE)
				{
					for (int i = 0; i < MAX_EAT_PRICK_DIVISION_NUM; i++)
					{
						if (_divisionNum + 1 +i>= PLAYER_MAX_DIVISION_NUM)
						{
							break;
						}
						splitScore = MAX_EAT_PRICK_SCORE;
						divisionScore = divisionScore - splitScore;
					}
					division->eatPrick(divisionScore);
				}
				else
				{
					for (int i = 0; i < MAX_EAT_PRICK_DIVISION_NUM; i++)
					{
						if (_divisionNum + 1 +i>= PLAYER_MAX_DIVISION_NUM)
						{
							break;
						}
						divisionScore = divisionScore - splitScore;
					}
					division->eatPrick(divisionScore);
				}

				/*循环创建小球*/
				float angle = 0;
				Vec2 divisionPosition = division->getPosition();
				float radius = sqrt(splitScore * PLAYER_INITIAL_RADIUS * PLAYER_INITIAL_RADIUS / PLAYER_INITIAL_SCORE);
				float anglePerDivision = PI * 2 / MAX_EAT_PRICK_DIVISION_NUM;
				for (int i = 0; i < MAX_EAT_PRICK_DIVISION_NUM; i++)
				{
					if (_divisionNum + 1 >= PLAYER_MAX_DIVISION_NUM)
					{
						break;
					}

					_state = State::DIVIDE;
					_combineEnable = false;

					Vec2 velocity = Vec2(cosf(angle), sinf(angle));
					auto splitDivision = this->createDivision(divisionPosition, /*velocity,*/ splitScore);
					_map->addChild(splitDivision, splitScore);

					Vec2 newPosition = Vec2(PRICK_SPLIT_DISTANCE*cosf(angle), PRICK_SPLIT_DISTANCE*sinf(angle));
					auto sequence = Sequence::create(
						EaseOut::create(MoveBy::create(0.3f, newPosition), 1.8f),
						//CallFunc::create(CC_CALLBACK_0(Player::divideFinish, this),
						CallFunc::create(CC_CALLBACK_0(Player::collidePrickFinish, this)),
						NULL);
					splitDivision->runAction(sequence);

					angle += anglePerDivision;

				}

				this->scheduleOnce(schedule_selector(Player::setCombine), 15);

				return true;
			}
		}
	}

	return false;
}
/*
void Player::setConcentrate()
{
	_state = State::CONCENTRATE;
}
*/
bool Player::collideRival(Player * rival)
{
	bool bRet = false;
	for (int i = 0; i < _divisionList.size(); i++)
	{
		auto division1 = _divisionList.at(i);
		int result = rival->collideDivision(division1);
		if (result == 2) // 玩家分身被吃
		{
			_divisionList.eraseObject(division1);
			division1->removeFromParentAndCleanup(true);
			_divisionNum--;
			i--;
			bRet = true;
		}
		else if (result == 1) // 玩家分身吃了敌人
		{
			bRet = true;
		}
	}

	return bRet;
}

int Player::collideDivision(PlayerDivision * division)
{
	int flag = 0;
	Vec2 playerPosition = division->getPosition();
	float playerRadius = division->getRadius();

	for (int j = 0; j < _divisionList.size(); j++)
	{
		auto division2 = _divisionList.at(j);
		Vec2 rivalPosition = division2->getPosition();
		float rivalRadius = division2->getRadius();
		float distance = playerPosition.distance(rivalPosition);
		if (distance< abs(playerRadius - rivalRadius))
		{
			int playerScore = division->getScore();
			int rivalScore = division2->getScore();
			if (playerScore>rivalScore*MIN_EAT_MULTIPLE)  //玩家分身吃了对手
			{
				division->eatRival(rivalScore);
				return 1;
			}
			else if (rivalScore > playerScore*MIN_EAT_MULTIPLE)  //玩家分身被吃
			{
			    //division2->eatRival(playerScore);
				return 2;
			}
		}
	}
	return flag;
}

std::string  Player::getPlayerName()
{
	return _playerName;
}

void Player::GameOver()
{
	Director::getInstance()->replaceScene(EndScene::createScene());
}

int Player::getTotalScore()
{
	int score = 0;

	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			score += division->getScore();
		}
	}
	return score;
}

int Player::getDivisionNum()
{
	return _divisionNum;
}

Vector<PlayerDivision *> & Player::getDivisionList()
{
	return _divisionList;
}



void Player::onExit()
{
	Node::onExit();
}

void Player::clearDivisionList()
{
	for (auto division : _divisionList)
	{

		division->removeFromParentAndCleanup(true);
	}
	_divisionList.clear();
	_divisionNum = 0;
}

int Player::countSpitSporeNum()
{
	int count = 0;
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			int score = division->getScore();
			if (score >= PLAYER_MIN_SPIT_SCORE)
			{
				count++;
			}
		}
	}

	return count;
}

void Player::collidePrickFinish()
{
	_state = State::NORMAL;
}