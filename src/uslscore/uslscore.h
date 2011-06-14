// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USLSCORE_H
#define USLSCORE_H

#include <uslscore/pch.h>

#include <uslscore/STLArray.h>
#include <uslscore/STLIteration.h>
#include <uslscore/STLList.h>
#include <uslscore/STLMap.h>
#include <uslscore/STLSet.h>
#include <uslscore/STLString.h>

#include <uslscore/USLuaSerializer.h>
#include <uslscore/USLuaState.h>
#include <uslscore/USLuaStateHandle.h>
#include <uslscore/USLuaObject.h>
#include <uslscore/USLuaRuntime.h>
#include <uslscore/USLuaRef.h>

#include <uslscore/USAccessors.h>
#include <uslscore/USBase64Cipher.h>
#include <uslscore/USByteStream.h>
#include <uslscore/USCipher.h>
#include <uslscore/USCipherStream.h>
#include <uslscore/USDeflater.h>
#include <uslscore/USDeviceTime.h>
#include <uslscore/USDirectoryItr.h>
#include <uslscore/USFactory.h>
#include <uslscore/USFilename.h>
#include <uslscore/USFileStream.h>
#include <uslscore/USFileSys.h>
#include <uslscore/USFloat.h>
#include <uslscore/USGlobals.h>
#include <uslscore/USInflater.h>
#include <uslscore/USLeanArray.h>
#include <uslscore/USLeanList.h>
#include <uslscore/USLeanPool.h>
#include <uslscore/USLeanStack.h>
#include <uslscore/USList.h>
#include <uslscore/USLog.h>
#include <uslscore/USMemStream.h>
#include <uslscore/USRect.h>
#include <uslscore/USRef.h>
#include <uslscore/USRtti.h>
#include <uslscore/USStream.h>
#include <uslscore/USStreamFormatter.h>
#include <uslscore/USTypeID.h>
#include <uslscore/USUnion.h>
#include <uslscore/USVec2D.h>
#include <uslscore/USVec3D.h>
#include <uslscore/USWeak.h>
#include <uslscore/USZip.h>
#include <uslscore/USZipFile.h>

#include <uslscore/USLuaState-impl.h>

//================================================================//
// uslscore
//================================================================//
namespace uslscore {

	//----------------------------------------------------------------//
	void			InitGlobals			( USGlobals* globals );
}

#endif
