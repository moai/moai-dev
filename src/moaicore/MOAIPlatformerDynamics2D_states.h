// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPLATFORMERDYNAMICS2D_STATES_H
#define	MOAIPLATFORMERDYNAMICS2D_STATES_H

#include <moaicore/MOAIFsm.h>
#include <moaicore/MOAIPlatformerDynamics2D.h>

//================================================================//
// MOAIPlatformerDynamics2D_Idle
//================================================================//
class MOAIPlatformerDynamics2D_Idle :
	public MOAIFsmState < MOAIPlatformerDynamics2D > {
private:
	
	//----------------------------------------------------------------//
	void OnBegin ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
		UNUSED ( fsm );
		UNUSED ( self );
		printf ( "MOAIPlatformerDynamics2D_Idle OnBegin\n" );
	}
	
	//----------------------------------------------------------------//
	void OnEnd ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
		UNUSED ( fsm );
		UNUSED ( self );
		printf ( "MOAIPlatformerDynamics2D_Idle OnEnd\n" );
	}
	
	//----------------------------------------------------------------//
	MOAIFsmTrans::Type OnUpdate ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
		UNUSED ( fsm );
		UNUSED ( self );
		printf ( "MOAIPlatformerDynamics2D_Idle OnUpdate\n" );
		return MOAIFsmTrans::END;
	}
};

//================================================================//
// MOAIPlatformerDynamics2D_JumpArc
//================================================================//
class MOAIPlatformerDynamics2D_JumpArc :
	public MOAIFsmState < MOAIPlatformerDynamics2D > {
private:
	
	//----------------------------------------------------------------//
	void OnBegin ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
		UNUSED ( fsm );
		UNUSED ( self );
		printf ( "MOAIPlatformerDynamics2D_JumpArc OnBegin\n" );
	}
	
	//----------------------------------------------------------------//
	void OnEnd ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
		UNUSED ( fsm );
		UNUSED ( self );
		printf ( "MOAIPlatformerDynamics2D_JumpArc OnEnd\n" );
	}
	
	//----------------------------------------------------------------//
	MOAIFsmTrans::Type OnUpdate ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
		UNUSED ( fsm );
		UNUSED ( self );
		printf ( "MOAIPlatformerDynamics2D_JumpArc OnUpdate\n" );
		return MOAIFsmTrans::END;
	}
};

#endif
