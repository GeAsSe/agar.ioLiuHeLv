#include "Server.h"
#include "Log.h"
#include "Game.h"
#include "XmlParser.h"
#include<sstream>

CClientCallback::CClientCallback()
{
	m_nNextPlayerID = 1;
}

int CClientCallback::RequestLen(char *buffer, unsigned int len)
{
    // ����Э�鳤��
    if (len < sizeof(int))
    {
        return sizeof(int);
    }
    else
    {
        // �ײ����ư���󳤶�Ϊ MAX_PKGLEN 1<<16
        return *(int*)(buffer);
    }
}

void CClientCallback::Process(char *buffer, unsigned int len, ICommunication *target)
{
	DEBUG_LOG("Server recv msg:\n" << string(buffer + 4, len));
    do
    {
        // ��ȡ��Ϸʵ������
        CGame *pGame = CSingleton<CGame>::GetInstancePtr();

        CXmlParser xml;
        if (!xml.LoadContent(buffer + 4, len))
        {
            break;
        }

        // ȡ�����󷽷�
        string strMethod;
        if (!xml.GetContent("request/method", strMethod))//�ұ�ǩ���ѱ�ǩ������ݴ���������
        {
            break;
        }

        if (strMethod == "PLAYER_JOIN")
        {
            // ����һ��Ψһ����ұ��
			DEBUG_LOG("get it");
			std::string name;
			int keyID;
			xml.GetContent("request/name", name);
			xml.GetContent("request/keyID", keyID);
            int nPlayerID = CreatePlayerID();
			DEBUG_LOG("Playername:" << name << "get");
			pGame->Join(nPlayerID, target, name, keyID);
        }
		else if (strMethod == "PLAYER_DIDIVE") {
			int playerID;
			xml.GetContent("request/playerID", playerID);
			DEBUG_LOG("playerID" << playerID << "get");
			pGame->respDIVIDE(playerID);
		}
		else if (strMethod == "PLAYER_SPIT") {
			int playerID;
			int sporecount;
			xml.GetContent("request/playerID", playerID);
			xml.GetContent("request/sporecount", sporecount);
			DEBUG_LOG("sporecount" << sporecount << "get");
			pGame->respSPIT(playerID, sporecount);
		}
		else if (strMethod == "UPDATEPRICK") {
			int delNum;
			vector<int>globalIDs;
			vector<string>globalIDsstr;
			xml.GetContent("request/delnum", delNum);
			
			xml.GetContent("request/globalID", globalIDsstr);
			for (auto i : globalIDsstr) {
				globalIDs.push_back(stoi(i));
			}

			DEBUG_LOG("delnum:" << delNum << "get");
			pGame->delPrick(delNum,globalIDs);
		}
		else if (strMethod == "SYNPLAYERINFO") {
			int ID;
			int divisionNum;

			vector<double>positions;
			vector<string>positionsstr;

			vector<int>scores;
			vector<string>scoresstr;

			xml.GetContent("request/ID", ID);
			xml.GetContent("request/divisionNum",divisionNum);
			
			xml.GetContent("request/position", positionsstr);
			for (auto f : positionsstr) {
				positions.push_back(stof(f));
			}

			xml.GetContent("request/scores", scoresstr);
			for (auto i : scoresstr) {
				scores.push_back(stoi(i));
			}

			DEBUG_LOG("ID:" << ID << "get");

			pGame->SYNPLAYERINFO(ID, divisionNum, positions, scores);
		}
		else if (strMethod == "SYNSPOREINFO") {
			int newsporeNum;

			vector<int>globalIDs;
			vector<string>globalIDsstr;

			vector<double>positions;
			vector<string>positionsstr;
			xml.GetContent("request/newsporeNum", newsporeNum);
			
			xml.GetContent("request/globalID", globalIDsstr);
			for (auto i : globalIDsstr) {
				globalIDs.push_back(stoi(i));
			}

			xml.GetContent("request/position", positionsstr);
			for (auto f : positionsstr) {
				positions.push_back(stof(f));
			}
			pGame->SYNSPOREINFO(newsporeNum, globalIDs, positions);

		}
		else if (strMethod == "MESSAGE") {
			std::string message;
			xml.GetContent("request/message", message);
			DEBUG_LOG("message:" << message << " get");
			pGame->MESSAGE(message);
		}
		else if (strMethod == "DELSPORE") {
			int globalID;
			xml.GetContent("request/globalID", globalID);
			pGame->DELSPORE(globalID);
		}
    } while (0);
}


int CClientCallback::CreatePlayerID()
{
    return m_nNextPlayerID++;
}




CServer::CServer()
{
    DEBUG_LOG("Server created");
}

CServer::~CServer()
{
    DEBUG_LOG("Server destroyed");
}

bool CServer::ServerInit()
{
    m_Poller = new CSelectPoller();  //copy
    m_Listener = new CTCPListener(5555, NULL);              // ���������� 0.0.0.0:5555 �˿�
    m_Listener->SetClientModule(&m_client);
    m_Poller->AddPollObject(m_Listener, POLLTYPE_IN);

    return true;
}
///////////////////////////////////////////////////////
void CServer::ServerUninit()
{
    delete m_Listener;
    delete m_Poller;

    m_Listener = NULL;
    m_Poller = NULL;
}

