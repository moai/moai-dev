// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPLATFORMERDYNAMICS2D_STATES_H
#define	MOAIPLATFORMERDYNAMICS2D_STATES_H

#include <moaicore/MOAIFsm.h>
#include <moaicore/MOAIPlatformerDynamics2D.h>

//================================================================//
// MOAIPlatformerDynamics2D_Fall
//================================================================//
class MOAIPlatformerDynamics2D_Fall :
	public MOAIFsmState < MOAIPlatformerDynamics2D > {
private:

	//----------------------------------------------------------------//
	void	OnBegin		( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self );
	void	OnEnd		( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self );
	float	OnUpdate	( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self, float step );
};

//================================================================//
// MOAIPlatformerDynamics2D_Idle
//================================================================//
class MOAIPlatformerDynamics2D_Idle :
	public MOAIFsmState < MOAIPlatformerDynamics2D > {
private:

	//----------------------------------------------------------------//
	void	OnBegin		( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self );
	void	OnEnd		( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self );
	float	OnUpdate	( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self, float step );
};

//================================================================//
// MOAIPlatformerDynamics2D_Jump
//================================================================//
class MOAIPlatformerDynamics2D_Jump :
	public MOAIFsmState < MOAIPlatformerDynamics2D > {
private:

	//----------------------------------------------------------------//
	void	OnBegin		( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self );
	void	OnEnd		( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self );
	float	OnUpdate	( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self, float step );
};

//================================================================//
// MOAIPlatformerDynamics2D_Fall
//================================================================//
	
//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D_Fall::OnBegin ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
	MOAIPlatformerBody2D* body = self.mBody;
	
	self.mStateTimer = 0.0f;
	self.mStateY = self.mFallArc.Evaluate ( 0.0f );
	
	if ( body->IsStanding ()) {
		fsm.SetState < MOAIPlatformerDynamics2D_Idle >();
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D_Fall::OnEnd ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
	UNUSED ( fsm );
	UNUSED ( self );
}

//----------------------------------------------------------------//
float MOAIPlatformerDynamics2D_Fall::OnUpdate ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self, float step ) {
	UNUSED ( fsm );
	
	float stateY = self.mFallArc.Evaluate ( self.mStateTimer + step );
	float yMove = stateY - self.mStateY;
	self.mStateY = stateY;
	
	self.AccumulateMove ( 0.0f, yMove );
	self.UpdateXMove ( step );
	return 0.0f;
}

//================================================================//
// MOAIPlatformerDynamics2D_Idle
//================================================================//
	
//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D_Idle::OnBegin ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
	MOAIPlatformerBody2D* body = self.mBody;
	
	if ( !body->IsStanding ()) {
		fsm.SetState < MOAIPlatformerDynamics2D_Fall >();
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D_Idle::OnEnd ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
	UNUSED ( fsm );
	UNUSED ( self );
}

//----------------------------------------------------------------//
float MOAIPlatformerDynamics2D_Idle::OnUpdate ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self, float step ) {
	UNUSED ( fsm );

	self.UpdateXMove ( step );
	return 0.0f;
}

//================================================================//
// MOAIPlatformerDynamics2D_Jump
//================================================================//
	
//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D_Jump::OnBegin ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
	UNUSED ( fsm );
	
	self.mStateTimer = 0.0f;
	self.mStateY = self.mJumpArc.Evaluate ( 0.0f );
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D_Jump::OnEnd ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
	UNUSED ( fsm );
	UNUSED ( self );
}

//----------------------------------------------------------------//
float MOAIPlatformerDynamics2D_Jump::OnUpdate ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self, float step ) {
	
	float evalStep = step;
		
	if (( self.mStateTimer + step ) >= self.mJumpArc.mDuration ) {
		evalStep = self.mJumpArc.mDuration - self.mStateTimer;
		fsm.SetState < MOAIPlatformerDynamics2D_Fall >();
	}
	
	float stateY = self.mJumpArc.Evaluate ( self.mStateTimer + evalStep );
	float yMove = stateY - self.mStateY;
	self.mStateY = stateY;
	
	self.AccumulateMove ( 0.0f, yMove );
	self.UpdateXMove ( step );
	return step - evalStep;
}

#endif
