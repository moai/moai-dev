// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPLATFORMERDYNAMICS2D_STATES_H
#define	MOAIPLATFORMERDYNAMICS2D_STATES_H

#include <moaicore/MOAIFsm.h>
#include <moaicore/MOAIPlatformerDynamics2D.h>

//================================================================//
// MOAIPlatformerDynamics2D_Boost
//================================================================//
class MOAIPlatformerDynamics2D_Boost :
	public MOAIFsmState < MOAIPlatformerDynamics2D > {
private:

	friend class MOAIPlatformerDynamics2D;
	
	float	mDuration;

	//----------------------------------------------------------------//
	void	OnBegin		( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self );
	void	OnEnd		( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self );
	float	OnUpdate	( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self, float step );
};

//================================================================//
// MOAIPlatformerDynamics2D_Fall
//================================================================//
class MOAIPlatformerDynamics2D_Fall :
	public MOAIFsmState < MOAIPlatformerDynamics2D > {
private:

	friend class MOAIPlatformerDynamics2D;

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

	friend class MOAIPlatformerDynamics2D;

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

	friend class MOAIPlatformerDynamics2D;

	//----------------------------------------------------------------//
	void	OnBegin		( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self );
	void	OnEnd		( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self );
	float	OnUpdate	( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self, float step );
};

//================================================================//
// MOAIPlatformerDynamics2D_Boost
//================================================================//
	
//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D_Boost::OnBegin ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
	UNUSED ( fsm );
	self.mState = MOAIPlatformerDynamics2D::STATE_BOOST;
	self.mStateTimer = -this->mDuration;
	self.mStateY = self.mJumpArc.Evaluate ( self.mStateTimer );
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D_Boost::OnEnd ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
	UNUSED ( fsm );
	UNUSED ( self );
}

//----------------------------------------------------------------//
float MOAIPlatformerDynamics2D_Boost::OnUpdate ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self, float step ) {
	UNUSED ( fsm );
	
	float evalStep = step;
		
	if (( self.mStateTimer + step ) >= 0.0f ) {
		evalStep = -self.mStateTimer;
		fsm.SetState < MOAIPlatformerDynamics2D_Jump >();
	}
	
	float stateY = self.mJumpArc.Evaluate ( self.mStateTimer + evalStep );
	float yMove = stateY - self.mStateY;
	self.mStateY = stateY;
	
	self.AccumulateMove ( 0.0f, yMove );
	self.UpdateXMove ( step );
	return step - evalStep;
}

//================================================================//
// MOAIPlatformerDynamics2D_Fall
//================================================================//
	
//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D_Fall::OnBegin ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
	UNUSED ( fsm );
	
	self.mState = MOAIPlatformerDynamics2D::STATE_FALL;
	self.mStateTimer = 0.0f;
	self.mStateY = self.mFallArc.Evaluate ( 0.0f );
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D_Fall::OnEnd ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
	UNUSED ( fsm );
	UNUSED ( self );
}

//----------------------------------------------------------------//
float MOAIPlatformerDynamics2D_Fall::OnUpdate ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self, float step ) {
	UNUSED ( fsm );
	MOAIPlatformerBody2D* body = self.mBody;
	
	if ( body->IsStanding ()) {
		fsm.SetState < MOAIPlatformerDynamics2D_Idle >();
	}
	
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
	UNUSED ( fsm );
	
	self.mState = MOAIPlatformerDynamics2D::STATE_IDLE;
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D_Idle::OnEnd ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
	UNUSED ( fsm );
	UNUSED ( self );
}

//----------------------------------------------------------------//
float MOAIPlatformerDynamics2D_Idle::OnUpdate ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self, float step ) {
	UNUSED ( fsm );
	MOAIPlatformerBody2D* body = self.mBody;

	if ( !body->IsStanding ()) {
		fsm.SetState < MOAIPlatformerDynamics2D_Fall >();
	}

	self.UpdateXMove ( step );
	return 0.0f;
}

//================================================================//
// MOAIPlatformerDynamics2D_Jump
//================================================================//
	
//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D_Jump::OnBegin ( MOAIFsm& fsm, MOAIPlatformerDynamics2D& self ) {
	UNUSED ( fsm );
	
	self.mState = MOAIPlatformerDynamics2D::STATE_JUMP;
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
