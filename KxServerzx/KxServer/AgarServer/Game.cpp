#include "Game.h"
#include "Log.h"




CGame::CGame()
{
    DEBUG_LOG("Game created");
}

CGame::~CGame()
{
    DEBUG_LOG("Game destroyed");
}

bool CGame::Initialize()
{
	setseed();
	Update();
    DEBUG_LOG("Game initialized OK");
    return true;
}

static long long int rate = 0;
void CGame::Update()
{
	IRepeatTimeObject::Init(0.1f, 0x7fffffff);

	TimeVal tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	CTimerManager::GetInstance()->AttachTimerWithFixTime(tv, this);

}

void CGame::collide()
{
	if (m_player.size() == 0) {
		return;
	}
	for (auto p1 : m_player) {
		for (auto p2 : m_player) {
			if (p1.first >= p2.first) {
				continue;
			}
			if (p1.second->collideRival(p2.second) == true) {
				respcollide(p1.first, p2.first);
			}
			if (p1.second->getDivisionNum() == 0) {
				m_player.erase(p1.first);
			}
			if (p2.second->getDivisionNum() == 0) {
				m_player.erase(p2.first);
			}
		}
	}
}

void CGame::respcollide(PLAYER_ID ID1, PLAYER_ID ID2)
{
	auto player1 = m_player[ID1];
	auto player2 = m_player[ID2];
	ICommunication *target1 = player1->get_target();
	ICommunication *target2 = player2->get_target();
	ostringstream oss;
	oss << "<info><method>COLLIDE</method></info>";
	string s = oss.str();
	int nLen = s.length();
	target1->Send((char *)&nLen, 4);
	target1->Send(const_cast<char *>(s.c_str()), s.length());
	target2->Send((char *)&nLen, 4);
	target2->Send(const_cast<char *>(s.c_str()), s.length());
}

void CGame::AddPrick()
{
	static int id = 0;
	int xPosition = rand() % 2350;
	int yPosition = rand() % 1250;
	/*
	Prick prick;
	prick.setPrick(xPosition, yPosition);
	m_pricks.insert(make_pair(id, &prick));
	id++;
	*/
	Prick *prick = new Prick(xPosition, yPosition);
	m_pricks[id] = prick;
	ostringstream oss;
	oss << "<info><method>ADDPRICK</method>"
		<< "<globalID>" << id << "</globalID>"
		<< "<x>" << xPosition << "</x>"
		<< "<y>" << yPosition << "</y>"
		<< "</info>";
	string s = oss.str();
	int nLen = s.length();
	id++;
	DEBUG_LOG("prick added");
	//下面要把生成的此PRICK信息发给所有玩家
	if (m_player.size() == 0) {
		return;
	}
	for (auto player : m_player) {
		ICommunication *target = player.second->get_target();
		if (player.second == NULL) {
			continue;
		}
		target->Send((char *)&nLen, 4);
		target->Send(const_cast<char *>(s.c_str()), s.length());
	}
}

// 功能: 玩家加入游戏，需要将其放入地图，坐标随机
//
void CGame::Join(PLAYER_ID playerID, ICommunication *target, std::string name, int keyID)
{
	Player *player = new Player;
	*player = *player->create(name, keyID, playerID, target);
	int x = rand() % 2400;          //将来考虑生成在无单位处
	int y = rand() % 1800;
	*player->createDivision(x, y, 10);
	m_player.insert(make_pair(playerID, player));

	respJoin(playerID, target);

}

