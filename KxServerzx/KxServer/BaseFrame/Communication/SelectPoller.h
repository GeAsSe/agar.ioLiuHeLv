/*
 * ʵ�ֿ�ƽ̨��Select
 * ��ÿ��Poll��ѯ��ʱ�򣬻Ὣ�쳣�Ķ�����ӵ�ɾ���б���
 * ����ѯ��֮��һ����ɾ��
 *
 *  2013-04-20 By ��ү
 *  
 */
#ifndef __SELECTPOLLER_H__
#define __SELECTPOLLER_H__

#include <set>

#include "ICore.h"

namespace KxServer {

class CSelectPoller :
	public ICommunicationPoller
{
public:
	CSelectPoller(void);
	virtual ~CSelectPoller(void);

	//poll all communication object, and process event
	virtual int Poll();

	virtual int AddPollObject(ICommunication* obj, int type);

	virtual int ModifyPollObject(ICommunication* obj, int type);
	
    virtual int RemovePollObject(ICommunication* obj);

private:
	void Clear();

private:
	fd_set	m_InSet;
	fd_set	m_OutSet;
	TimeVal m_TimeOut;
	int		m_MaxCount;

	std::set<ICommunication*> m_AllSet;
    std::set<ICommunication*> m_RemoveSet;
    std::set<COMMUNICATIONID> m_RemoveIdSet;
};

}

#endif
