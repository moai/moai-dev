#include "pch.h"
#include <moai-sim/MOAIKeyCode.h>
#include <moai-sim/MOAIKeyCodeEnum.h>

//================================================================//
// MOAIKeyCode
//================================================================//

//----------------------------------------------------------------//
void MOAIKeyCode::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "BACKSPACE",		( u32 )MOAI_KEY_BACKSPACE );
	state.SetField ( -1, "TAB",				( u32 )MOAI_KEY_TAB );
	state.SetField ( -1, "RETURN",			( u32 )MOAI_KEY_RETURN );
	state.SetField ( -1, "SHIFT",			( u32 )MOAI_KEY_SHIFT );
	state.SetField ( -1, "CONTROL",			( u32 )MOAI_KEY_CONTROL );
	state.SetField ( -1, "ALT",				( u32 )MOAI_KEY_ALT );
	state.SetField ( -1, "PAUSE",			( u32 )MOAI_KEY_PAUSE );
	state.SetField ( -1, "CAPS_LOCK",		( u32 )MOAI_KEY_CAPS_LOCK );
	state.SetField ( -1, "ESCAPE",			( u32 )MOAI_KEY_ESCAPE );
	state.SetField ( -1, "SPACE",			( u32 )MOAI_KEY_SPACE );
	state.SetField ( -1, "PAGE_UP",			( u32 )MOAI_KEY_PAGE_UP );
	state.SetField ( -1, "PAGE_DOWN",		( u32 )MOAI_KEY_PAGE_DOWN );
	state.SetField ( -1, "END",				( u32 )MOAI_KEY_END );
	state.SetField ( -1, "HOME",			( u32 )MOAI_KEY_HOME );
	state.SetField ( -1, "LEFT",			( u32 )MOAI_KEY_LEFT );
	state.SetField ( -1, "UP",				( u32 )MOAI_KEY_UP );
	state.SetField ( -1, "RIGHT",			( u32 )MOAI_KEY_RIGHT );
	state.SetField ( -1, "DOWN",			( u32 )MOAI_KEY_DOWN );
	state.SetField ( -1, "PRINT_SCREEN",	( u32 )MOAI_KEY_PRINT_SCREEN );
	state.SetField ( -1, "INSERT",			( u32 )MOAI_KEY_INSERT );
	state.SetField ( -1, "DELETE",			( u32 )MOAI_KEY_DELETE );
	state.SetField ( -1, "DIGIT_0",			( u32 )MOAI_KEY_DIGIT_0 );
	state.SetField ( -1, "DIGIT_1",			( u32 )MOAI_KEY_DIGIT_1 );
	state.SetField ( -1, "DIGIT_2",			( u32 )MOAI_KEY_DIGIT_2 );
	state.SetField ( -1, "DIGIT_3",			( u32 )MOAI_KEY_DIGIT_3 );
	state.SetField ( -1, "DIGIT_4",			( u32 )MOAI_KEY_DIGIT_4 );
	state.SetField ( -1, "DIGIT_5",			( u32 )MOAI_KEY_DIGIT_5 );
	state.SetField ( -1, "DIGIT_6",			( u32 )MOAI_KEY_DIGIT_6 );
	state.SetField ( -1, "DIGIT_7",			( u32 )MOAI_KEY_DIGIT_7 );
	state.SetField ( -1, "DIGIT_8",			( u32 )MOAI_KEY_DIGIT_8 );
	state.SetField ( -1, "DIGIT_9",			( u32 )MOAI_KEY_DIGIT_9 );
	state.SetField ( -1, "A",				( u32 )MOAI_KEY_A );
	state.SetField ( -1, "B",				( u32 )MOAI_KEY_B );
	state.SetField ( -1, "C",				( u32 )MOAI_KEY_C );
	state.SetField ( -1, "D",				( u32 )MOAI_KEY_D );
	state.SetField ( -1, "E",				( u32 )MOAI_KEY_E );
	state.SetField ( -1, "F",				( u32 )MOAI_KEY_F );
	state.SetField ( -1, "G",				( u32 )MOAI_KEY_G );
	state.SetField ( -1, "H",				( u32 )MOAI_KEY_H );
	state.SetField ( -1, "I",				( u32 )MOAI_KEY_I );
	state.SetField ( -1, "J",				( u32 )MOAI_KEY_J );
	state.SetField ( -1, "K",				( u32 )MOAI_KEY_K );
	state.SetField ( -1, "L",				( u32 )MOAI_KEY_L );
	state.SetField ( -1, "M",				( u32 )MOAI_KEY_M );
	state.SetField ( -1, "N",				( u32 )MOAI_KEY_N );
	state.SetField ( -1, "O",				( u32 )MOAI_KEY_O );
	state.SetField ( -1, "P",				( u32 )MOAI_KEY_P );
	state.SetField ( -1, "Q",				( u32 )MOAI_KEY_Q );
	state.SetField ( -1, "R",				( u32 )MOAI_KEY_R );
	state.SetField ( -1, "S",				( u32 )MOAI_KEY_S );
	state.SetField ( -1, "T",				( u32 )MOAI_KEY_T );
	state.SetField ( -1, "U",				( u32 )MOAI_KEY_U );
	state.SetField ( -1, "V",				( u32 )MOAI_KEY_V );
	state.SetField ( -1, "W",				( u32 )MOAI_KEY_W );
	state.SetField ( -1, "X",				( u32 )MOAI_KEY_X );
	state.SetField ( -1, "Y",				( u32 )MOAI_KEY_Y );
	state.SetField ( -1, "Z",				( u32 )MOAI_KEY_Z );
	state.SetField ( -1, "GUI",				( u32 )MOAI_KEY_GUI );
	state.SetField ( -1, "APPLICATION",		( u32 )MOAI_KEY_APPLICATION );
	state.SetField ( -1, "NUM_0",			( u32 )MOAI_KEY_NUM_0 );
	state.SetField ( -1, "NUM_1",			( u32 )MOAI_KEY_NUM_1 );
	state.SetField ( -1, "NUM_2",			( u32 )MOAI_KEY_NUM_2 );
	state.SetField ( -1, "NUM_3",			( u32 )MOAI_KEY_NUM_3 );
	state.SetField ( -1, "NUM_4",			( u32 )MOAI_KEY_NUM_4 );
	state.SetField ( -1, "NUM_5",			( u32 )MOAI_KEY_NUM_5 );
	state.SetField ( -1, "NUM_6",			( u32 )MOAI_KEY_NUM_6 );
	state.SetField ( -1, "NUM_7",			( u32 )MOAI_KEY_NUM_7 );
	state.SetField ( -1, "NUM_8",			( u32 )MOAI_KEY_NUM_8 );
	state.SetField ( -1, "NUM_9",			( u32 )MOAI_KEY_NUM_9 );
	state.SetField ( -1, "NUM_MULTIPLY",	( u32 )MOAI_KEY_NUM_MULTIPLY );
	state.SetField ( -1, "NUM_PLUS",		( u32 )MOAI_KEY_NUM_PLUS );
	state.SetField ( -1, "NUM_MINUS",		( u32 )MOAI_KEY_NUM_MINUS );
	state.SetField ( -1, "NUM_DECIMAL",		( u32 )MOAI_KEY_NUM_DECIMAL );
	state.SetField ( -1, "NUM_DIVIDE",		( u32 )MOAI_KEY_NUM_DIVIDE );
	state.SetField ( -1, "F1",				( u32 )MOAI_KEY_F1 );
	state.SetField ( -1, "F2",				( u32 )MOAI_KEY_F2 );
	state.SetField ( -1, "F3",				( u32 )MOAI_KEY_F3 );
	state.SetField ( -1, "F4",				( u32 )MOAI_KEY_F4 );
	state.SetField ( -1, "F5",				( u32 )MOAI_KEY_F5 );
	state.SetField ( -1, "F6",				( u32 )MOAI_KEY_F6 );
	state.SetField ( -1, "F7",				( u32 )MOAI_KEY_F7 );
	state.SetField ( -1, "F8",				( u32 )MOAI_KEY_F8 );
	state.SetField ( -1, "F9",				( u32 )MOAI_KEY_F9 );
	state.SetField ( -1, "F10",				( u32 )MOAI_KEY_F10 );
	state.SetField ( -1, "F11",				( u32 )MOAI_KEY_F11 );
	state.SetField ( -1, "F12",				( u32 )MOAI_KEY_F12 );
	state.SetField ( -1, "NUM_LOCK",		( u32 )MOAI_KEY_NUM_LOCK );
	state.SetField ( -1, "SCROLL_LOCK",		( u32 )MOAI_KEY_SCROLL_LOCK );
	state.SetField ( -1, "OEM_1",			( u32 )MOAI_KEY_OEM_1 );
	state.SetField ( -1, "OEM_PLUS",		( u32 )MOAI_KEY_OEM_PLUS );
	state.SetField ( -1, "OEM_COMMA",		( u32 )MOAI_KEY_OEM_COMMA );
	state.SetField ( -1, "OEM_MINUS",		( u32 )MOAI_KEY_OEM_MINUS );
	state.SetField ( -1, "OEM_PERIOD",		( u32 )MOAI_KEY_OEM_PERIOD );
	state.SetField ( -1, "OEM_2",			( u32 )MOAI_KEY_OEM_2 );
	state.SetField ( -1, "OEM_3",			( u32 )MOAI_KEY_OEM_3 );
	state.SetField ( -1, "OEM_4",			( u32 )MOAI_KEY_OEM_4 );
	state.SetField ( -1, "OEM_5",			( u32 )MOAI_KEY_OEM_5 );
	state.SetField ( -1, "OEM_6",			( u32 )MOAI_KEY_OEM_6 );
	state.SetField ( -1, "OEM_7",			( u32 )MOAI_KEY_OEM_7 );
	state.SetField ( -1, "OEM_8",			( u32 )MOAI_KEY_OEM_8 );
	state.SetField ( -1, "OEM_102",			( u32 )MOAI_KEY_OEM_102 );
}
