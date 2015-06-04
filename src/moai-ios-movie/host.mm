// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import "pch.h"
#import "host.h"

#import <moai-ios-movie/MOAIMoviePlayerIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosMovieAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosMovieAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosMovieContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIMoviePlayerIOS )
}
