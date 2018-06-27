/*
 * XmlParser.h
 *
 *  Created on: Jun 01, 2016
 *      Author: lory
 *
 * Usage: 
 *     <name_list>
 *         <name age="28">Jimmy</name>
 *         <name age="29">Tony</name>
 *         <name age="30">Lory</name>
 *         <other>other value</other>
 *     </name_list>
 *
 *     // ����ӽڵ�����
 *     GetChildNum("name_list") => 3
 *     GetChildNum("name_list/name&0") => 0
 *
 *     // ���ָ���ڵ������
 *     GetContent("name_list/name&0", strName) => "Jimmy"
 *     GetContent("name_list/name&1", strName) => "Tony"
 *     GetContent("name_list/name") => array("Jimmy", "Tony", "Lory");
 *
 *     // ���ָ���ڵ��ָ������
 *     GetAttribute("name_list/name&2", "age", nAge) => 30
 *     GetAttribute("name_list/name&2", "age", strAge) => "30"
 */

#ifndef ___XML__PARSER__20160601___
#define ___XML__PARSER__20160601___

#include <string>
#include <list>
#include <vector>
#include "Tinyxml2.h"

// ���� XML �ڵ�·����һ���ӽڵ�
class CXmlPathNode
{
public:
    CXmlPathNode(const std::string &strNodeName);

public:
    const std::string& GetName() const { return m_strName; }
    const std::string& GetFullName() const { return m_strFullName; }
    int GetIndex() const { return m_nIndex; }

protected:
    std::string                     m_strFullName;    // XML �ڵ�ԭʼ���ƣ���: name&0/name/name&1
    std::string                     m_strName;        // XML �ڵ�ڵ�����
    int                           m_nIndex;         // XML �ڵ����ţ���ͬһ��������ͬ�����ƣ����� &0/&1/&2 ���б�ʶ
};

class CXmlParser
{
public:
    CXmlParser();
    ~CXmlParser();

public:
    bool LoadFile(const char* lpszFileName);
    bool LoadContent(const char* lpszContent, int nLen = -1);

    bool GetContent(const char* lpszPath, std::string &strValue) const;
    bool GetContent(const char* lpszPath, int &nValue) const;
    bool GetContent(const char* lpszPath, std::list<std::string> &lstValues) const;
    bool GetContent(const char* lpszPath, std::vector<std::string> &vecValues) const;
    bool GetAttribute(const char* lpszPath, const char* lpszAttribute, std::string &strValue) const;
    bool GetAttribute(const char* lpszPath, const char* lpszAttribute, int &nValue) const;

    int GetChildNum(const char* lpszPath) const;

protected:
    void ExplodePathNode(const char* lpszPath, std::list<CXmlPathNode> &lstNodes) const;
    const tinyxml2::XMLElement* LocateSiblingElement(const tinyxml2::XMLElement *pElement, const CXmlPathNode &node) const;

protected:
    tinyxml2::XMLDocument           m_doc;
};

#endif
