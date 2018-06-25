#pragma once
#include "cocos2d.h"
#include "KXServer.h"
USING_NS_CC;
using namespace KxServer;

class CClient : public IBaseModule,public CBaseServer
{
private:
	CClient();
	virtual ~CClient();
protected:
	virtual bool CClientInit();
	virtual void CClientUnInit();

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
	CTCPListener * m_Listener;
	CTCPConnector* m_Connecter;

};