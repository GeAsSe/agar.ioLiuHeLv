#include "client.h"
#include"XmlParser.h"
#include"/Cgame/Classes/GameScene/GameLayer.h"
#include<vector>
#include<string>
#include"/Cgame/Classes/Web/Singleton.h"

CClient::CClient()
{

}


CClient::~CClient()
{

}

bool CClient::CClientInit()
{
	m_Connecter = new CTCPConnector("192.168.31.175", 5555, m_Poller);
	m_Connecter->SetModule(new CClient());
	m_Poller->AddPollObject(m_Connecter, POLLTYPE_IN);

	// TODO: 客户端加入游戏
	char buff[128];
	memset(buff, 0, sizeof(buff));
	char* text = "<request><method>PLAYER_JOIN</method></request>\n";
	int len = 4 + strlen(text) + 1;
	*(int*)(buff) = len;
	strcpy(buff + 4, text);

	m_Connecter->Send(buff, len);

	return true;
}

int CClient::RequestLen(char * buffer, unsigned int len)
{
	if (len < sizeof(int))
	{
		return sizeof(int);
	}
	else
	{
		return *(int*)(buffer);
	}
}

void CClient::Process(char * buffer, unsigned int len, ICommunication * target)
{
	do
	{
		GameLayer *layer = CSingleton<GameLayer>::GetInstancePtr();
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

		if (strMethod == "PLAYERMOVE")
		{
			int playerID;
			int PDnum;
			std::vector<double> positions;
			std::string position;
			xml.GetContent("info/playerID", playerID);
			xml.GetContent("info/PD", PDnum);
			for (int i = 0; i < PDnum*2; i++) {
				xml.GetContent("info/position", position);
				positions.push_back(atof(position));
			}
			layer->playerMoveEvent(playerID, positions);
		}
		else if (strMethod == "PLAYERDIVIDE")
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
			std::vector<int>scores;
			int score;
			xml.GetContent("info/playerID", playerID);
			xml.GetContent("info/playername", playername);
			xml.GetContent("info/keyID",KeywordID);
			xml.GetContent("info/divisionNum", divisionNum);
			for (int i = 0; i < divisionNum; i++) {
				xml.GetContent("info/scores", score);
				scores.push_back(score);
			}
			//double不会传
			layer->enterPlayerEvent( playerID,  playername,  KeywordID,  divisionNum,position, scores);
		}
		else if (strMethod == "UPDATEPLAYER") {
			int playerID;
			int divisionNum;
			int score;
			std::vector<double>position;
			std::vector<int> scores;
			xml.GetContent("info/playerID", playerID);
			xml.GetContent("info/divisionNum", divisionNum);
			for (int i = 0; i < divisionNum; i++) {
				xml.GetContent("info/scores", score);
				scores.push_back(score);
			}
			//double不会接；
			layer->updatePlayerEvent(playerID, divisionNum, position, scores);
		}
		else if (strMethod == "MESSAGE") {
			std::string message;
			xml.GetContent("info/message", message);
		}
		else if (strMethod == "COLLIDE") {
			//通知运行collide函数
			layer->collide();
		}
		else if (strMethod == "START") {
			//food的初始化数据
			int seed;
			xml.GetContent("info/seed", seed);
			layer->initFood(seed);
			//孢子的初始化数据
			int sporeNum,
			std::vector<int>sporeglobalIDs;
			int sporeglobalID;
			std::vector<double> sporeposition;
			xml.GetContent("info/sporenum", sporeNum);
			for (int i = 0; i < sporeNum; i++) {
				xml.GetContent("info/sporeglobalID", sporeglobalID);
				sporeglobalIDs.push_back(sporeglobalID);
			}
			//double不会写
			layer->initSpore(sporeNum, sporeglobalIDs, sporeposition);
			//刺的初始化
			int prickNum;
			std::vector<int> prickglobalIDs;
			int prickglobalID;
			std::vector<double>position;
			xml.GetContent("info/prickNum", prickNum);
			for (int i = 0; i < prickNum; i++) {
				xml.GetContent("info/prickglobalID", prickglobalID);
				prickglobalIDs.push_back(prickglobalID);
			}
			//double不会写
			layer->initPrick( prickNum, prickglobalIDs, position);
			//rival的初始化
			int rivalNum;
			std::vector<int>rivalIDs;
			int rivalID;
			std::vector<std::string>rivalnames;
			std::vector<int> rivalKeywordIDs;
			int rivalKeywordID;
			std::vector<int>rivaldivisionNums;
			int rivaldivisionNum;

			int totalpdNum = 0;
			std::vector<double>rivalpositions; 
			std::vector<int>rivalscores;
			int rivalscore;

			xml.GetContent("info/rivalNum", rivalNum);
			for (int i = 0; i < rivalNum; i++)
			{
				xml.GetContent("info/playerID", rivalID);
				rivalIDs.push_back(rivalID);
			}
			xml.GetContent("info/playername", rivalnames);
			for (int i = 0; i < rivalNum; i++)
			{
				xml.GetContent("info/KeywordID", rivalKeywordID);
				rivalKeywordIDs.push_back(rivalKeywordID);
			}
			for (int i = 0; i < rivalNum; i++)
			{
				xml.GetContent("info/divisionNum", rivaldivisionNum);
				rivaldivisionNums.push_back(rivaldivisionNum);
			}
			//double不会写；
			for (int i = 0; i < rivalNum; i++) {
				totalpdNum += rivaldivisionNums[i];
			}

			for (int i = 0; i < totalpdNum; i++)
			{
				xml.GetContent("info/rivalscores", rivalscore);
				rivalscores.push_back(rivalscore);
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
			std::vector<int>scores;
			int score;
			xml.GetContent("info/ID", ID);
			xml.GetContent("info/name", name);
			xml.GetContent("info/KeywordID", KeywordID);
			xml.GetContent("info/divisionNum", divisionNum);
			//double不会写；
			for (int i = 0; i < divisionNum; i++) {
				xml.GetContent("info/scores", score);
				scores.push_back(score);
			}
			layer->initPlayer(ID, name, KeywordID, divisionNum, positions, scores);
		}

	} while (0);
}
