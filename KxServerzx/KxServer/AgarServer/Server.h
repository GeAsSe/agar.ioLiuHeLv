#pragma once

#include "Singleton.h"
#include "KXServer.h"
using namespace KxServer;


class CClientCallback : public IBaseModule
{
public:
    CClientCallback();

private:
	virtual int RequestLen(char* buffer, unsigned int len)override;
    virtual void Process(char* buffer, unsigned int len, ICommunication *target) override;

private:
    int CreatePlayerID();

private:
    int m_nNextPlayerID;
};


class CServer : public CBaseServer
{
    DECLARE_SINGLETON(CServer)

private:
    CServer();
    virtual ~CServer();

protected:
    virtual bool ServerInit() override;
    virtual void ServerUninit() override;

private:
    CTCPListener*               m_Listener;
    CClientCallback             m_client;
	CTCPConnector*              m_Connecter;
};
