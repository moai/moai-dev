// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFSM_H
#define	MOAIFSM_H

#include <moaicore/MOAIRtti.h>

class MOAIFsm;

//================================================================//
// MOAIFsmTrans
//================================================================//
namespace MOAIFsmTrans {

	enum Type {
		CONTINUE,	// Continue the update processing with the next pending state.
		YIELD,		// Stop update processing until the next update step.
		END			// Stop update processing and clear the MOAIFsm.
	};
}

//================================================================//
// MOAIFsmStateBase
//================================================================//
class MOAIFsmStateBase :
	public RTTIBase {
private:

	//----------------------------------------------------------------//
	virtual void				Begin			( MOAIFsm& fsm ) = 0;
	virtual void				End				( MOAIFsm& fsm ) = 0;
	virtual bool				SetSelf			( RTTIBase* self ) = 0;
	virtual MOAIFsmTrans::Type	Update			( MOAIFsm& fsm ) = 0;

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
		if ( this->mSelf ) {
			this->OnBegin ( fsm, *this->mSelf );
		}
	}
	
	//----------------------------------------------------------------//
	void End ( MOAIFsm& fsm ) {
		if ( this->mSelf ) {
			this->OnEnd ( fsm, *this->mSelf );
		}
	}

	//----------------------------------------------------------------//
	bool SetSelf ( RTTIBase* self ) {
	
		this->mSelf = self->AsType < TYPE >();
		return this->mSelf != 0;
	}

	//----------------------------------------------------------------//
	MOAIFsmTrans::Type Update ( MOAIFsm& fsm ) {
		if ( this->mSelf ) {
			return this->OnUpdate ( fsm, *this->mSelf );
		}
		return MOAIFsmTrans::END;
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
	virtual MOAIFsmTrans::Type OnUpdate ( MOAIFsm& fsm, TYPE& self ) {
		UNUSED ( fsm );
		UNUSED ( self );
		return MOAIFsmTrans::END;
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
		
		bool loop = true;
		while ( loop ) {
			
			if ( !this->mNextState ) return;
			
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
	template < typename STATE_TYPE >
	STATE_TYPE& SetState ( RTTIBase& self ) {
		
		this->mSelf = &self;
		USUnion < MOAIFsmStateBase >& buffer = mBuffers [ this->mID ];
		MOAIFsmStateBase& state = buffer.Set < STATE_TYPE >();
		this->mNextState = &state;
		return ( STATE_TYPE& )state;
	}
	
	//----------------------------------------------------------------//
	void Update () {
		
		bool loop = true;
		while ( loop ) {
			
			this->GotoNextState ();
			if ( !this->mState ) return;
			
			u32 result = this->mState->Update ( *this );
			
			switch ( result) {
				
				case MOAIFsmTrans::YIELD: {
					loop = false;
					return;
				}
				
				case MOAIFsmTrans::END: {
					this->mState->End ( *this );
					this->mState = 0;
					return;
				}
			}
		}
	}
};

#endif
