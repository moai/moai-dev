
#ifndef MOAIFREETYPETEXTBOX_H
#define MOAIFREETYPETEXTBOX_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIProp.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>

class MOAIFreeTypeFont;
class MOAITexture;

using namespace std;

struct MOAIFreeTypeTextLine {
	int lineWidth;
	wchar_t* text;
};

struct MOAIFreeTypeImageBuffer {
	u32 width;
	u32 height;
	void *data;
};

/** @name	MOAIFreeTypeTextBox
	@text	An alternative to MOAITextBox that uses an instance of MOAIFreeTypeFont directly.
 */

class MOAIFreeTypeTextBox : public MOAIGlobalClass < MOAIFreeTypeTextBox, MOAILuaObject > {
private:

//----------------------------------------------------------------//
	static int _generateLabelTexture	( lua_State* L );
	
public:

	DECL_LUA_SINGLETON( MOAIFreeTypeTextBox )
	
	static MOAITexture*	GenerateTexture( cc8* text, MOAIFreeTypeFont *font, int size, int width, int height, int alignment, int wordbreak );
	
						MOAIFreeTypeTextBox		();
						~MOAIFreeTypeTextBox	();
	void				RegisterLuaClass		( MOAILuaState& state );
};

#endif /* defined(MOAIFREETYPETEXTBOX_H) */
