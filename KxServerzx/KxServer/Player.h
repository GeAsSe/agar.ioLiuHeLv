#ifndef _PLAYER_H_
#define _PLAYER_H_
#include"Spore.h"
#include"food.h"
#include"PD.h"
#include"Prick.h"
#include "KXServer.h"
using namespace KxServer;

class Prick;
class PD;
class Food;
class Spore;

class Player {
public:
	enum State {
		NORMAL,
		DIVIDE,
		SPIT
	};

	Player();
	~Player();

	static Player * create(std::string &name, int keyID, int ID, ICommunication *target);
	bool init(std::string &name, int keyID, int ID, ICommunication *target);

	PD * createDivision(double x, double y, int score);

	int CountSpitSporeNum();

	bool collideRival(Player * rival);

	int collideDivision(PD * division);

	int getDivisionNum();
	std::string getPlayername();
	int getTotalScore();

	vector<PD *>& getDivisionList();
	void clearDivisionList();
	ICommunication* get_target();
	int get_keyID();
private:
	vector<PD *> _divisionList;
	int _divisionNum;
	std::string _name;
	int _keyID;
	State _state;
	bool _combineEnable;
	int _ID;
	ICommunication*         m_target;
};


#endif // !_PLAYER_H_
