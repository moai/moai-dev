//[MOAI LICENSE]

#ifndef	MOAIQUAD_H
#define	MOAIQUAD_H

#include <moaicore/MOAIGfxPrim2D.h>
#include <moaicore/MOAIOverlapPrim2D.h>

class MOAIMultiShape2D;

//================================================================//
// MOAIQuad
//================================================================//
/**	@brief Quadrilateral for collisions and overlap.
*/
class MOAIQuad :
	public MOAIOverlapPrim2D {
private:
	
	USQuad			mQuad;
	
	//----------------------------------------------------------------//
	static int		_inside					( lua_State* L );
	static int		_setQuad				( lua_State* L );
	static int		_setRect				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			OnDepNodeUpdate			();

public:

	friend class MOAICollisionMgr;
	
	enum {
		TOTAL_ATTR = MOAITransform2D::TOTAL_ATTR,
	};
	
	DECL_LUA_DATA ( MOAIQuad )
	
	//----------------------------------------------------------------//
	void				ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
	u32					CountAttributes			();
	void				DrawDebug				();
	u32					GetLocalFrame			( USRect& frame );
	void				GetShape				( MOAIMultiShape2D& shape );
	bool				Inside					( USVec2D vec );
	MOAIOverlapPrim2D*	IsOverlapPrim2D			();
						MOAIQuad				();
						~MOAIQuad				();
	void				RegisterLuaClass		( USLuaState& state );
	void				RegisterLuaFuncs		( USLuaState& state );
	STLString			ToString				();
};

#endif
