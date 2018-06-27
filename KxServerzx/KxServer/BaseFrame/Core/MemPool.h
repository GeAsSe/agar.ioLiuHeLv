/* 
*   MemPool �ڴ��
*   �����ڴ���Ƭ����߷����ڴ�Ч�ʣ��ڴ渴��
*   
*   2013-04-14 By ��ү
*
*/
#ifndef __MEMPOOL_H__
#define __MEMPOOL_H__

#include <set>
#include <map>
#include <list>

namespace KxServer {

typedef std::map<unsigned int, std::list<void*>*>	    MemML;
typedef std::map<unsigned int, std::set<void*>*>		MemMS;

class CMemPool
{
public:
	CMemPool(void);
	virtual ~CMemPool(void);

	// �����СΪsize���ڴ�,ʵ�ʷ����ڴ��С���ܻ����
	void* MemAlocate(unsigned int size);

	// �����ڴ�,��MemPool������ڴ����,���յ��ڴ��СΪsize,�ɹ�����0
	int MemRecycle(void* mem, unsigned int size);

	//��ʾ�ڴ�ص�ǰ״̬
	void MemDumpInfo();

private:
	
    //����Ҫ������ڴ���С
	inline unsigned int MemFitSize(unsigned int size);

	//��ȡһ���Է������������
	inline unsigned int MemFitCounts(unsigned int size);

	//��չ�ڴ��
	int MemExtend(unsigned int size, std::list<void*>* plist, std::set<void*>* pset);

	//��չ���ڴ��
	int MemExtendNewSize(unsigned int size);

	//�ͷ�ָ����С���ڴ�
	int MemReleaseWithSize(unsigned int size);

	//��Ҫ�ͷŶ��ٿ��ڴ�
	unsigned int MemRelsaseCount(unsigned int size, unsigned int freecount, unsigned int stubcount);

	//���ݵ�ǰˮλ��״̬�Զ�����Ƿ��ͷ��ڴ�
	int MemAutoRelease(unsigned size, std::list<void*>* plist, std::set<void*>* pset);


private:

	MemML			m_Free;				                    //�����ڴ��
	MemMS			m_Stub;				                    //�ڴ����

	unsigned int	m_AlocatedSize;		                    //�ѷ����С
	unsigned int	m_WaterMark;		                    //���ˮλ
	unsigned int	m_MinAlocateSize;	                    //������С�ڴ��СΪ 1 << m_MinAlocateSize

	static const unsigned int MAX_WATER_MARK = 1 << 30;		//���ˮλ
	static const unsigned int MAX_POOL_SIZE = 1 << 31;		//�ڴ��������� 

	static const unsigned int MEM_BASE_COUNT = 32;			//ÿ�η�������
	static const unsigned int MEM_SIZE_MIN = 1 << 20;		//�ڴ��������С	1M
	static const unsigned int MEM_SIZE_MID = 1 << 26;		//�ڴ����������	64M
	static const unsigned int MEM_SIZE_BIG = 1 << 27;		//�ڴ���������� 128M

};

class CMemManager
{
public:

    static CMemManager* GetInstance();
    
    static void Destroy();

    // �����СΪsize���ڴ�,ʵ�ʷ����ڴ��С���ܻ����
    void* MemAlocate(unsigned int size);

    // �����ڴ�,��MemPool������ڴ����,���յ��ڴ��СΪsize,�ɹ�����0
    int MemRecycle(void* mem, unsigned int size);

    //��ʾ�ڴ�ص�ǰ״̬
    void MemDumpInfo();

private:

    CMemManager();

    ~CMemManager();

private:

    CMemPool* m_MemPool;

    static CMemManager* m_Instance;
};

inline void* MemMgrAlocate(unsigned int size)
{
    return CMemManager::GetInstance()->MemAlocate(size);
}

inline int MemMgrRecycle(void* mem, unsigned int size)
{
    return CMemManager::GetInstance()->MemRecycle(mem, size);
}

}

#endif