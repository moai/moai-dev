#include "KeyCodeMapping.h"
#include <moai-sim/MOAIKeyCode.h>

//----------------------------------------------------------------//
u32 GetMoaiKeyCode ( SDL_Event sdlEvent ) {
	// Test for non-OEM keys
	u32 sdlKeyCode = sdlEvent.key.keysym.sym;
	switch ( sdlKeyCode )	{
		case SDLK_BACKSPACE:		return MOAIKeyCodes::BACKSPACE;
		case SDLK_TAB:				return MOAIKeyCodes::TAB;
		case SDLK_RETURN:
		case SDLK_KP_ENTER:			return MOAIKeyCodes::RETURN;
		case SDLK_LSHIFT:
		case SDLK_RSHIFT:			return MOAIKeyCodes::SHIFT;
		case SDLK_LCTRL:
		case SDLK_RCTRL:			return MOAIKeyCodes::CONTROL;
		case SDLK_LALT:
		case SDLK_RALT:				return MOAIKeyCodes::ALT;
		case SDLK_PAUSE:			return MOAIKeyCodes::PAUSE;
		case SDLK_CAPSLOCK:			return MOAIKeyCodes::CAPS_LOCK;
		case SDLK_ESCAPE:			return MOAIKeyCodes::ESCAPE;
		case SDLK_SPACE:			return MOAIKeyCodes::SPACE;
		case SDLK_PAGEUP:			return MOAIKeyCodes::PAGE_UP;
		case SDLK_PAGEDOWN:			return MOAIKeyCodes::PAGE_DOWN;
		case SDLK_END:				return MOAIKeyCodes::END;
		case SDLK_HOME:				return MOAIKeyCodes::HOME;
		case SDLK_LEFT:				return MOAIKeyCodes::LEFT;
		case SDLK_UP:				return MOAIKeyCodes::UP;
		case SDLK_RIGHT:			return MOAIKeyCodes::RIGHT;
		case SDLK_DOWN:				return MOAIKeyCodes::DOWN;
		case SDLK_PRINTSCREEN:		return MOAIKeyCodes::PRINT_SCREEN;
		case SDLK_INSERT:			return MOAIKeyCodes::INSERT;
		case SDLK_DELETE:			return MOAIKeyCodes::DELETE;
		case SDLK_0:				return MOAIKeyCodes::DIGIT_0;
		case SDLK_1:				return MOAIKeyCodes::DIGIT_1;
		case SDLK_2:				return MOAIKeyCodes::DIGIT_2;
		case SDLK_3:				return MOAIKeyCodes::DIGIT_3;
		case SDLK_4:				return MOAIKeyCodes::DIGIT_4;
		case SDLK_5:				return MOAIKeyCodes::DIGIT_5;
		case SDLK_6:				return MOAIKeyCodes::DIGIT_6;
		case SDLK_7:				return MOAIKeyCodes::DIGIT_7;
		case SDLK_8:				return MOAIKeyCodes::DIGIT_8;
		case SDLK_9:				return MOAIKeyCodes::DIGIT_9;
		case SDLK_a:				return MOAIKeyCodes::A;
		case SDLK_b:				return MOAIKeyCodes::B;
		case SDLK_c:				return MOAIKeyCodes::C;
		case SDLK_d:				return MOAIKeyCodes::D;
		case SDLK_e:				return MOAIKeyCodes::E;
		case SDLK_f:				return MOAIKeyCodes::F;
		case SDLK_g:				return MOAIKeyCodes::G;
		case SDLK_h:				return MOAIKeyCodes::H;
		case SDLK_i:				return MOAIKeyCodes::I;
		case SDLK_j:				return MOAIKeyCodes::J;
		case SDLK_k:				return MOAIKeyCodes::K;
		case SDLK_l:				return MOAIKeyCodes::L;
		case SDLK_m:				return MOAIKeyCodes::M;
		case SDLK_n:				return MOAIKeyCodes::N;
		case SDLK_o:				return MOAIKeyCodes::O;
		case SDLK_p:				return MOAIKeyCodes::P;
		case SDLK_q:				return MOAIKeyCodes::Q;
		case SDLK_r:				return MOAIKeyCodes::R;
		case SDLK_s:				return MOAIKeyCodes::S;
		case SDLK_t:				return MOAIKeyCodes::T;
		case SDLK_u:				return MOAIKeyCodes::U;
		case SDLK_v:				return MOAIKeyCodes::V;
		case SDLK_w:				return MOAIKeyCodes::W;
		case SDLK_x:				return MOAIKeyCodes::X;
		case SDLK_y:				return MOAIKeyCodes::Y;
		case SDLK_z:				return MOAIKeyCodes::Z;
		case SDLK_LGUI:
		case SDLK_RGUI:				return MOAIKeyCodes::GUI;
		case SDLK_APPLICATION:		return MOAIKeyCodes::APPLICATION;
		case SDLK_KP_0:				return MOAIKeyCodes::NUM_0;
		case SDLK_KP_1:				return MOAIKeyCodes::NUM_1;
		case SDLK_KP_2:				return MOAIKeyCodes::NUM_2;
		case SDLK_KP_3:				return MOAIKeyCodes::NUM_3;
		case SDLK_KP_4:				return MOAIKeyCodes::NUM_4;
		case SDLK_KP_5:				return MOAIKeyCodes::NUM_5;
		case SDLK_KP_6:				return MOAIKeyCodes::NUM_6;
		case SDLK_KP_7:				return MOAIKeyCodes::NUM_7;
		case SDLK_KP_8:				return MOAIKeyCodes::NUM_8;
		case SDLK_KP_9:				return MOAIKeyCodes::NUM_9;
		case SDLK_KP_MULTIPLY:		return MOAIKeyCodes::NUM_MULTIPLY;
		case SDLK_KP_PLUS:			return MOAIKeyCodes::NUM_PLUS;
		case SDLK_KP_MINUS:			return MOAIKeyCodes::NUM_MINUS;
		case SDLK_KP_PERIOD:		return MOAIKeyCodes::NUM_DECIMAL;
		case SDLK_KP_DIVIDE:		return MOAIKeyCodes::NUM_DIVIDE;
		case SDLK_F1:				return MOAIKeyCodes::F1;
		case SDLK_F2:				return MOAIKeyCodes::F2;
		case SDLK_F3:				return MOAIKeyCodes::F3;
		case SDLK_F4:				return MOAIKeyCodes::F4;
		case SDLK_F5:				return MOAIKeyCodes::F5;
		case SDLK_F6:				return MOAIKeyCodes::F6;
		case SDLK_F7:				return MOAIKeyCodes::F7;
		case SDLK_F8:				return MOAIKeyCodes::F8;
		case SDLK_F9:				return MOAIKeyCodes::F9;
		case SDLK_F10:				return MOAIKeyCodes::F10;
		case SDLK_F11:				return MOAIKeyCodes::F11;
		case SDLK_F12:				return MOAIKeyCodes::F12;
		case SDLK_NUMLOCKCLEAR:		return MOAIKeyCodes::NUM_LOCK;
		case SDLK_SCROLLLOCK:		return MOAIKeyCodes::SCROLL_LOCK;
	}

	// The key is an OEM key, i.e. it depends on the keyboard layout.
	// SDL won't give us enouth information; this hack at least supports US keyboards.
	switch ( sdlKeyCode ) {
		case SDLK_SEMICOLON:		return MOAIKeyCodes::OEM_1;
		case SDLK_EQUALS:			return MOAIKeyCodes::OEM_PLUS;
		case SDLK_COMMA:			return MOAIKeyCodes::OEM_COMMA;
		case SDLK_MINUS:			return MOAIKeyCodes::OEM_MINUS;
		case SDLK_PERIOD:			return MOAIKeyCodes::OEM_PERIOD;
		case SDLK_SLASH:			return MOAIKeyCodes::OEM_2;
		case SDLK_BACKQUOTE:		return MOAIKeyCodes::OEM_3;
		case SDLK_LEFTBRACKET:		return MOAIKeyCodes::OEM_4;
		case SDLK_BACKSLASH:		return MOAIKeyCodes::OEM_5;
		case SDLK_RIGHTBRACKET:		return MOAIKeyCodes::OEM_6;
		case SDLK_QUOTE:			return MOAIKeyCodes::OEM_7;
	}

	return MOAIKeyCodes::INVALID;
}