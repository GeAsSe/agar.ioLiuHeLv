/*
 * ���������࣬����������ĳ�ʼ�������е���Ϊ
 *
 *  2013-04-20 By ��ү
 *  
 */
#ifndef __BASESERVER_H__
#define __BASESERVER_H__

#include "ICore.h"

namespace KxServer {

class CBaseServer
{
public:
	CBaseServer(void);
	virtual ~CBaseServer(void);

	//start server
	virtual void ServerStart();

	//server run
	virtual void ServerRun();

	//server init
	virtual bool ServerInit();

	//server uninit
	virtual void ServerUninit();

protected:
	ICommunicationPoller*	m_Poller;
	bool					m_IsRunning;
};

}

#endif
