#include "Util.h"

/*
* ����: Explode
* ����: ��Դ�ַ����� lpszDelimiter Ϊ�ֽ�������ɸ��Ӵ��������� subList ��
* ����:
*       [in] lpszSrc       Դ�ַ���
*       [in] lpszDelimiter �ֽ紮
*       [out]subList       ���ڱ���������Ӵ��б�
* ����: N/A
*/
void Util::Explode(const char *lpszSrc, const char *lpszDelimiter, std::list<std::string> &subList)
{
    int nDelimiterLen = strlen(lpszDelimiter);
    const char *lpszFind;

    while (*lpszSrc != '\0')
    {
        lpszFind = strstr(lpszSrc, lpszDelimiter);
        if (NULL == lpszFind)
        {
            subList.push_back(lpszSrc);
            break;
        }

        int nSubLen = lpszFind - lpszSrc;
        if (0 == nSubLen)
        {
            subList.push_back("");
        }
        else
        {
            std::string strText(lpszSrc, lpszFind - lpszSrc);
            subList.push_back(strText);
        }

        lpszSrc = lpszFind + nDelimiterLen;
    }

    if (*lpszSrc == '\0')
    {
        // ����ַ������Էָ�����β������Ҫ����һ���մ�
        subList.push_back("");
    }
}
