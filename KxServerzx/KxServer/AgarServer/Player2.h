#pragma once

#include "KXServer.h"
using namespace KxServer;

// 定义玩家ID数据类型
typedef int PLAYER_ID;

class CPlayer
{
public:
    CPlayer(PLAYER_ID playerID, ICommunication *target);
    ~CPlayer();
    
public:
    void Join();
    void Move(int x, int y);

private:
    PLAYER_ID               m_playerID;
    ICommunication*         m_target;
};

