// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFSM_H
#define	MOAIFSM_H

#include <moaicore/MOAIRtti.h>

class MOAIFsm;

//================================================================//
// MOAIFsmStateBase
//================================================================//
class MOAIFsmStateBase :
	public RTTIBase {
private:

	//----------------------------------------------------------------//
	virtual void		Begin			( MOAIFsm& fsm ) = 0;
	virtual void		End				( MOAIFsm& fsm ) = 0;
	virtual bool		SetSelf			( RTTIBase* self ) = 0;
	virtual float		Update			( MOAIFsm& fsm, float step ) = 0;

public:

	friend class MOAIFsm;
	
	//----------------------------------------------------------------//
	MOAIFsmStateBase () {
		this->SimpleRTTI < RTTIBase >( this );
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIFsmStateBase () {
	}
};

//================================================================//
// MOAIFsmState
//================================================================//
template < typename	TYPE >
class MOAIFsmState :
	public MOAIFsmStateBase {
private:

	friend class MOAIFsm;

	TYPE* mSelf;

	//----------------------------------------------------------------//
	void Begin ( MOAIFsm& fsm ) {
		assert ( this->mSelf );
		this->OnBegin ( fsm, *this->mSelf );
	}
	
	//----------------------------------------------------------------//
	void End ( MOAIFsm& fsm ) {
		assert ( this->mSelf );
		this->OnEnd ( fsm, *this->mSelf );
	}

	//----------------------------------------------------------------//
	bool SetSelf ( RTTIBase* self ) {
		this->mSelf = self->AsType < TYPE >();
		return this->mSelf != 0;
	}

	//----------------------------------------------------------------//
	float Update ( MOAIFsm& fsm, float step ) {
		assert ( this->mSelf );
		return this->OnUpdate ( fsm, *this->mSelf, step );
	}

public:
	
	//----------------------------------------------------------------//
	virtual void OnBegin ( MOAIFsm& fsm, TYPE& self ) {
		UNUSED ( fsm );
		UNUSED ( self );
	}
	
	//----------------------------------------------------------------//
	virtual void OnEnd ( MOAIFsm& fsm, TYPE& self ) {
		UNUSED ( fsm );
		UNUSED ( self );
	}
	
	//----------------------------------------------------------------//
	virtual float OnUpdate ( MOAIFsm& fsm, TYPE& self, float step ) {
		UNUSED ( fsm );
		UNUSED ( self );
		UNUSED ( step );
		return 0.0f;
	}
	
	//----------------------------------------------------------------//
	MOAIFsmState () :
		mSelf ( 0 ) {
		this->SimpleRTTI < MOAIFsmStateBase >( this );
	}
};

//================================================================//
// MOAIFsm
//================================================================//
class MOAIFsm {
private:

	USUnion < MOAIFsmStateBase > mBuffers [ 2 ];
	u32 mID;

	MOAIFsmStateBase* mState;
	MOAIFsmStateBase* mNextState;

	RTTIBase* mSelf;

	//----------------------------------------------------------------//
	u32 GetNextID () {
		return (( this->mID + 1 ) & 0x01 );
	}

	//----------------------------------------------------------------//
	void GotoNextState () {
		
		while ( this->mNextState ) {
			
			if ( !this->mNextState->SetSelf ( this->mSelf )) {
				this->Clear ();
				return;
			}
			
			// finish the current state
			if ( this->mState ) {
				this->mState->End ( *this );
				this->mState = 0;
			}
			
			// start the next state
			this->mState = this->mNextState;
			this->mNextState = 0;
			
			// next...
			this->mID = this->GetNextID ();
			
			this->mState->Begin ( *this );	
		}
	}

public:
	
	//----------------------------------------------------------------//
	void Clear () {
		
		if ( this->mState ) {
			this->mState->End ( *this );
		}
		this->mState = 0;
		this->mNextState = 0;
		this->mID = 0;
	}
	
	//----------------------------------------------------------------//
	MOAIFsmStateBase* GetState () {
		return this->mState;
	}
	
	//----------------------------------------------------------------//
	template < typename STATE_TYPE >
	STATE_TYPE* GetState () {
		if ( this->mState ) {
			return this->mState->template AsType < STATE_TYPE >();
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	MOAIFsm () :
		mID ( 0 ),
		mState ( 0 ),
		mNextState ( 0 ),
		mSelf ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIFsm () {
	}
	
	//----------------------------------------------------------------//
	void SetSelf ( RTTIBase& self ) {
		this->mSelf = &self;
	}
	
	//----------------------------------------------------------------//
	template < typename STATE_TYPE >
	STATE_TYPE& SetState () {
		
		USUnion < MOAIFsmStateBase >& buffer = mBuffers [ this->mID ];
		MOAIFsmStateBase& state = buffer.Set < STATE_TYPE >();
		this->mNextState = &state;
		return ( STATE_TYPE& )state;
	}
	
	//----------------------------------------------------------------//
	void Update ( float step ) {
		
		if ( !this->mSelf ) return;
		
		do {
			this->GotoNextState ();
			if ( !this->mState ) return;
			step = this->mState->Update ( *this, step );
		}
		while ( step > 0.0f );
	}
};

#endif
