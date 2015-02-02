#include "pch.h"
#include "MOAIKeyCode.h"

//================================================================//
// MOAIKeyCode
//================================================================//

//----------------------------------------------------------------//
void MOAIKeyCode::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "BACKSPACE",		( u32 )MOAIKeyCodes::BACKSPACE );
	state.SetField ( -1, "TAB",				( u32 )MOAIKeyCodes::TAB );
	state.SetField ( -1, "RETURN",			( u32 )MOAIKeyCodes::RETURN );
	state.SetField ( -1, "SHIFT",			( u32 )MOAIKeyCodes::SHIFT );
	state.SetField ( -1, "CONTROL",			( u32 )MOAIKeyCodes::CONTROL );
	state.SetField ( -1, "ALT",				( u32 )MOAIKeyCodes::ALT );
	state.SetField ( -1, "PAUSE",			( u32 )MOAIKeyCodes::PAUSE );
	state.SetField ( -1, "CAPS_LOCK",		( u32 )MOAIKeyCodes::CAPS_LOCK );
	state.SetField ( -1, "ESCAPE",			( u32 )MOAIKeyCodes::ESCAPE );
	state.SetField ( -1, "SPACE",			( u32 )MOAIKeyCodes::SPACE );
	state.SetField ( -1, "PAGE_UP",			( u32 )MOAIKeyCodes::PAGE_UP );
	state.SetField ( -1, "PAGE_DOWN",		( u32 )MOAIKeyCodes::PAGE_DOWN );
	state.SetField ( -1, "END",				( u32 )MOAIKeyCodes::END );
	state.SetField ( -1, "HOME",			( u32 )MOAIKeyCodes::HOME );
	state.SetField ( -1, "LEFT",			( u32 )MOAIKeyCodes::LEFT );
	state.SetField ( -1, "UP",				( u32 )MOAIKeyCodes::UP );
	state.SetField ( -1, "RIGHT",			( u32 )MOAIKeyCodes::RIGHT );
	state.SetField ( -1, "DOWN",			( u32 )MOAIKeyCodes::DOWN );
	state.SetField ( -1, "PRINT_SCREEN",	( u32 )MOAIKeyCodes::PRINT_SCREEN );
	state.SetField ( -1, "INSERT",			( u32 )MOAIKeyCodes::INSERT );
	state.SetField ( -1, "DELETE",			( u32 )MOAIKeyCodes::DELETE );
	state.SetField ( -1, "DIGIT_0",			( u32 )MOAIKeyCodes::DIGIT_0 );
	state.SetField ( -1, "DIGIT_1",			( u32 )MOAIKeyCodes::DIGIT_1 );
	state.SetField ( -1, "DIGIT_2",			( u32 )MOAIKeyCodes::DIGIT_2 );
	state.SetField ( -1, "DIGIT_3",			( u32 )MOAIKeyCodes::DIGIT_3 );
	state.SetField ( -1, "DIGIT_4",			( u32 )MOAIKeyCodes::DIGIT_4 );
	state.SetField ( -1, "DIGIT_5",			( u32 )MOAIKeyCodes::DIGIT_5 );
	state.SetField ( -1, "DIGIT_6",			( u32 )MOAIKeyCodes::DIGIT_6 );
	state.SetField ( -1, "DIGIT_7",			( u32 )MOAIKeyCodes::DIGIT_7 );
	state.SetField ( -1, "DIGIT_8",			( u32 )MOAIKeyCodes::DIGIT_8 );
	state.SetField ( -1, "DIGIT_9",			( u32 )MOAIKeyCodes::DIGIT_9 );
	state.SetField ( -1, "A",				( u32 )MOAIKeyCodes::A );
	state.SetField ( -1, "B",				( u32 )MOAIKeyCodes::B );
	state.SetField ( -1, "C",				( u32 )MOAIKeyCodes::C );
	state.SetField ( -1, "D",				( u32 )MOAIKeyCodes::D );
	state.SetField ( -1, "E",				( u32 )MOAIKeyCodes::E );
	state.SetField ( -1, "F",				( u32 )MOAIKeyCodes::F );
	state.SetField ( -1, "G",				( u32 )MOAIKeyCodes::G );
	state.SetField ( -1, "H",				( u32 )MOAIKeyCodes::H );
	state.SetField ( -1, "I",				( u32 )MOAIKeyCodes::I );
	state.SetField ( -1, "J",				( u32 )MOAIKeyCodes::J );
	state.SetField ( -1, "K",				( u32 )MOAIKeyCodes::K );
	state.SetField ( -1, "L",				( u32 )MOAIKeyCodes::L );
	state.SetField ( -1, "M",				( u32 )MOAIKeyCodes::M );
	state.SetField ( -1, "N",				( u32 )MOAIKeyCodes::N );
	state.SetField ( -1, "O",				( u32 )MOAIKeyCodes::O );
	state.SetField ( -1, "P",				( u32 )MOAIKeyCodes::P );
	state.SetField ( -1, "Q",				( u32 )MOAIKeyCodes::Q );
	state.SetField ( -1, "R",				( u32 )MOAIKeyCodes::R );
	state.SetField ( -1, "S",				( u32 )MOAIKeyCodes::S );
	state.SetField ( -1, "T",				( u32 )MOAIKeyCodes::T );
	state.SetField ( -1, "U",				( u32 )MOAIKeyCodes::U );
	state.SetField ( -1, "V",				( u32 )MOAIKeyCodes::V );
	state.SetField ( -1, "W",				( u32 )MOAIKeyCodes::W );
	state.SetField ( -1, "X",				( u32 )MOAIKeyCodes::X );
	state.SetField ( -1, "Y",				( u32 )MOAIKeyCodes::Y );
	state.SetField ( -1, "Z",				( u32 )MOAIKeyCodes::Z );
	state.SetField ( -1, "GUI",				( u32 )MOAIKeyCodes::GUI );
	state.SetField ( -1, "APPLICATION",		( u32 )MOAIKeyCodes::APPLICATION );
	state.SetField ( -1, "NUM_0",			( u32 )MOAIKeyCodes::NUM_0 );
	state.SetField ( -1, "NUM_1",			( u32 )MOAIKeyCodes::NUM_1 );
	state.SetField ( -1, "NUM_2",			( u32 )MOAIKeyCodes::NUM_2 );
	state.SetField ( -1, "NUM_3",			( u32 )MOAIKeyCodes::NUM_3 );
	state.SetField ( -1, "NUM_4",			( u32 )MOAIKeyCodes::NUM_4 );
	state.SetField ( -1, "NUM_5",			( u32 )MOAIKeyCodes::NUM_5 );
	state.SetField ( -1, "NUM_6",			( u32 )MOAIKeyCodes::NUM_6 );
	state.SetField ( -1, "NUM_7",			( u32 )MOAIKeyCodes::NUM_7 );
	state.SetField ( -1, "NUM_8",			( u32 )MOAIKeyCodes::NUM_8 );
	state.SetField ( -1, "NUM_9",			( u32 )MOAIKeyCodes::NUM_9 );
	state.SetField ( -1, "NUM_MULTIPLY",	( u32 )MOAIKeyCodes::NUM_MULTIPLY );
	state.SetField ( -1, "NUM_PLUS",		( u32 )MOAIKeyCodes::NUM_PLUS );
	state.SetField ( -1, "NUM_MINUS",		( u32 )MOAIKeyCodes::NUM_MINUS );
	state.SetField ( -1, "NUM_DECIMAL",		( u32 )MOAIKeyCodes::NUM_DECIMAL );
	state.SetField ( -1, "NUM_DIVIDE",		( u32 )MOAIKeyCodes::NUM_DIVIDE );
	state.SetField ( -1, "F1",				( u32 )MOAIKeyCodes::F1 );
	state.SetField ( -1, "F2",				( u32 )MOAIKeyCodes::F2 );
	state.SetField ( -1, "F3",				( u32 )MOAIKeyCodes::F3 );
	state.SetField ( -1, "F4",				( u32 )MOAIKeyCodes::F4 );
	state.SetField ( -1, "F5",				( u32 )MOAIKeyCodes::F5 );
	state.SetField ( -1, "F6",				( u32 )MOAIKeyCodes::F6 );
	state.SetField ( -1, "F7",				( u32 )MOAIKeyCodes::F7 );
	state.SetField ( -1, "F8",				( u32 )MOAIKeyCodes::F8 );
	state.SetField ( -1, "F9",				( u32 )MOAIKeyCodes::F9 );
	state.SetField ( -1, "F10",				( u32 )MOAIKeyCodes::F10 );
	state.SetField ( -1, "F11",				( u32 )MOAIKeyCodes::F11 );
	state.SetField ( -1, "F12",				( u32 )MOAIKeyCodes::F12 );
	state.SetField ( -1, "NUM_LOCK",		( u32 )MOAIKeyCodes::NUM_LOCK );
	state.SetField ( -1, "SCROLL_LOCK",		( u32 )MOAIKeyCodes::SCROLL_LOCK );
	state.SetField ( -1, "OEM_1",			( u32 )MOAIKeyCodes::OEM_1 );
	state.SetField ( -1, "OEM_PLUS",		( u32 )MOAIKeyCodes::OEM_PLUS );
	state.SetField ( -1, "OEM_COMMA",		( u32 )MOAIKeyCodes::OEM_COMMA );
	state.SetField ( -1, "OEM_MINUS",		( u32 )MOAIKeyCodes::OEM_MINUS );
	state.SetField ( -1, "OEM_PERIOD",		( u32 )MOAIKeyCodes::OEM_PERIOD );
	state.SetField ( -1, "OEM_2",			( u32 )MOAIKeyCodes::OEM_2 );
	state.SetField ( -1, "OEM_3",			( u32 )MOAIKeyCodes::OEM_3 );
	state.SetField ( -1, "OEM_4",			( u32 )MOAIKeyCodes::OEM_4 );
	state.SetField ( -1, "OEM_5",			( u32 )MOAIKeyCodes::OEM_5 );
	state.SetField ( -1, "OEM_6",			( u32 )MOAIKeyCodes::OEM_6 );
	state.SetField ( -1, "OEM_7",			( u32 )MOAIKeyCodes::OEM_7 );
	state.SetField ( -1, "OEM_8",			( u32 )MOAIKeyCodes::OEM_8 );
	state.SetField ( -1, "OEM_102",			( u32 )MOAIKeyCodes::OEM_102 );
}
