// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAINODEHARNESS_H
#define	MOAINODEHARNESS_H

class MOAINode;

//================================================================//
// MOAIManipulator
//================================================================//
/**	@brief Hook for binding primitives to tools.
*/
class MOAIManipulator :
	public USAttributed {
protected:

	USVec2D			mLoc;
	float			mAngle;
	USVec2D			mScale;

public:

	enum {
		ATTR_X_LOC,
		ATTR_Y_LOC,
		ATTR_Z_ROT,
		ATTR_X_SCL,
		ATTR_Y_SCL,
		TOTAL_ATTR,
	};
	
	GET_SET ( USVec2D, Loc, mLoc )
	GET_SET ( float, Angle, mAngle )
	GET_SET ( USVec2D, Scale, mScale )
	
	//----------------------------------------------------------------//
	void			ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
	u32				CountAttributes			();
					MOAIManipulator			();
					~MOAIManipulator			();
};

//================================================================//
// MOAINodeHarness
//================================================================//
class MOAINodeHarness :
	public virtual USAttributed {
protected:

	USRef < MOAINode >	mMOAINode;
	USAttrMap			mManipulatorAttrMap;

	//----------------------------------------------------------------//
	virtual void		Clear					() = 0;

public:
	
	//----------------------------------------------------------------//
	virtual void		AddFloatAttribute		( cc8* name, u32 id, float value ) = 0;
	void				BindNode				( MOAINode* node );
	u32					CountAttributes			();
						MOAINodeHarness			();
	virtual				~MOAINodeHarness		();
	void				MapManipulatorAttr		( u32 nodeAttrID, u32 manipAttrID );
	void				ReadManipulator			( MOAIManipulator& manipulator );
	void				SetFlags				( u32 flags );
	void				UnbindNode				();
	void				WriteManipulator		( MOAIManipulator& manipulator );
	STLString			ToString				();
};

#endif