void CGame::respJoin(PLAYER_ID playerID, ICommunication * target)
{
	for (auto player : m_player) {
		ICommunication *m_target = player.second->get_target();
		if (m_target == NULL) {
			continue;
		}
		if (m_target == target) {
			ostringstream oss;
			oss << "<info><method>START</method>"
				<< "<seed>" << seed << "</seed>"

				<< "<sporenum>" << m_spores.size() << "</sporenum>";
			for (auto spore : m_spores) {
				oss << "<sporeglobalID>" << spore.first << "</sporeglobalID>";
				}
			for (auto spore : m_spores) {
				oss << "<sporeposition>" << spore.second->get_x() << "</sporeposition>"
					<< "<sporeposition>" << spore.second->get_y() << "</sporeposition>";
			}
			oss << "<prickNum>" << m_pricks.size() << "</prickNum>";
			for (auto prick : m_pricks) {
				oss << "<prickglobalID>" << prick.first << "</prickglobalID>";
			}
			for (auto prick : m_pricks) {
				oss << "<prickposition>" << prick.second->get_x() << "</prickposition>"
					<< "<prickposition>" << prick.second->get_y() << "</prickposition>";
			}

			oss << "<rivalNum>" << m_player.size() - 1 << "</rivalNum>";
			for (auto player : m_player) {
				if (player.first != playerID) {
					oss << "<rivalID>" << player.first << "</rivalID>";
				}
			}
			for (auto player : m_player) {
				if (player.first != playerID) {
					oss << "<rivalname>" << player.second->getPlayername() << "</rivalname>";
				}
			}
			for (auto player : m_player) {
				if (player.first != playerID) {
					oss << "<rivalKeywordID>" << player.second->get_keyID() << "</rivalKeywordID>";
				}
			}
			for (auto player : m_player) {
				if (player.first != playerID) {
					oss << "<rivaldivisionNum>" << player.second->getDivisionNum()<< "</rivaldivisionNum>";
				}
			}
			for (auto player : m_player) {
				if (player.first != playerID) {
					for (auto division : player.second->getDivisionList()) {
						oss << "<rivalposition>" << division->get_x() << "</rivalposition>"
							<< "<rivalposition>" << division->get_y() << "</rivalposition>";
					}
				}
			}
			for (auto player : m_player) {
				if (player.first != playerID) {
					for (auto division : player.second->getDivisionList()) {
						oss << "<rivalscores>" << division->getScore() << "</rivalscores>";
					}
				}
			}
			oss << "<ID>" << playerID << "</ID>";
			auto self = m_player[playerID];
			oss << "<name>" << self->getPlayername() << "</name>"
				<< "<KeywordID>" << self->get_keyID() << "</KeywordID>"
				<< "<divisionNum>" << self->getDivisionNum() << "</divisionNum>";
			for (auto division : self->getDivisionList()) {
				oss << "<position>" << division->get_x() << "</position>"
					<< "<position>" << division->get_y() << "</position>";
			}
			for (auto division : self->getDivisionList()) {
				oss << "<scores>" << division->getScore() << "</scores>";
			}
			oss << "</info>";
			string s = oss.str();
			int nLen = s.length();
			target->Send((char *)&nLen, 4);
			target->Send(const_cast<char *>(s.c_str()), s.length());
		}
		else {
			auto p = m_player[playerID];
			ostringstream oss;
			oss << "<info><method>ENTERPLAYER</method>"
				<< "<playerID>" << playerID << "</playerID>"
				<< "<playername>" << p->getPlayername() << "</playername>"
				<< "<KeyID>" << p->get_keyID() << "</KeyID>"
				<< "<divisionNum>" << p->getDivisionNum() << "</divisionNum>";
			for (auto division : p->getDivisionList()) {
				oss << "<position>" << division->get_x() << "</position>"
					<< "<position>" << division->get_y() << "</position>";
			}
			for (auto division : p->getDivisionList()) {
				oss << "<scores>" << division->getScore() << "</scores>";
			}
			oss << "</info>";
			string s = oss.str();
			int nLen = s.length();
			m_target->Send((char *)&nLen, 4);
			m_target->Send(const_cast<char *>(s.c_str()), s.length());
		}
	}
}

void CGame::respDIVIDE(PLAYER_ID playerID)
{
	for (auto player : m_player) {
		if (player.first != playerID) {
			ICommunication * target = player.second->get_target();
			ostringstream oss;
			oss << "<info><method>PLAYERDIVIDE</method>"
				<< "<playerID>" << playerID << "</playerID>"
				<< "</info>";
			string s = oss.str();
			int nLen = s.length();
			target->Send((char *)&nLen, 4);
			target->Send(const_cast<char *>(s.c_str()), s.length());
		}
	}
}

void CGame::respSPIT(PLAYER_ID playerID, int sporecount)
{
	ostringstream oss;
	oss << "<info><method>SPITSPORERESULT</method>"
		<< "<playerID>" << playerID << "</playerID>";
	int globalID = 0;
	for (auto sporeItem : m_spores) {
		if (sporeItem.first >= globalID) {
			globalID = sporeItem.first;
		}
	}
	oss << "<globalID>" << globalID + 1<< "</globalID>"
		<< "<sporecount>" << sporecount << "</sporecount>"
		<< "</info>";
	string s = oss.str();
	int nLen = s.length();
	for (auto player : m_player) {
		ICommunication * target = player.second->get_target();
		target->Send((char *)&nLen, 4);
		target->Send(const_cast<char *>(s.c_str()), s.length());
	}
}


