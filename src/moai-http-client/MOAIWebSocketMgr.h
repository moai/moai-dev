// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIWEBSOCKETMGR_H
#define MOAIWEBSOCKETMGR_H

#include <moai-http-client/MoaiWebSocket.h>

//================================================================//
// MOAIUrlMgrCurl
//================================================================//
class MOAIWebSocketMgr :
	public MOAIGlobalClass < MOAIWebSocketMgr > {
private:

	STLList < MOAIWebSocket* > mTasks;

	//----------------------------------------------------------------//
	void			AddHandle				( MOAIWebSocket& task );

public:

	friend class MOAIWebSocket;

	//----------------------------------------------------------------//
					MOAIWebSocketMgr			();
					~MOAIWebSocketMgr			();
	void			Process					();
};

#endif
