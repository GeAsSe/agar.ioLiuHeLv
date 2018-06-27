/*
 * ����SocketͨѶ����
 * �����˷����������ӳ٣����ͺͽ��սӿ�
 * ����Socket�����쳣
 *
 *  2013-04-12 By ��ү
 *  
 */
#ifndef __KX_SOCKET__
#define __KX_SOCKET__

#include "ICore.h"

namespace KxServer {

enum SOCKET_TYPE
{
	SOCKET_TYPETCP,
	SOCKET_TYPEUDP
};

enum SOCKET_STATE
{
	SOCKET_STATEUNINIT,					//δ��ʼ��
	SOCKET_STATEINIT,					//�ѳ�ʼ��
	SOCKET_STATEBIND,					//�Ѱ�
	SOCKET_STATECONNECTED,				//������
	SOCKET_STATELISTENING,				//���ڼ���
	SOCKET_STATEERROR					//����
};

enum SOCKET_CODE
{
	SOCKET_ERRORUNKNOW	= -9999,		//δ֪��Socket����
	SOCKET_ERRORFAILE,					//Socket����ʧ��
	SOCKET_ERRORTYPE,					//Socket���ʹ���:��TCP��UDP����UDP����TCP����
	SOCKET_ERRORSTATE,					//Socket״̬����,����δ��ʼ������connect����δlisten����accept
	SOCKET_ERRORAGAIN,					//������Ϊ�������׽���ʱ��send��recv���ݲ����������ٴ�send��recv

	SOCKET_SUCCESS		= 0				//�ɹ�����
};

class CBaseSocket
{
public:
	CBaseSocket(int socketType = SOCKET_TYPETCP);

	//����һ���Ѿ������ϵľ��
	CBaseSocket(int socketType, COMMUNICATIONID handle);
	
	virtual ~CBaseSocket(void);
	
	// ��ʼ��Socket
	int SocketInit();
	
	// ������TCPSocket�����¿��Ե���
	int SocketListen(int maxListenQueue);
	
	// ����ָ����IP�Ͷ˿�
	int SocketConnect(const char* addr, int port);
	
	// �󶨵�ָ����Ip�Ͷ˿�
	int SocketBind(const char* addr, int port);
	
	// Accept����һ��Socket���Ӷ���
	CBaseSocket* SocketAccept(void);
	
	// �ر�Socket����
	void SocketClose(void);

	
	// ���׽ӿ�д����
	int SocketSend(const char* buffer, int size);
	
	// ��������
	int SocketRecv(char* buffer, int size);

	//���õ�ǰ��SocketAddr,���SocketAddr���ᱻ����UDP����
	void SocketSetAddr(SocketAddr &name);
	
	//���õ�ǰ��SocketAddr,���SocketAddr���ᱻ����UDP����
	void SocketSetAddr(const char* ip, int port);

	//����ƽ̨�ж��Ƿ�EAGAIN����
	bool IsSocketError();

	void SocketNonBlock(bool bNonBlock);

	inline bool SocketNonBlock()
	{
		return m_bNonBlock;
	}

	//��ʹ��Ngale�㷨��ʱ����
	void SocketNonDelay();
	
	//��ȡ��ǰ��Socket��ַ,�����ַ��ÿ��UDP�����Լ�SetAddrʱ�ᱻ����
	inline SocketAddr SocketGetAddr()
	{
		return m_SockAddr;
	}

	//Get The Socket Handle
	inline COMMUNICATIONID GetSocket()
	{
		return m_Socket;
	}

private:

	//��ʼ��Addr�ṹ�壬�����˲�ͬƽ̨��ʵ��
	void SocketInitAddr(SocketAddr &name, int port, const char* ip = NULL);

private:
	COMMUNICATIONID	m_Socket;
	int				m_SocketType;
	int				m_SocketState;
	SocketAddr	    m_SockAddr;
	bool			m_bNonBlock;
};

}

#endif