//
// 说明: 只是简单说明 Move 参数，实际还需要考虑其所有子球的移动
//
/*
void CGame::Move(PLAYER_ID playerID, int x, int y)
{
    m_players[playerID]->Move(x, y);
}
*/






















void CGame::setseed()
{
	seed = rand() + 1;
}

int CGame::getseed()
{
	return seed;
}

void CGame::delPrick(int delNum, vector<int> globalIDs)
{
	for (int i = 0; i < delNum; i++) {
		m_pricks.erase(globalIDs[i]);
	}
}

void CGame::SYNPLAYERINFO(PLAYER_ID ID, int divisionNum, vector<double> positions, vector<int> scores)
{
	auto player = m_player.at(ID);
	if (player != NULL) {
		int Oldsize = player->getDivisionNum();
		int newSize = divisionNum;
		if (Oldsize != newSize) {
			if (newSize == 0) {
				m_player.erase(ID);
			}
			else {
				player->clearDivisionList();
				for (int i = 0, j = 0; i < newSize; i++) {
					double x = positions[j];
					double y = positions[j + 1];
					int score = scores[i];
					auto division = player->createDivision(x, y, score);
					j += 2;
				}
			}
		}
		else {
			vector<PD*>divisionList = player->getDivisionList();
			int i = 0, j = 0;
			for (auto division : divisionList) {
				double x = positions[j];
				double y = positions[j + 1];
				int score = scores[i];
				if (division != NULL) {
					division->setPosition(x, y);
					division->setScore(score);
				}
				i++;
				j += 2;
			}
		}
		UPDATAPLAYEREVENT(ID, divisionNum, positions, scores);
	}
	
}

void CGame::UPDATAPLAYEREVENT(PLAYER_ID ID, int divisionNum, vector<double> positions, vector<int> scores)
{
	ostringstream oss;
	oss << "<info><method>UPDATEPLAYER</method>"
		<< "<playerID>" << ID << "</playerID>"
		<< "<divisionNum>" << divisionNum << "</divisionNum>";
	for (auto p : positions) {
		oss << "<position>" << p << "</position>";
	}
	for (auto s : scores) {
		oss << "<scores>" << s << "</scores>";
	}
	oss << "</info>";
	string s = oss.str();
	int nLen = s.length();
	for (auto player: m_player) {
		if (player.first != ID) {
			ICommunication *target = player.second->get_target();
			target->Send((char *)&nLen, 4);
			target->Send(const_cast<char *>(s.c_str()), s.length());
		}
	}
}

void CGame::SYNSPOREINFO(int newsporeNum, vector<int> globalIDs, vector<double> positions)
{
	for (int i = 0, j = 0; i < newsporeNum; i++) {
		if (m_spores.find(globalIDs[i]) == m_spores.end()) {
			Spore * spore = new Spore;
			spore->setPosition(positions[j], positions[j + 1]);
			m_spores[globalIDs[i]] = spore;
		}
		else {
			auto spore = m_spores.at(globalIDs[i]);
			spore->setPosition(positions[j], positions[j + 1]);
		}
		j += 2;
	}
}

void CGame::MESSAGE(std::string message)
{
	ostringstream oss;
	oss << "<info><method>MESSAGE</method>"
		<< "<message>" << message << "</message>"
		<< "</info>";
	string s = oss.str();
	int nLen = s.length();
	for (auto p: m_player) {
		if (p.second != NULL) {
			ICommunication * target = p.second->get_target();
			if (target != NULL) {
				target->Send((char *)&nLen, 4);
				target->Send(const_cast<char *>(s.c_str()), s.length());
			}
		}
	}
}

void CGame::DELSPORE(int globalID)
{
	m_spores.erase(globalID);
}

void CGame::OnTimer(const TimeVal &now)
{
	collide();
	if (rate % 50 == 0) {
		AddPrick();
	}
	rate++;
	IRepeatTimeObject::OnTimer(now);

}