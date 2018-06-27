#pragma once

#include "Singleton.h"
#include "GameMap.h"
#include "Player2.h"
#include"../food.h"
#include"../Prick.h"
#include"../Player.h"
#include"../Spore.h"
#include"../PD.h"
#include"KXServer.h"
#include<sstream>



using namespace std;

class CGame :public IRepeatTimeObject
{
    DECLARE_SINGLETON(CGame)

private:
    CGame();
    ~CGame();

public:
    bool Initialize();                                              // 游戏初始化  启动定时器
	void Update();
	void collide();
	void AddPrick();
	void setseed();
	int getseed();

	void respcollide(PLAYER_ID ID1, PLAYER_ID ID2);

	void Join(PLAYER_ID playerID, ICommunication *target, std::string name, int keyID);          // 玩家加入游戏
	void respJoin(PLAYER_ID playerID, ICommunication *target);

	void respDIVIDE(PLAYER_ID playerID);

	void respSPIT(PLAYER_ID playerID ,int sporecount);

	void delPrick(int delNum, vector<int>globalIDs);

	void SYNPLAYERINFO(PLAYER_ID ID, int divisionNum, vector<double>positions, vector<int>scores);
	void UPDATAPLAYEREVENT(PLAYER_ID ID, int divisionNum, vector<double>positions, vector<int>scores);

	void SYNSPOREINFO(int newsporeNum, vector<int>globalIDs, vector<double>positions);
	
	void MESSAGE(std::string message);

	void DELSPORE(int globalID);

protected:
	virtual void OnTimer(const TimeVal& now) override;


private:
    CGameMap                         m_gameMap;                      // 游戏地图
    map<PLAYER_ID, CPlayer*>         m_cplayers;                      // 所有玩家
	map<PLAYER_ID, Player*>          m_player;                        //新所有玩家
	map<int, Spore*>                 m_spores;                      // 所有孢子
	map<int, Prick*>                 m_pricks;                      // 所有刺
	int                              seed;                          //food种子数
};
