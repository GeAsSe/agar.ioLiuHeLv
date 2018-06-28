#ifndef __KXSERVER_H__
#define __KXSERVER_H__

#include "Classes\Web\Core\ICore.h"
#include "Classes\Web\Core\TimerManager.h"
#include "Classes\Web\Core\MemPool.h"
#include "Classes\Web\Core\BufferList.h"
#include "Classes\Web\Core\CommPool.h"
#include "Classes\Web\Core\Ref.h"

#include "Classes\Web\ServerBase\BaseServer.h"

#include "Classes\Web\Communication\SelectPoller.h"
#include "Classes\Web\Communication\Epoller.h"
#include "Classes\Web\Communication\BaseSocket.h"
#include "Classes\Web\Communication\TCPListener.h"
#include "Classes\Web\Communication\TCPConnector.h"
#include "Classes\Web\Communication\TCPClienter.h"
#include "Classes\Web\Communication\UDPUnit.h"

#include "Core/KxLog.h"

#endif
