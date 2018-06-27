#ifndef _PD_H_
#define _PD_H_
#include"Entity.h"
#include"food.h"
#include"Spore.h"
#include"Prick.h"

class Food;
class Prick;
class Spore;

class PD :public Entity {
public:
	PD();
	~PD();

	static PD * create(const std::string &name, int keyID, int score);
	bool init(const std::string &name, int keyID, int score);

	bool collideFood(Food * food);
	bool collideSpore(Spore * spore);
	bool collidePrick(Prick * prick);
	
	void eatFood(Food * food);
	void eatSpore(Spore * spore);
	void eatPrick(int score);
	void eatRival(int score);

	float getSpeed();
	void calculateData();

	virtual void setScore(int score);

	void spitSpore();
	void divide();

	void setV(double x, double y);
	
	double getV_x();
	double getV_y();

	void setPrePosition(double x, double y);
	double getPrePosition_x();
	double getPrePosition_y();

private:
	std::string _name;
	int _keyID;
	float _speed;
	double _V_x;
	double _V_y;
	double _PrePosition_x;
	double _PrePosition_y;
};


#endif
