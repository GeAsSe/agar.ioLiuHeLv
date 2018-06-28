#pragma once
#ifndef _GameLayer_H_
#define _GameLayer_H_

#include "cocos2d.h"
/*#include "Header\Common.h"*/
#include "ui\CocosGUI.h"
#include<vector>
#include"proj.win32\client.h"
#include"Classes\Web\Singleton.h"


USING_NS_CC;
using namespace ui;

class Joystick;
class Player;
class Food;
class Spore;
class Prick;

class GameLayer : public Layer {
public:
	GameLayer();
	~GameLayer();

	virtual bool init();

	CREATE_FUNC(GameLayer);

	virtual void onExit();

	virtual void update(float dt);		//��Ϸ��ÿ֡����

	void start();
	//void initData(int seed,);			//��ʼ����ͼ����
	void initRival(std::vector<int>playerIDs,
	int rivalNum, std::vector<std::string>playernames, std::vector<int> KeywordIDs,
	std::vector<int>divisionNum, std::vector<double>positions, std::vector<int>scores);		//��ʼ������
	void initPlayer(int ID, std::string name, int KeywordID, int divisionNum, std::vector<double>positions, std::vector<int>score);		//��ʼ�����
	void initFood(int seed);						//��ʼ������
	void initSpore(int sporeNum, std::vector<int>globalIDs, std::vector<double> position);		//��ʼ������
	void initPrick(int prickNum, std::vector<int> globalIDs, std::vector<double>position);		//��ʼ���̴�

	void resetpoller(float dt);
	/*�������Ժ���*/
	void initDataDefault();
	void initRival();
	void initPlayer();
	void initFood();

	void updateView();		//������Ϸ��ͼ
	void updateFood();		//����ʳ��
	void updateSpore();		//��������
	void updatePrick();		//�����̴�
	void updateRival();		//���¶���

	void updateRank(float dt);		//�������а���Ϣ
	void updateScore(float dt);		//���·���

	/*�������Ժ���*/
	void startAddPrick(float dt);//����ˢ��
	void addPrick(float dt);//�����Ӵ�

	void collideFood(Player * player);		//�붹�ӵ���ײ���
	void collide();			//��ײ���

	void spitSpore();		//�����Ӳ���
	void dividePlayer();		//�������
	void resetFood(Node * node);		//���ö���
	/*�������Ϣ�ϴ�*/
	/*void synPlayerMove();*/		//ͬ������ƶ�
	void synPlayerInfo();		//ͬ�������Ϣ
	void synSporeInfo();		//ͬ������
    /*���������������Ϣ*/
	void playerMoveEvent(int playerID,std::vector<double>position);
	void playerDivideEvent(int playerID);
	void spitSporeResultEvent(int playerID, int globalID, int sporeCount);
	void addPrickEvent(int globalID, int x, int y);
	void enterPlayerEvent(int playerID, std::string playername, int KeywordID, int divisionNum, std::vector<double>position, std::vector<int>scores);
	void updatePlayerEvent(int playerID, int divisionNum, std::vector<double>position, std::vector<int> scores);
private:
	Node * _map;				//��ͼ
	Player * _player;			//���
	Map<int, Player *> _rivalMap;		//�����б�
	Vector<Food *> _foodList;			//�����б�
	Map<int, Prick *> _prickMap;			//�̴�
	Map<int, Spore *> _sporeMap;			//����
	std::vector<int> _vecSporeNeedUpdate;			//��Ҫ���µ�����
	float _mapScale;			//��ͼ��������
	int _ID;                  //���ID��

};

#endif