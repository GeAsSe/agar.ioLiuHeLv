#include "Player.h"

Player::Player()
{


	_name = "phantom";
	_keyID = 1;
	_state = State::NORMAL;
	_combineEnable = true;
	_ID = 0;
	m_target =NULL;
	_divisionNum = 0;
}

Player::~Player()
{
	_divisionList.clear();
}

Player * Player::create(std::string & name, int keyID, int ID, ICommunication *target)
{
	Player * player = new Player();

	player->_name = name;
	player->_keyID = keyID;
	player->_state = State::NORMAL;
	player->_combineEnable = true;
	player->_ID = ID;
	player->m_target = target;

	return player;
}

PD * Player::createDivision(double x, double y, int score)
{
	auto division = PD::create(_name, _keyID, score);
	division->set_x(x);
	division->set_y(y);
	_divisionList.push_back(division);
	_divisionNum++;
	return nullptr;
}

int Player::CountSpitSporeNum()
{	
	int count = 0;
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			int score = division->getScore();
			if (score >= 32)
			{
				count++;
			}
		}
	}
	return count;
}

bool Player::collideRival(Player * rival)
{
	bool bRet = false;
	for (int i = 0; i < _divisionList.size(); i++)
	{
		auto division1 = _divisionList.at(i);
		int result = rival->collideDivision(division1);
		if (result == 2) // 玩家分身被吃
		{
			bRet = true;
		}
		else if (result == 1) // 玩家分身吃了敌人
		{
			bRet = true;
		}
	}
	return bRet;
}

int Player::collideDivision(PD * division)
{
	int flag = 0;
	double x1 = division->get_x();
	double y1 = division->get_y();
	float playerRadius = division->getRadius();

	for (int j = 0; j < _divisionList.size(); j++)
	{
		auto division2 = _divisionList.at(j);
		double x2 = division2->get_x();
		double y2 = division2->get_y();
		float rivalRadius = division2->getRadius();
		float distance = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
		if (distance< abs(playerRadius - rivalRadius))
		{
			int playerScore = division->getScore();
			int rivalScore = division2->getScore();
			if (playerScore>rivalScore*1.20) //玩家分身吃了对手
			{
				return 1;
			}
			else if (rivalScore > playerScore*1.20)  //玩家分身被吃
			{
				return 2;
			}
		}
	}
	return flag;
}

int Player::getDivisionNum()
{
	return _divisionNum;
}

std::string Player::getPlayername()
{
	return _name;
}

int Player::getTotalScore()
{
	int score = 0;
	for (auto division : _divisionList) {
		if (division != NULL) {
			score += division->getScore();
		}
	}
	return score;
}


vector<PD*>& Player::getDivisionList()
{
	return _divisionList;
	// TODO: 在此处插入 return 语句
}


void Player::clearDivisionList()
{
	_divisionList.clear();
	_divisionNum = 0;
}

ICommunication * Player::get_target()
{
	return m_target;
}

int Player::get_keyID()
{
	return _keyID;
}
