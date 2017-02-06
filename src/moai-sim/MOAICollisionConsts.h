// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONCONSTS_H
#define	MOAICOLLISIONCONSTS_H

#define COLLISION_COMBO_CODE(shapeType0, shapeType1) (( 0x00000001 << shapeType0 ) | ( 0x00010000 << shapeType1 ))

//================================================================//
// MOAICollisionConsts
//================================================================//
namespace MOAICollisionConsts {

	enum {
		BOX,		// 3D (AXIS-ALIGNED)
		CIRCLE,		// 2D (AXIS-ALIGNED)
		FRUSTUM,	// 3D
		POLYGON,	// 2D
		PRISM,		// 3D
		QUAD,		// 2D
		RECT,		// 2D (AXIS-ALIGNED)
		SPHERE,		// 3D (AXIS-ALIGNED)
		NONE,
	};

	static const u32 BOX__BOX				= COLLISION_COMBO_CODE ( BOX, BOX );
	static const u32 BOX__FRUSTUM			= COLLISION_COMBO_CODE ( BOX, FRUSTUM );
	static const u32 BOX__PRISM				= COLLISION_COMBO_CODE ( BOX, PRISM );
	static const u32 BOX__SPHERE			= COLLISION_COMBO_CODE ( BOX, SPHERE );

	static const u32 CIRCLE__CIRCLE			= COLLISION_COMBO_CODE ( CIRCLE, CIRCLE );
	static const u32 CIRCLE__POLYGON		= COLLISION_COMBO_CODE ( CIRCLE, POLYGON );
	static const u32 CIRCLE__QUAD			= COLLISION_COMBO_CODE ( CIRCLE, QUAD );
	static const u32 CIRCLE__RECT			= COLLISION_COMBO_CODE ( CIRCLE, RECT );

	static const u32 FRUSTUM__BOX			= COLLISION_COMBO_CODE ( FRUSTUM, BOX );
	static const u32 FRUSTUM__FRUSTUM		= COLLISION_COMBO_CODE ( FRUSTUM, FRUSTUM );
	static const u32 FRUSTUM__PRISM			= COLLISION_COMBO_CODE ( FRUSTUM, PRISM );
	static const u32 FRUSTUM__SPHERE		= COLLISION_COMBO_CODE ( FRUSTUM, SPHERE );

	static const u32 POLYGON__CIRCLE		= COLLISION_COMBO_CODE ( POLYGON, CIRCLE );
	static const u32 POLYGON__POLYGON		= COLLISION_COMBO_CODE ( POLYGON, POLYGON );
	static const u32 POLYGON__QUAD			= COLLISION_COMBO_CODE ( POLYGON, QUAD );
	static const u32 POLYGON__RECT			= COLLISION_COMBO_CODE ( POLYGON, RECT );
	
	static const u32 PRISM__BOX				= COLLISION_COMBO_CODE ( PRISM, BOX );
	static const u32 PRISM__FRUSTUM			= COLLISION_COMBO_CODE ( PRISM, FRUSTUM );
	static const u32 PRISM__PRISM			= COLLISION_COMBO_CODE ( PRISM, PRISM );
	static const u32 PRISM__SPHERE			= COLLISION_COMBO_CODE ( PRISM, SPHERE );
	
	static const u32 QUAD__CIRCLE			= COLLISION_COMBO_CODE ( QUAD, CIRCLE );
	static const u32 QUAD__POLYGON			= COLLISION_COMBO_CODE ( QUAD, POLYGON );
	static const u32 QUAD__QUAD				= COLLISION_COMBO_CODE ( QUAD, QUAD );
	static const u32 QUAD__RECT				= COLLISION_COMBO_CODE ( QUAD, RECT );

	static const u32 RECT__CIRCLE			= COLLISION_COMBO_CODE ( RECT, CIRCLE );
	static const u32 RECT__POLYGON			= COLLISION_COMBO_CODE ( RECT, POLYGON );
	static const u32 RECT__QUAD				= COLLISION_COMBO_CODE ( RECT, QUAD );
	static const u32 RECT__RECT				= COLLISION_COMBO_CODE ( RECT, RECT );

	static const u32 SPHERE__BOX			= COLLISION_COMBO_CODE ( SPHERE, BOX );
	static const u32 SPHERE__FRUSTUM		= COLLISION_COMBO_CODE ( SPHERE, FRUSTUM );
	static const u32 SPHERE__PRISM			= COLLISION_COMBO_CODE ( SPHERE, PRISM );
	static const u32 SPHERE__SPHERE			= COLLISION_COMBO_CODE ( SPHERE, SPHERE );
};

#endif
