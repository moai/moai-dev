#ifndef MOAIKEYCODE_H
#define MOAIKEYCODE_H

//================================================================//
// MOAIKeyCode
//================================================================//
/**	@lua	MOAIKeyCode
	@text	Namespace to hold key codes to be used with MOAIKeyboardSensor.
	
	@const	BACKSPACE			The [Backspace] key.
	@const	TAB					The [Tab] key.
	@const	RETURN				The [Return] key on the main keyboard or the [ENTER] key on the numeric keypad.
	@const	SHIFT				The [Shift] key.
	@const	CONTROL				The [Ctrl] key.
	@const	ALT					The [Alt] key on a PC keyboard; the [Option] key on a Mac keyboard.
	@const	PAUSE				The [Pause] key.
	@const	CAPS_LOCK			The [Caps Lock] key.
	@const	ESCAPE				The [Esc] key.
	@const	SPACE				The spacebar. Most MOAIKeyboardSensor functions will accept the string " " instead of this constant.
	@const	PAGE_UP				The [Page Up] key.
	@const	PAGE_DOWN			The [Page Down] key.
	@const	END					The [End] key.
	@const	HOME				The [Home] key.
	@const	LEFT				The left arrow key.
	@const	UP					The up arrow key.
	@const	RIGHT				The right arrow key.
	@const	DOWN				The down arrow key.
	@const	PRINT_SCREEN		The [Print Screen] key.
	@const	INSERT				The [Insert] key.
	@const	DELETE				The [Delete] key.
	@const	DIGIT_0				The [0] key one the main keyboard. Most MOAIKeyboardSensor functions will accept the string "0" instead of this constant.
	@const	DIGIT_1				The [1] key one the main keyboard. Most MOAIKeyboardSensor functions will accept the string "1" instead of this constant.
	@const	DIGIT_2				The [2] key one the main keyboard. Most MOAIKeyboardSensor functions will accept the string "2" instead of this constant.
	@const	DIGIT_3				The [3] key one the main keyboard. Most MOAIKeyboardSensor functions will accept the string "3" instead of this constant.
	@const	DIGIT_4				The [4] key one the main keyboard. Most MOAIKeyboardSensor functions will accept the string "4" instead of this constant.
	@const	DIGIT_5				The [5] key one the main keyboard. Most MOAIKeyboardSensor functions will accept the string "5" instead of this constant.
	@const	DIGIT_6				The [6] key one the main keyboard. Most MOAIKeyboardSensor functions will accept the string "6" instead of this constant.
	@const	DIGIT_7				The [7] key one the main keyboard. Most MOAIKeyboardSensor functions will accept the string "7" instead of this constant.
	@const	DIGIT_8				The [8] key one the main keyboard. Most MOAIKeyboardSensor functions will accept the string "8" instead of this constant.
	@const	DIGIT_9				The [9] key one the main keyboard. Most MOAIKeyboardSensor functions will accept the string "9" instead of this constant.
	@const	A					The [A] key. Most MOAIKeyboardSensor functions will accept the string "a" instead of this constant.
	@const	B					The [B] key. Most MOAIKeyboardSensor functions will accept the string "b" instead of this constant.
	@const	C					The [C] key. Most MOAIKeyboardSensor functions will accept the string "c" instead of this constant.
	@const	D					The [D] key. Most MOAIKeyboardSensor functions will accept the string "d" instead of this constant.
	@const	E					The [E] key. Most MOAIKeyboardSensor functions will accept the string "e" instead of this constant.
	@const	F					The [F] key. Most MOAIKeyboardSensor functions will accept the string "f" instead of this constant.
	@const	G					The [G] key. Most MOAIKeyboardSensor functions will accept the string "g" instead of this constant.
	@const	H					The [H] key. Most MOAIKeyboardSensor functions will accept the string "h" instead of this constant.
	@const	I					The [I] key. Most MOAIKeyboardSensor functions will accept the string "i" instead of this constant.
	@const	J					The [J] key. Most MOAIKeyboardSensor functions will accept the string "j" instead of this constant.
	@const	K					The [K] key. Most MOAIKeyboardSensor functions will accept the string "k" instead of this constant.
	@const	L					The [L] key. Most MOAIKeyboardSensor functions will accept the string "l" instead of this constant.
	@const	M					The [M] key. Most MOAIKeyboardSensor functions will accept the string "m" instead of this constant.
	@const	N					The [N] key. Most MOAIKeyboardSensor functions will accept the string "n" instead of this constant.
	@const	O					The [O] key. Most MOAIKeyboardSensor functions will accept the string "o" instead of this constant.
	@const	P					The [P] key. Most MOAIKeyboardSensor functions will accept the string "p" instead of this constant.
	@const	Q					The [Q] key. Most MOAIKeyboardSensor functions will accept the string "q" instead of this constant.
	@const	R					The [R] key. Most MOAIKeyboardSensor functions will accept the string "r" instead of this constant.
	@const	S					The [S] key. Most MOAIKeyboardSensor functions will accept the string "s" instead of this constant.
	@const	T					The [T] key. Most MOAIKeyboardSensor functions will accept the string "t" instead of this constant.
	@const	U					The [U] key. Most MOAIKeyboardSensor functions will accept the string "u" instead of this constant.
	@const	V					The [V] key. Most MOAIKeyboardSensor functions will accept the string "v" instead of this constant.
	@const	W					The [W] key. Most MOAIKeyboardSensor functions will accept the string "w" instead of this constant.
	@const	X					The [X] key. Most MOAIKeyboardSensor functions will accept the string "x" instead of this constant.
	@const	Y					The [Y] key. Most MOAIKeyboardSensor functions will accept the string "y" instead of this constant.
	@const	Z					The [Z] key. Most MOAIKeyboardSensor functions will accept the string "z" instead of this constant.
	@const	GUI					The [Windows] key on a PC keyboard; the [Apple] key on a Mac keyboard.
	@const	APPLICATION			The Windows context menu key.
	@const	NUM_0				The [0] key on the numeric keypad.
	@const	NUM_1				The [1] key on the numeric keypad.
	@const	NUM_2				The [2] key on the numeric keypad.
	@const	NUM_3				The [3] key on the numeric keypad.
	@const	NUM_4				The [4] key on the numeric keypad.
	@const	NUM_5				The [5] key on the numeric keypad.
	@const	NUM_6				The [6] key on the numeric keypad.
	@const	NUM_7				The [7] key on the numeric keypad.
	@const	NUM_8				The [8] key on the numeric keypad.
	@const	NUM_9				The [9] key on the numeric keypad.
	@const	NUM_MULTIPLY		The [*] key on the numeric keypad.
	@const	NUM_PLUS			The [+] key on the numeric keypad.
	@const	NUM_MINUS			The [-] key on the numeric keypad.
	@const	NUM_DECIMAL			The [.] key on the numeric keypad. Note that on a non-US keyboard, this key may have a different label.
	@const	NUM_DIVIDE			The [/] key on the numeric keypad. Note that on a non-US keyboard, this key may have a different label.
	@const	F1					The [F1] key.
	@const	F2					The [F2] key.
	@const	F3					The [F3] key.
	@const	F4					The [F4] key.
	@const	F5					The [F5] key.
	@const	F6					The [F6] key.
	@const	F7					The [F7] key.
	@const	F8					The [F8] key.
	@const	F9					The [F9] key.
	@const	F10					The [F10] key.
	@const	F11					The [F11] key.
	@const	F12					The [F12] key.
	@const	NUM_LOCK			The [Num Lock] key.
	@const	SCROLL_LOCK			The [Scroll Lock] key.
	@const	OEM_1				This key has different functions depending on the keyboard layout. For instance, this is the [;] key on a US keyboard; the [Ü] key on a German keyboard; the [$] key on a French keyboard. Most MOAIKeyboardSensor functions will accept the string ";" instead of this constant.
	@const	OEM_PLUS			This is the key that creates the "+" character. On some keyboard layouts (including US), this requires holding the [Shift] key. Most MOAIKeyboardSensor functions will accept the string "=" instead of this constant.
	@const	OEM_COMMA			This is the key that creates the "," character. On some keyboard layouts, this requires holding the [Shift] key. Most MOAIKeyboardSensor functions will accept the string "," instead of this constant.
	@const	OEM_MINUS			This is the key that creates the "-" character. On some keyboard layouts, this requires holding the [Shift] key. Most MOAIKeyboardSensor functions will accept the string "-" instead of this constant.
	@const	OEM_PERIOD			This is the key that creates the "." character. On some keyboard layouts, this requires holding the [Shift] key. Most MOAIKeyboardSensor functions will accept the string "." instead of this constant.
	@const	OEM_2				This key has different functions depending on the keyboard layout. For instance, this is the [/] key on a US keyboard; the [#] key on a German keyboard; the [:] key on a French keyboard. Most MOAIKeyboardSensor functions will accept the string "/" instead of this constant.
	@const	OEM_3				This key has different functions depending on the keyboard layout. For instance, this is the [`] key on a US keyboard; the [Ö] key on a German keyboard; the [Ù] key on a French keyboard. Most MOAIKeyboardSensor functions will accept the string "`" instead of this constant.
	@const	OEM_4				This key has different functions depending on the keyboard layout. For instance, this is the [[] key on a US keyboard; the [ß] key on a German keyboard; the [)] key on a French keyboard. Most MOAIKeyboardSensor functions will accept the string "[" instead of this constant.
	@const	OEM_5				This key has different functions depending on the keyboard layout. For instance, this is the [\] key on a US keyboard; the [^] key on a German keyboard; the [*] key on a French keyboard. Most MOAIKeyboardSensor functions will accept the string "\\" instead of this constant.
	@const	OEM_6				This key has different functions depending on the keyboard layout. For instance, this is the []] key on a US keyboard; the [´] key on a German keyboard; the [^] key on a French keyboard. Most MOAIKeyboardSensor functions will accept the string "]" instead of this constant.
	@const	OEM_7				This key has different functions depending on the keyboard layout. For instance, this is the ['] key on a US keyboard; the [Ä] key on a German keyboard; the [²] key on a French keyboard. Most MOAIKeyboardSensor functions will accept the string "'" instead of this constant.
	@const	OEM_8				This key has different functions depending on the keyboard layout. For instance, this is the [!] key on a French keyboard.
	@const	OEM_102				This is either the [<] key or the [\] key on the RT 102-key keyboard.
*/
class MOAIKeyCode :
	public MOAIGlobalClass < MOAIKeyCode, MOAILuaObject > {
public:
	
	DECL_LUA_SINGLETON ( MOAIKeyCode )
	
	//----------------------------------------------------------------//
	void				RegisterLuaClass		( MOAILuaState& state );
};
#endif