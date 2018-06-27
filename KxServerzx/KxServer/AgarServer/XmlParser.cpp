/*
 * XmlParser.cpp
 *
 *  Created on: Jun 01, 2016
 *      Author: lory
 */

#include <stdlib.h>
#include <string>
#include "XmlParser.h"
#include "Util.h"
using namespace std;

CXmlPathNode::CXmlPathNode(const std::string &strNodeName) : m_strFullName(strNodeName)
{
    do
    {
        // 查找 & 符号
        std::string::size_type nPos = strNodeName.find('&');
        if (nPos == std::string::npos)
        {
            m_strName = strNodeName;
            m_nIndex = 0;
            break;
        }

        m_strName.assign(strNodeName.c_str(), nPos);
        m_nIndex = atoi(strNodeName.c_str() + nPos + 1);
    } while (0);
}


CXmlParser::CXmlParser()
{
}

CXmlParser::~CXmlParser()
{
}

bool CXmlParser::LoadFile(const char* lpszFileName)
{
    return (m_doc.LoadFile(lpszFileName) == tinyxml2::XML_NO_ERROR);
}

bool CXmlParser::LoadContent(const char* lpszContent, int nLen)
{
    return (m_doc.Parse(lpszContent, nLen) == tinyxml2::XML_NO_ERROR);
}

bool CXmlParser::GetContent(const char* lpszPath, std::string &strValue) const
{
    bool bSuccess = false;

    do
    {
        std::list<CXmlPathNode> lstNodes;
        ExplodePathNode(lpszPath, lstNodes);
        if (lstNodes.empty())
        {
            break;
        }

        std::list<CXmlPathNode>::const_iterator it = lstNodes.begin();
        const tinyxml2::XMLElement *pElement = m_doc.FirstChildElement(it->GetName().c_str());
        pElement = LocateSiblingElement(pElement, *it);

        for (++it; it != lstNodes.end() && pElement != NULL; ++it)
        {
            pElement = pElement->FirstChildElement(it->GetName().c_str());
            if (pElement != NULL)
            {
                pElement = LocateSiblingElement(pElement, *it);
            }
        }

        if (NULL == pElement)
        {
            break;
        }

        strValue.clear();
        const char* lpszText = pElement->GetText();
        if (NULL != lpszText)
        {
            strValue = lpszText;
        }

        bSuccess = true;
    } while (0);

    return bSuccess;
}

bool CXmlParser::GetContent(const char* lpszPath, int &nValue) const
{
    std::string strValue;
    if (!GetContent(lpszPath, strValue))
    {
        return false;
    }

    nValue = stoi(strValue.c_str());
    return true;
}

bool CXmlParser::GetContent(const char* lpszPath, std::list<std::string> &lstValues) const
{
    bool bSuccess = false;

    do
    {
        std::list<CXmlPathNode> lstNodes;
        ExplodePathNode(lpszPath, lstNodes);
        if (lstNodes.empty())
        {
            break;
        }

        std::list<CXmlPathNode>::const_iterator it = lstNodes.begin();
        const tinyxml2::XMLElement *pElement = m_doc.FirstChildElement(it->GetName().c_str());
        pElement = LocateSiblingElement(pElement, *it);

        for (++it; it != lstNodes.end() && pElement != NULL; ++it)
        {
            pElement = pElement->FirstChildElement(it->GetName().c_str());
            if (pElement != NULL)
            {
                pElement = LocateSiblingElement(pElement, *it);
            }
        }

        if (NULL == pElement)
        {
            break;
        }

        lstValues.clear();
        for (; pElement != NULL; pElement = pElement->NextSiblingElement(pElement->Name()))
        {
            const char* lpszText = pElement->GetText();
            if (lpszText != NULL)
            {
                lstValues.push_back(lpszText);
            }
            else
            {
                lstValues.push_back("");
            }
        }

        bSuccess = true;
    } while (0);

    return bSuccess;
}

