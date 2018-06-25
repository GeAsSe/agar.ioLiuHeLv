#ifndef __KXSERVER_H__
#define __KXSERVER_H__

#include "Core/ICore.h"
#include "Core/TimerManager.h"
#include "Core/MemPool.h"
#include "Core/BufferList.h"
#include "Core/CommPool.h"
#include "Core/Ref.h"

#include "ServerBase/BaseServer.h"

#include "communication/SelectPoller.h"
#include "communication/Epoller.h"
#include "communication/BaseSocket.h"
#include "communication/TCPListener.h"
#include "communication/TCPConnector.h"
#include "communication/TCPClienter.h"
#include "communication/UDPUnit.h"

#include "Core/KxLog.h"

#endif
