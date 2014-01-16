// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEaseType.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// MOAIEaseType
//================================================================//

//----------------------------------------------------------------//
void MOAIEaseType::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "EASE_IN", ( u32 )USInterpolate::kEaseIn );
	state.SetField ( -1, "EASE_OUT", ( u32 )USInterpolate::kEaseOut );
	state.SetField ( -1, "FLAT", ( u32 )USInterpolate::kFlat );
	state.SetField ( -1, "LINEAR", ( u32 )USInterpolate::kLinear );
	state.SetField ( -1, "SHARP_EASE_IN", ( u32 )USInterpolate::kSharpEaseIn );
	state.SetField ( -1, "SHARP_EASE_OUT", ( u32 )USInterpolate::kSharpEaseOut );
	state.SetField ( -1, "SHARP_SMOOTH", ( u32 )USInterpolate::kSharpSmooth );
	state.SetField ( -1, "SMOOTH", ( u32 )USInterpolate::kSmooth );
	state.SetField ( -1, "SOFT_EASE_IN", ( u32 )USInterpolate::kSoftEaseIn );
	state.SetField ( -1, "SOFT_EASE_OUT", ( u32 )USInterpolate::kSoftEaseOut );
	state.SetField ( -1, "SOFT_SMOOTH", ( u32 )USInterpolate::kSoftSmooth );
}
