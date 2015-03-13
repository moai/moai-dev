// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAI_UTIL_H
#define	MOAI_UTIL_H

#include <moai-util/pch.h>

#include <moai-util/MOAIByteStream.h>
#include <moai-util/MOAIDataBuffer.h>
#include <moai-util/MOAIDataBufferStream.h>
#include <moai-util/MOAIDataIOTask.h>
#include <moai-util/MOAIFileStream.h>
#include <moai-util/MOAIFileSystem.h>
#include <moai-util/MOAIHashWriter.h>
#include <moai-util/MOAIJsonParser.h>
#include <moai-util/MOAIMath.h>
#include <moai-util/MOAIMemStream.h>
#include <moai-util/MOAIMutex.h>
#include <moai-util/MOAIMutex_posix.h>
#include <moai-util/MOAIMutex_win32.h>
#include <moai-util/MOAIParser.h>
#include <moai-util/MOAIStream.h>
#include <moai-util/MOAIStreamAdapter.h>
#include <moai-util/MOAIStreamReader.h>
#include <moai-util/MOAIStreamWriter.h>
#include <moai-util/MOAITask.h>
#include <moai-util/MOAITaskQueue.h>
#include <moai-util/MOAITaskSubscriber.h>
#include <moai-util/MOAITaskThread.h>
#include <moai-util/MOAIThread.h>
#include <moai-util/MOAIThread_posix.h>
#include <moai-util/MOAIThread_win32.h>

#if MOAI_WITH_TINYXML
  #include <moai-util/MOAIXmlParser.h>
  #include <moai-util/MOAIXmlWriter.h>
#endif 

//================================================================//
// moai_util
//================================================================//
namespace moai_util {

	//----------------------------------------------------------------//
	void			Init				(); // initialize a Moai context
}

#endif
