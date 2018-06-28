#include "client.h"
/*#include"XmlParser.h"*/
#include"Classes\Web\XmlParser.h"
#include"Classes\GameScene\GameLayer.h"
#include<vector>
#include<string>
#include"Classes\Web\Singleton.h"
#include"proj.win32\nameancolor.h"

CClient::CClient()
{

}


CClient::~CClient()
{

}

bool CClient::ClientInit()
{
	m_Poller = new CSelectPoller();
	m_Connecter = new CTCPConnector("192.168.31.175", 5555, m_Poller);
	m_Connecter->SetModule(this);
	m_Poller->AddPollObject(m_Connecter, POLLTYPE_IN);

	return true;
}

void CClient::ClientUnInit()
{
	delete m_Poller;

	m_Poller = NULL;
}

int CClient::RequestLen(char * buffer, unsigned int len)
{
	if (len < sizeof(int))
	{
		return sizeof(int);
	}
	else
	{
		return 4 + *(int*)(buffer);
	}
}

void CClient::Process(char * buffer, unsigned int len, ICommunication * target)
{
	do
	{
		//GameLayer *layer = CSingleton<GameLayer>::GetInstancePtr();
		auto pnnc = CSingleton<nameandcolor>::GetInstancePtr();
		auto layer = pnnc->layer;
		CXmlParser xml;
		if (!xml.LoadContent(buffer + 4, len - 4))
		{
			break;
		}

		// 取出请求方法
		std::string strMethod;
		if (!xml.GetContent("info/method", strMethod))//找标签，把标签里的内容传到容器里
		{
			break;
		}

		
		if (strMethod == "PLAYERDIVIDE")
		{
			int playerID;
			xml.GetContent("info/playerID", playerID);
			layer->playerDivideEvent(playerID);
		}
		else if (strMethod == "SPITSPORERESULT") {
			int playerID;
			int globalID;
			int sporecount;
			xml.GetContent("info/playerID", playerID);
			xml.GetContent("info/globalID", globalID);
			xml.GetContent("info/sporecount", sporecount);
			layer->spitSporeResultEvent(playerID, globalID, sporecount);
		}
		else if (strMethod == "ADDPRICK") {
			int globalID;
			int x;
			int y;
			xml.GetContent("info/globalID", globalID);
			xml.GetContent("info/x", x);
			xml.GetContent("info/y", y);
			layer->addPrickEvent(globalID, x, y);
		}
		else if (strMethod == "ENTERPLAYER") {
			int playerID;
			std::string playername;
			int KeywordID;
			int divisionNum; 
			std::vector<double>position; 
			std::vector<std::string>positionStr;
			std::vector<int>scores;
			std::vector<std::string>scoresStr;
	

			xml.GetContent("info/playerID", playerID);
			xml.GetContent("info/playername", playername);
			xml.GetContent("info/keyID",KeywordID);
			xml.GetContent("info/divisionNum", divisionNum);

			xml.GetContent("info/position", positionStr);
			for (auto f : positionStr)
			{
				position.push_back(stof(f));
			}

			xml.GetContent("info/scores", scoresStr);
			for (auto i : scoresStr)
			{
				scores.push_back(stoi(i));
			}
		
			layer->enterPlayerEvent( playerID,  playername,  KeywordID,  divisionNum,position, scores);
		}
		else if (strMethod == "UPDATEPLAYER") {
			int playerID;
			int divisionNum;
			std::vector<double>position;
			std::vector<std::string>positionStr;
			std::vector<int> scores;
			std::vector<std::string>scoresStr;
			xml.GetContent("info/playerID", playerID);
			xml.GetContent("info/divisionNum", divisionNum);

			xml.GetContent("info/position", positionStr);
			for (auto f : positionStr)
			{
				position.push_back(stof(f));
			}
			
			xml.GetContent("info/scores", scoresStr);
			for (auto i : scoresStr)
			{
				scores.push_back(stoi(i));
			}

			layer->updatePlayerEvent(playerID, divisionNum, position, scores);
		}
		else if (strMethod == "MESSAGE") {
			std::string message;
			xml.GetContent("info/message", message);
			auto pnnc = CSingleton<nameandcolor>::GetInstancePtr();
			pnnc->mess->pushmessage(message);
		}
		else if (strMethod == "COLLIDE") {
			layer->collide();
		}
		else if (strMethod == "START") {
			//food的初始化数据
			int seed;
			xml.GetContent("info/seed", seed);
			layer->initFood(seed);
			//孢子的初始化数据
			int sporeNum;
			std::vector<int>sporeglobalIDs;
			std::vector<std::string>sporeglobalIDsStr;
			std::vector<double> sporeposition;
			std::vector<std::string>sporepositionStr;
            xml.GetContent("info/sporenum", sporeNum);
			xml.GetContent("info/sporeglobalID", sporeglobalIDsStr);
			for (auto i : sporeglobalIDsStr)
			{
				sporeglobalIDs.push_back(stoi(i));
			}
			xml.GetContent("info/sporeposition", sporepositionStr);
			for (auto f : sporepositionStr)
			{
				sporeposition.push_back(stof(f));
			}
			layer->initSpore(sporeNum, sporeglobalIDs, sporeposition);
			//刺的初始化
			int prickNum;
			std::vector<int> prickglobalIDs;
			std::vector<std::string> prickglobalIDsStr;
			std::vector<double>prickposition;
			std::vector<std::string> prickpositionStr;
			xml.GetContent("info/prickNum", prickNum);
			xml.GetContent("info/prickglobalID", prickglobalIDsStr);
			for (auto i : prickglobalIDsStr)
			{
				prickglobalIDs.push_back(stoi(i));
			}
			xml.GetContent("info/prickposition", prickpositionStr);
			for (auto f : prickpositionStr)
			{
				prickposition.push_back(stof(f));
			}
			layer->initPrick( prickNum, prickglobalIDs, prickposition);
			//rival的初始化
			int rivalNum;

			std::vector<int>rivalIDs;
			std::vector<std::string>rivalIDsStr;

			std::vector<std::string>rivalnames;

			std::vector<int> rivalKeywordIDs;
			std::vector<std::string>rivalKeywordIDsStr;

			std::vector<int>rivaldivisionNums;
			std::vector<std::string>rivaldivisionNumsStr;

			std::vector<double>rivalpositions; 
			std::vector<std::string>rivalpositionsStr;

			std::vector<int>rivalscores;
			std::vector<std::string>rivalscoresStr;
			

			xml.GetContent("info/rivalNum", rivalNum);
			
			xml.GetContent("info/rivalID", rivalIDsStr);
			for (auto i : rivalIDsStr)
			{
				rivalIDs.push_back(stoi(i));
			}

			xml.GetContent("info/rivalname", rivalnames);


			xml.GetContent("info/rivalKeywordID", rivalKeywordIDsStr);
			for (auto i : rivalKeywordIDsStr)
			{
				rivalKeywordIDs.push_back(stoi(i));
			}

			xml.GetContent("info/rivaldivisionNum", rivaldivisionNumsStr);
			for (auto i : rivaldivisionNumsStr)
			{
				rivaldivisionNums.push_back(stoi(i));
			}
		
			xml.GetContent("info/rivalposition", rivalpositionsStr);
			for (auto f : rivalpositionsStr)
			{
				rivalpositions.push_back(stof(f));
			}
			
			xml.GetContent("info/rivalscores", rivalscoresStr);
			for (auto i : rivalscoresStr)
			{
				rivalscores.push_back(stoi(i));
			}

			layer->initRival(rivalIDs,
				rivalNum, rivalnames, rivalKeywordIDs,
				rivaldivisionNums, rivalpositions, rivalscores);
			//player的初始化；
			int ID;
			std::string name;
			int KeywordID;
			int divisionNum;

			std::vector<double>positions;
			std::vector<std::string>positionsStr;

			std::vector<int>scores;
			std::vector<std::string>scoresStr;

			xml.GetContent("info/ID", ID);
			xml.GetContent("info/name", name);
			xml.GetContent("info/KeywordID", KeywordID);
			xml.GetContent("info/divisionNum", divisionNum);
			
			xml.GetContent("info/position", positionsStr);
			for (auto f : positionsStr)
			{
				positions.push_back(stof(f));
			}

			xml.GetContent("info/scores", scoresStr);
			for (auto i : scoresStr)
			{
				scores.push_back(stoi(i));
			}
			
			layer->initPlayer(ID, name, KeywordID, divisionNum, positions, scores);
		}

	} while (0);
}

void CClient::resetPoller()
{
	m_Poller->Poll();
}

void CClient::Send(char * buff, int len)
{
	m_Connecter->Send(buff, len);
}
