#include "Player2.h"
#include "Log.h"
#include <sstream>
using namespace std;

CPlayer::CPlayer(PLAYER_ID playerID, ICommunication *target)
{
    m_playerID = playerID;
    m_target = target;

    DEBUG_LOG("Player[" << m_playerID << "]-Player created");
}

CPlayer::~CPlayer()
{
    DEBUG_LOG("Player[" << m_playerID << "]-Player destroyed");
}

void CPlayer::Join()
{
    // �� playerID ���ظ��ͻ��ˣ��ں����������У�����Я����ֵ
    ostringstream oss;
    oss << "<response>\n"
        << "<player_id>" 
		<< m_playerID 
		<< "</player_id>\n"
        << "</response>\n";
    string s = oss.str();
    int nLen = s.length();
    
    m_target->Send((char *)&nLen, 4);
    m_target->Send(const_cast<char *>(s.c_str()), s.length());
}

void CPlayer::Move(int x, int y)
{
}
