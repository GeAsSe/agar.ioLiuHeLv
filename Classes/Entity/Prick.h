#pragma once
#ifndef _PRICK_H
#define _PRICK_H
#include"Entity.h"


class Prick:public Entity
{
public:
	Prick();
	~Prick();
	//������
	static Prick * create(const std::string& filename);
	//��ʼ����
	bool init(const std::string& filename);
};

#endif 



