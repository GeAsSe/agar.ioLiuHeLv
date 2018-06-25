/*
 * �ļ�: Singleton.h
 * ����: ����ģ����
 * �÷�: 
 *       1. class CTest �Ĺ��캯��������������������Ϊ˽�л򱣻���
 *       2. �� class CTest ���н� CSingleton<CTest> ������Ϊ��Ԫ�࣬ʹ�ú� DECLARE_SINGLETON(CTest)
 *       class CTest
 *       {
 *           DECLARE_SINGLETON(CTest)
 *       private:
 *           CTest() {}
 *           ~CTest() {}
 *       public:
 *           void Test() {}
 *       };
 *
 *       CTest *ptr = CSingleton<CTest>::GetInstancePtr();
 *       ptr->Test();
 */

#ifndef ___SINGLETON__20160608___
#define ___SINGLETON__20160608___

#include <stdio.h>

#define DECLARE_SINGLETON(T)	friend class CSingleton<T>;

// Template class for creating singleton instance global classes.
template <typename T>
class CSingleton
{
private:
	CSingleton() {}
	~CSingleton() {}

public:
	static T& GetInstance();
	static T* GetInstancePtr();

private:
	static T* m_pInstance;
};

template <typename T>
T* CSingleton<T>::m_pInstance = NULL;

template <typename T>
T& CSingleton<T>::GetInstance()
{
	if (NULL == m_pInstance)
	{
		if (NULL == m_pInstance)
		{
			static T instance;
			m_pInstance = &instance;
		}
	}

	return *m_pInstance;
}

template <typename T>
T* CSingleton<T>::GetInstancePtr()
{
    T &obj = GetInstance();
    return &obj;
}

#endif //__SINGLETON_H__
