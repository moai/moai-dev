//[MOAI LICENSE]

#ifndef	MOAIRECT_H
#define	MOAIRECT_H

#include <moaicore/MOAIGfxPrim2D.h>
#include <moaicore/MOAIOverlapPrim2D.h>

class MOAIMultiShape2D;

//================================================================//
// MOAIRect
//================================================================//
/**	@brief Axis-aligned rectange for collisions and overlap.
*/
class MOAIRect :
	public MOAIOverlapPrim2D {
private:
	
	USRect			mRect;
	
	//----------------------------------------------------------------//
	static int		_inside					( lua_State* L );
	static int		_setRect				( lua_State* L );
	
public:
	
	enum {
		TOTAL_ATTR = MOAITransform2D::TOTAL_ATTR,
	};
	
	DECL_LUA_DATA ( MOAIRect )
	
	//----------------------------------------------------------------//
	void				ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
	u32					CountAttributes			();
	void				DrawDebug				();
	u32					GetLocalFrame			( USRect& frame );
	void				GetShape				( MOAIMultiShape2D& shape );
	bool				Inside					( USVec2D vec );
	MOAIOverlapPrim2D*	IsOverlapPrim2D			();
						MOAIRect				();
						~MOAIRect				();
	void				RegisterLuaClass		( USLuaState& state );
	void				RegisterLuaFuncs		( USLuaState& state );
	STLString			ToString				();
};

#endif