bool CXmlParser::GetContent(const char* lpszPath, std::vector<std::string> &vecValues) const
{
    bool bSuccess = false;

    do
    {
        std::list<CXmlPathNode> lstNodes;
        ExplodePathNode(lpszPath, lstNodes);
        if (lstNodes.empty())
        {
            break;
        }

        std::list<CXmlPathNode>::const_iterator it = lstNodes.begin();
        const tinyxml2::XMLElement *pElement = m_doc.FirstChildElement(it->GetName().c_str());
        pElement = LocateSiblingElement(pElement, *it);

        for (++it; it != lstNodes.end() && pElement != NULL; ++it)
        {
            pElement = pElement->FirstChildElement(it->GetName().c_str());
            if (pElement != NULL)
            {
                pElement = LocateSiblingElement(pElement, *it);
            }
        }

        if (NULL == pElement)
        {
            break;
        }

        vecValues.clear();
        for (; pElement != NULL; pElement = pElement->NextSiblingElement(pElement->Name()))
        {
            const char* lpszText = pElement->GetText();
            if (lpszText != NULL)
            {
                vecValues.push_back(lpszText);
            }
            else
            {
                vecValues.push_back("");
            }
        }

        bSuccess = true;
    } while (0);

    return bSuccess;
}

bool CXmlParser::GetAttribute(const char* lpszPath, const char* lpszAttribute, std::string &strValue) const
{
    bool bSuccess = false;

    do
    {
        std::list<CXmlPathNode> lstNodes;
        ExplodePathNode(lpszPath, lstNodes);
        if (lstNodes.empty())
        {
            break;
        }

        std::list<CXmlPathNode>::const_iterator it = lstNodes.begin();
        const tinyxml2::XMLElement *pElement = m_doc.FirstChildElement(it->GetName().c_str());
        pElement = LocateSiblingElement(pElement, *it);

        for (++it; it != lstNodes.end() && pElement != NULL; ++it)
        {
            pElement = pElement->FirstChildElement(it->GetName().c_str());
            if (pElement != NULL)
            {
                pElement = LocateSiblingElement(pElement, *it);
            }
        }

        if (NULL == pElement)
        {
            break;
        }

        const char* lpszText = pElement->Attribute(lpszAttribute);
        if (NULL == lpszText)
        {
            break;
        }

        strValue.clear();
        strValue = lpszText;

        bSuccess = true;
    } while (0);

    return bSuccess;
}

bool CXmlParser::GetAttribute(const char* lpszPath, const char* lpszAttribute, int &nValue) const
{
    std::string strValue;
    if (!GetAttribute(lpszPath, lpszAttribute, strValue))
    {
        return false;
    }

    nValue = stoi(strValue.c_str());
    return true;
}

int CXmlParser::GetChildNum(const char* lpszPath) const
{
    int nCount = 0;

    do
    {
        std::list<CXmlPathNode> lstNodes;
        ExplodePathNode(lpszPath, lstNodes);
        if (lstNodes.empty())
        {
            break;
        }

        std::list<CXmlPathNode>::const_iterator it = lstNodes.begin();
        const tinyxml2::XMLElement *pElement = m_doc.FirstChildElement(it->GetName().c_str());
        pElement = LocateSiblingElement(pElement, *it);

        for (++it; it != lstNodes.end() && pElement != NULL; ++it)
        {
            pElement = pElement->FirstChildElement(it->GetName().c_str());
            if (pElement != NULL)
            {
                pElement = LocateSiblingElement(pElement, *it);
            }
        }

        if (NULL == pElement)
        {
            break;
        }

        pElement = pElement->FirstChildElement();
        while (pElement != NULL)
        {
            ++nCount;
            pElement = pElement->NextSiblingElement();
        }
    } while (0);

    return nCount;
}

/*
 * 功能: 将 XML 路径解析为有序的 CXmlPathNode 节点集合
 */
void CXmlParser::ExplodePathNode(const char* lpszPath, std::list<CXmlPathNode> &lstNodes) const
{
    std::list<std::string> lstItems;
    std::list<std::string>::const_iterator it;

    Util::Explode(lpszPath, "/", lstItems);
    for (it = lstItems.begin(); it != lstItems.end(); ++it)
    {
        CXmlPathNode pathNode(*it);
        lstNodes.push_back(pathNode);
    }
}

const tinyxml2::XMLElement* CXmlParser::LocateSiblingElement(const tinyxml2::XMLElement *pElement, const CXmlPathNode &node) const
{
    int nLoopTimes = node.GetIndex();
    if (nLoopTimes < 0)
    {
        return NULL;
    }

    while (nLoopTimes-- && pElement != NULL)
    {
        pElement = pElement->NextSiblingElement(node.GetName().c_str());
    }

    return pElement;
}
