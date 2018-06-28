#pragma once
#ifndef _PLAYER_H
#define _PLAYER_H

#include "cocos2d.h"

USING_NS_CC;

class Prick;
class PlayerDivision;
class Food;
class Spore;

class Player :public Node {
public:

	enum class State
	{
		NORMAL, //����״̬
		DIVIDE,  //����״̬,130��
		SPIT,  //����״̬
	};

	Player();
	~Player();


	//������Ҳ�������һ�����Ϣ��
	static Player * create(std::string & name,  int keywordID,/* Vec2 velocity,*/ Node * map);
	bool init(std::string & name, int keywordID, /*Vec2 velocity,*/ Node * map);
	//�˳���
	virtual void onExit();

	//����һ����ҷ�����Ҫλ�á��ٶȡ��÷ֲ�����
	PlayerDivision * createDivision(Vec2 position, int score);
	//�������һ�ο����³���������
	int countSpitSporeNum();		
	//������,��ҪMap��sporeMap�����globalID;
	void spitSpore(Node * map, Map<int, Spore *> & sporeMap, int globalID);		
	//����,���������̬��
	void dividePlayer();		
	
	bool collideFood(Food * food);		//��ʳ�����ײ���
	bool collideSpore(Spore * spore);		//�����ӵ���ײ���
	bool collidePrick(Prick *prick);		//���̴̵���ײ���
	bool collideRival(Player *rival);		//����ֵ���ײ���,�ж�˭��˭��
	int collideDivision(PlayerDivision * division);		//����ַ������ײ�����ؽ��ֵ��collideRival�ж�˭��˭��

	void updateDivision();		//�����Ϣ����
	void GameOver();			//��Ҹ���

	void divideFinish();		//�����ص�Normal״̬��

	void collidePrickFinish();	//�����̻ص��к�״̬��

	

	Rect getPlayerRect();		//��ȡΧס������з���ĳ����Σ�
	void setCombine(float dt);  //�����ں�״̬,�������ںϻ��߷���״̬��
	int getDivisionNum();	    //��ȡ��ǰ��������
	std::string getPlayerName();//����������ƣ�
	int getTotalScore();        //�õ���ҷ������֮�ͼ���Ϸ������
	Vector<PlayerDivision *> & getDivisionList();//��ȡ�����б�
	void clearDivisionList();
private:
	Vector<PlayerDivision *> _divisionList;		//��ҷ����б�
	int _divisionNum;							//��������

	Node * _map;			//��ͼָ��
	std::string _playerName;		//�����
	int _keywordID;			//�ؼ���id

	int _color;			//��ɫ,û�йؼ���ʱ����

	State _state;		//���״̬
	bool _combineEnable; //�����Ƿ���Ժ���
};

#endif
