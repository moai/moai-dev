#ifndef	MOAITEXTURERAW_H
#define	MOAITEXTURERAW_H

#include <moaicore/MOAITextureBase.h>

class MOAIDataBuffer;

//================================================================//
// MOAITextureRaw
//================================================================//
/**	@name	MOAITextureRaw
 @text	Texture class for raw 32-BGRA data.
 */
class MOAITextureRaw :
public MOAITextureBase {
	private:

	void*				mData;
	u32					mFrameWidth;
	u32					mFrameHeight;

	//----------------------------------------------------------------//
	static int			_load					( lua_State* L );

	//----------------------------------------------------------------//
	bool				IsRenewable				();
	void				OnClear					();
	void				OnCreate				();
	void				OnLoad					();

	public:

	DECL_LUA_FACTORY ( MOAITextureRaw )

	//----------------------------------------------------------------//
	static MOAIGfxState*	AffirmTexture			( MOAILuaState& state, int idx );
	bool					Init 					( MOAILuaState& state, int idx );
	void					Init					( MOAIDataBuffer& data, cc8* debugname );

	MOAITextureRaw				();
	virtual					~MOAITextureRaw			();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
