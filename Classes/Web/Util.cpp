#include "Util.h"

/*
* 函数: Explode
* 功能: 将源字符串以 lpszDelimiter 为分界拆解成若干个子串，保存在 subList 中
* 参数:
*       [in] lpszSrc       源字符串
*       [in] lpszDelimiter 分界串
*       [out]subList       用于保存输出的子串列表
* 返回: N/A
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
        // 如果字符串是以分隔串结尾，则需要加上一个空串
        subList.push_back("");
    }
}
