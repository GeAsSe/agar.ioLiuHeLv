#pragma once
#include "cocos2d.h"
#include "Web\KXServer.h"
#include "Web\Singleton.h"
USING_NS_CC;
using namespace KxServer;

class CClient : public IBaseModule
{
	DECLARE_SINGLETON(CClient)
private:
	CClient();
	virtual ~CClient();
public:
	 bool ClientInit();
	 void ClientUnInit();
public:
	void resetPoller();
	void Send(char*, int);
private:
	virtual int RequestLen(char* buffer, unsigned int len);
		/*
		//返回协议长度
		if (len < sizeof(int))
		{
			return sizeof(int);
		}
		else
		{
			return *(int*)(buffer);
		}*/
	void Process(char* buffer, unsigned int len, ICommunication *target);
private:
	/*CTCPListener * m_Listener;*/
	CTCPConnector* m_Connecter;
	CSelectPoller* m_Poller;

};