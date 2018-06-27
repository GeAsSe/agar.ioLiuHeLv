#include "Server.h"
#include "Game.h"
#include "Log.h"

int main(int argc, char *argv[])
{
    do
    {
        CGame *pGame = CSingleton<CGame>::GetInstancePtr();
        if (!pGame->Initialize())
        {
            ERROR_LOG("Failed to initialize game");
            break;
        }

        CServer *pServer = CSingleton<CServer>::GetInstancePtr();
        pServer->ServerStart();
    } while (0);

    DEBUG_LOG("Server is exiting");
    return 0;
}
