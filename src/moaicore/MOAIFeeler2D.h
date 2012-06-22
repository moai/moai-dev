//[MOAI LICENSE]

#ifndef	MOAIFEELER2D_H
#define	MOAIFEELER2D_H

#include <moaicore/MOAIPrim.h>

class MOAISurfaceHit2D;
class MOAIPartition;
class MOAISurfaceSampler2D;

class MOAIFeelerState2D;


class MOAIGrid;

//================================================================//
// MOAIFeeler2D
//================================================================//
/**	@brief 2D collision feeler.
*/
class MOAIFeeler2D :
	public virtual MOAIPrim {
private:
	
	float		mFloorCos;	// mY must be >= to count as floor
	float		mCeilCos;	// mY must be <= to count as ceil
	
	bool		mTouchFeet;
	bool		mTouchHead;
	bool		mTouchRight;
	bool		mTouchLeft;
	
	bool		mIsStanding;
	
	USVec2D		mMove;
	float		mHRad;
	float		mVRad;
	
	float		mSkirt;
	float		mHat;
	
	USVec2D		mAttachPoint; // point of attachment to platform (local space)
	USRef < MOAITransform2D > mAttachTarget;
	
	//----------------------------------------------------------------//
	static int		_isStanding				( lua_State* L );
	static int		_setMove				( lua_State* L );
	static int		_setSize				( lua_State* L );
	static int		_setSkirt				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			Attach					( MOAITransform2D* transform );
	void			Detach					();
	void			DrawSurfaces			( MOAISurfaceSampler2D& sampler );
	void			GatherSurfacesForMove	( MOAISurfaceSampler2D& sampler, USVec2D& move );
	void			GetNextHit				( MOAISurfaceSampler2D& sampler, USVec2D& loc, USVec2D& move, MOAISurfaceHit2D& hit );
	bool			GetNextMove				( MOAISurfaceSampler2D& sampler, USVec2D& loc, USVec2D& move, MOAISurfaceHit2D& hit );
	void			GetRect					( USRect& rect );
	void			GetSweptRect			( USVec2D& move, USRect& rect );
	void			GetTouching				( USVec2D& loc, MOAISurfaceSampler2D& sampler );
	void			GetWorldMtx				( USMatrix2D& transform );
	void			GetWorldMtxInv			( USMatrix2D& transform );
	void			Move					();
	void			SetAngles				( float floorAngle, float ceilAngle );
	void			SnapUp					( MOAISurfaceSampler2D& sampler, USVec2D& loc, USVec2D& move, MOAISurfaceHit2D& hit );
	void			TransformToLocal		( USVec2D& point );
	void			TransformToWorld		( USVec2D& point );

public:

	enum {
		ATTR_X_VELOCITY = MOAITransform2D::TOTAL_ATTR,
		ATTR_Y_VELOCITY,
		TOTAL_ATTR,
	};
	
	DECL_LUA_DATA ( MOAIFeeler2D )
	
	//----------------------------------------------------------------//
	void			ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
	u32				CountAttributes			();
	void			DrawDebug				();
	u32				GetLocalFrame			( USRect& frame );
					MOAIFeeler2D			();
					~MOAIFeeler2D			();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			SetMove					( float x, float y );
	STLString		ToString				();
};

#endif
