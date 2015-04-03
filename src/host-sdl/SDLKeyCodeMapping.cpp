#include <moai-core/host.h>
#include <host-modules/aku_modules.h>

#include "SDLKeyCodeMapping.h"

//----------------------------------------------------------------//
int GetMoaiKeyCode ( SDL_Event sdlEvent ) {
	// Test for non-OEM keys
	int sdlKeyCode = sdlEvent.key.keysym.sym;
	
	switch ( sdlKeyCode )	{
		case SDLK_BACKSPACE:		return MOAI_KEY_BACKSPACE;
		case SDLK_TAB:				return MOAI_KEY_TAB;
		case SDLK_RETURN:
		case SDLK_KP_ENTER:			return MOAI_KEY_RETURN;
		case SDLK_LSHIFT:
		case SDLK_RSHIFT:			return MOAI_KEY_SHIFT;
		case SDLK_LCTRL:
		case SDLK_RCTRL:			return MOAI_KEY_CONTROL;
		case SDLK_LALT:
		case SDLK_RALT:				return MOAI_KEY_ALT;
		case SDLK_PAUSE:			return MOAI_KEY_PAUSE;
		case SDLK_CAPSLOCK:			return MOAI_KEY_CAPS_LOCK;
		case SDLK_ESCAPE:			return MOAI_KEY_ESCAPE;
		case SDLK_SPACE:			return MOAI_KEY_SPACE;
		case SDLK_PAGEUP:			return MOAI_KEY_PAGE_UP;
		case SDLK_PAGEDOWN:			return MOAI_KEY_PAGE_DOWN;
		case SDLK_END:				return MOAI_KEY_END;
		case SDLK_HOME:				return MOAI_KEY_HOME;
		case SDLK_LEFT:				return MOAI_KEY_LEFT;
		case SDLK_UP:				return MOAI_KEY_UP;
		case SDLK_RIGHT:			return MOAI_KEY_RIGHT;
		case SDLK_DOWN:				return MOAI_KEY_DOWN;
		case SDLK_PRINTSCREEN:		return MOAI_KEY_PRINT_SCREEN;
		case SDLK_INSERT:			return MOAI_KEY_INSERT;
		case SDLK_DELETE:			return MOAI_KEY_DELETE;
		case SDLK_0:				return MOAI_KEY_DIGIT_0;
		case SDLK_1:				return MOAI_KEY_DIGIT_1;
		case SDLK_2:				return MOAI_KEY_DIGIT_2;
		case SDLK_3:				return MOAI_KEY_DIGIT_3;
		case SDLK_4:				return MOAI_KEY_DIGIT_4;
		case SDLK_5:				return MOAI_KEY_DIGIT_5;
		case SDLK_6:				return MOAI_KEY_DIGIT_6;
		case SDLK_7:				return MOAI_KEY_DIGIT_7;
		case SDLK_8:				return MOAI_KEY_DIGIT_8;
		case SDLK_9:				return MOAI_KEY_DIGIT_9;
		case SDLK_a:				return MOAI_KEY_A;
		case SDLK_b:				return MOAI_KEY_B;
		case SDLK_c:				return MOAI_KEY_C;
		case SDLK_d:				return MOAI_KEY_D;
		case SDLK_e:				return MOAI_KEY_E;
		case SDLK_f:				return MOAI_KEY_F;
		case SDLK_g:				return MOAI_KEY_G;
		case SDLK_h:				return MOAI_KEY_H;
		case SDLK_i:				return MOAI_KEY_I;
		case SDLK_j:				return MOAI_KEY_J;
		case SDLK_k:				return MOAI_KEY_K;
		case SDLK_l:				return MOAI_KEY_L;
		case SDLK_m:				return MOAI_KEY_M;
		case SDLK_n:				return MOAI_KEY_N;
		case SDLK_o:				return MOAI_KEY_O;
		case SDLK_p:				return MOAI_KEY_P;
		case SDLK_q:				return MOAI_KEY_Q;
		case SDLK_r:				return MOAI_KEY_R;
		case SDLK_s:				return MOAI_KEY_S;
		case SDLK_t:				return MOAI_KEY_T;
		case SDLK_u:				return MOAI_KEY_U;
		case SDLK_v:				return MOAI_KEY_V;
		case SDLK_w:				return MOAI_KEY_W;
		case SDLK_x:				return MOAI_KEY_X;
		case SDLK_y:				return MOAI_KEY_Y;
		case SDLK_z:				return MOAI_KEY_Z;
		case SDLK_LGUI:
		case SDLK_RGUI:				return MOAI_KEY_GUI;
		case SDLK_APPLICATION:		return MOAI_KEY_APPLICATION;
		case SDLK_KP_0:				return MOAI_KEY_NUM_0;
		case SDLK_KP_1:				return MOAI_KEY_NUM_1;
		case SDLK_KP_2:				return MOAI_KEY_NUM_2;
		case SDLK_KP_3:				return MOAI_KEY_NUM_3;
		case SDLK_KP_4:				return MOAI_KEY_NUM_4;
		case SDLK_KP_5:				return MOAI_KEY_NUM_5;
		case SDLK_KP_6:				return MOAI_KEY_NUM_6;
		case SDLK_KP_7:				return MOAI_KEY_NUM_7;
		case SDLK_KP_8:				return MOAI_KEY_NUM_8;
		case SDLK_KP_9:				return MOAI_KEY_NUM_9;
		case SDLK_KP_MULTIPLY:		return MOAI_KEY_NUM_MULTIPLY;
		case SDLK_KP_PLUS:			return MOAI_KEY_NUM_PLUS;
		case SDLK_KP_MINUS:			return MOAI_KEY_NUM_MINUS;
		case SDLK_KP_PERIOD:		return MOAI_KEY_NUM_DECIMAL;
		case SDLK_KP_DIVIDE:		return MOAI_KEY_NUM_DIVIDE;
		case SDLK_F1:				return MOAI_KEY_F1;
		case SDLK_F2:				return MOAI_KEY_F2;
		case SDLK_F3:				return MOAI_KEY_F3;
		case SDLK_F4:				return MOAI_KEY_F4;
		case SDLK_F5:				return MOAI_KEY_F5;
		case SDLK_F6:				return MOAI_KEY_F6;
		case SDLK_F7:				return MOAI_KEY_F7;
		case SDLK_F8:				return MOAI_KEY_F8;
		case SDLK_F9:				return MOAI_KEY_F9;
		case SDLK_F10:				return MOAI_KEY_F10;
		case SDLK_F11:				return MOAI_KEY_F11;
		case SDLK_F12:				return MOAI_KEY_F12;
		case SDLK_NUMLOCKCLEAR:		return MOAI_KEY_NUM_LOCK;
		case SDLK_SCROLLLOCK:		return MOAI_KEY_SCROLL_LOCK;
	}

	// The key is an OEM key, i.e. it depends on the keyboard layout.
	// SDL won't give us enouth information; this hack at least supports US keyboards.
	switch ( sdlKeyCode ) {
		case SDLK_SEMICOLON:		return MOAI_KEY_OEM_1;
		case SDLK_EQUALS:			return MOAI_KEY_OEM_PLUS;
		case SDLK_COMMA:			return MOAI_KEY_OEM_COMMA;
		case SDLK_MINUS:			return MOAI_KEY_OEM_MINUS;
		case SDLK_PERIOD:			return MOAI_KEY_OEM_PERIOD;
		case SDLK_SLASH:			return MOAI_KEY_OEM_2;
		case SDLK_BACKQUOTE:		return MOAI_KEY_OEM_3;
		case SDLK_LEFTBRACKET:		return MOAI_KEY_OEM_4;
		case SDLK_BACKSLASH:		return MOAI_KEY_OEM_5;
		case SDLK_RIGHTBRACKET:		return MOAI_KEY_OEM_6;
		case SDLK_QUOTE:			return MOAI_KEY_OEM_7;
	}

	return MOAI_KEY_INVALID;
}