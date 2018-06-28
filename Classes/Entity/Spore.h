#pragma once
#ifndef _Spore_H_
#define _Spore_H_
#include"Entity.h"

class Spore : public Entity {
public:
	Spore();
	~Spore();
	//��������
	static Spore * create(const std::string& filename);
	//��ʼ������
	bool init(const std::string& filename);
	//��������--ָ������

private:
	int _skinID;
};



#endif