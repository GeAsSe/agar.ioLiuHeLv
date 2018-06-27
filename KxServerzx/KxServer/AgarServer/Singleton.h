/*
 * 文件: Singleton.h
 * 功能: 单例模板类
 * 用法: 
 *       1. class CTest 的构造函数和析构函数必须设置为私有或保护型
 *       2. 在 class CTest 类中将 CSingleton<CTest> 类声明为友元类，使用宏 DECLARE_SINGLETON(CTest)
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
