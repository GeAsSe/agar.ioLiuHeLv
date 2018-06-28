#pragma once
#ifndef _PLAYERDIVISION_H
#define _PLAYERDIVISION_H

#include"Entity.h"

class Food;
class Prick;
class Spore;

class PlayerDivision:public Entity
{
public:
	PlayerDivision();
	~PlayerDivision();

	//��������
	static PlayerDivision * create(const std::string& filename);
	//��ʼ������
	bool init(const std::string& filename);
	//����������ҷ��ѳ��ķ�������ʼ�������ݣ�
	static PlayerDivision * create(const std::string &name,  int keywordID, int score);
	bool init(const std::string &name, int keywordID, int score);

	//�ж��Ƿ�ײ��ʳ�������,��ִ����Ӧ��eat����
	bool collideFood(Food * food);
	bool collideSpore(Spore *spore);
	bool collidePrick(Prick *prick);
	//ִ�гԵ�ʱ��ı�����score,������calcuateDate���·������ݣ�
	void eatFood(Food * food);
	void eatSpore(Spore * spore);
	void eatRival(int score);
	void eatPrick(int score);

	//��ȡ�����ٶȣ�
	float getSpeed();
	//ͨ�������score������뾶���ٶ�speed��
	void calculateData();
	
	//�趨����ķ���
	virtual void setScore(int score);

	//���û�ִ�з���������Ӻ���·���ķ�������Player����ã�
	void spitSpore();
	void divide();

	//ͨ������İ뾶�����趨����Ĵ�С��
	void scaleSprite();

	//�ڷ����ϼ������ֵı�ǩ��
	void setPlayerName(const std::string name);

	//�趨�ͻ�ȡ������ٶȣ�
	void setVelocity(Vec2 v);
	Vec2 getVelocity();

	//�趨�ͻ�ȡ��һ֡��λ��
	void setPrePosition(const Vec2 & position);
	Vec2 getPrePosition();

private:
	Label * _nameLabel;
	std::string _name;
	int _keywordID;

	float _speed;
	Vec2 _velocity;
	Vec2 _prePosition;  //��һ֡��λ��
};

#endif



