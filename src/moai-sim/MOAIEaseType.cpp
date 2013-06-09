// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIEaseType.h>

//================================================================//
// MOAIEaseType
//================================================================//

//----------------------------------------------------------------//
void MOAIEaseType::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "EASE_IN", ( u32 )ZLInterpolate::kEaseIn );
	state.SetField ( -1, "EASE_OUT", ( u32 )ZLInterpolate::kEaseOut );
	state.SetField ( -1, "FLAT", ( u32 )ZLInterpolate::kFlat );
	state.SetField ( -1, "LINEAR", ( u32 )ZLInterpolate::kLinear );
	state.SetField ( -1, "SHARP_EASE_IN", ( u32 )ZLInterpolate::kSharpEaseIn );
	state.SetField ( -1, "SHARP_EASE_OUT", ( u32 )ZLInterpolate::kSharpEaseOut );
	state.SetField ( -1, "SHARP_SMOOTH", ( u32 )ZLInterpolate::kSharpSmooth );
	state.SetField ( -1, "SMOOTH", ( u32 )ZLInterpolate::kSmooth );
	state.SetField ( -1, "SOFT_EASE_IN", ( u32 )ZLInterpolate::kSoftEaseIn );
	state.SetField ( -1, "SOFT_EASE_OUT", ( u32 )ZLInterpolate::kSoftEaseOut );
	state.SetField ( -1, "SOFT_SMOOTH", ( u32 )ZLInterpolate::kSoftSmooth );
}
