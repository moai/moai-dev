// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAI_UTIL_H
#define	MOAI_UTIL_H

#include <moai-core/pch.h>

#include <moai-core/MOAIByteStream.h>
#include <moai-core/MOAIDataBuffer.h>
#include <moai-core/MOAIDataBufferStream.h>
#include <moai-core/MOAIDataIOTask.h>
#include <moai-core/MOAIFileStream.h>
#include <moai-core/MOAIFileSystem.h>
#include <moai-core/MOAIFourier.h>
#include <moai-core/MOAIHashWriter.h>
#include <moai-core/MOAIImage.h>
#include <moai-core/MOAIImageLoadTask.h>
#include <moai-core/MOAIJsonParser.h>
#include <moai-core/MOAILuaUtil.h>
#include <moai-core/MOAIMath.h>
#include <moai-core/MOAIMemStream.h>
#include <moai-core/MOAIParser.h>
#include <moai-core/MOAIStream.h>
#include <moai-core/MOAIStreamAdapter.h>
#include <moai-core/MOAIStreamReader.h>
#include <moai-core/MOAIStreamWriter.h>
#include <moai-core/MOAITask.h>
#include <moai-core/MOAITaskQueue.h>
#include <moai-core/MOAITaskSubscriber.h>

#if MOAI_WITH_TINYXML
  #include <moai-core/MOAIXmlParser.h>
  #include <moai-core/MOAIXmlWriter.h>
#endif 

//================================================================//
// moai_util
//================================================================//
namespace moai_util {

	//----------------------------------------------------------------//
	void			Init				(); // initialize a Moai context
}

#endif
