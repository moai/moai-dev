// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	FSM_H
#define	FSM_H

//================================================================//
// USFsmTrans
//================================================================//
namespace USFsmTrans {

	enum Type {
		CONTINUE,	// Continue the update processing with the next pending state.
		YIELD,		// Stop update processing until the next update step.
		END			// Stop update processing and clear the USFsm.
	};
}

//================================================================//
// USFsmStateBase
//================================================================//
class USFsmStateBase :
	public RTTIBase {
private:

	//----------------------------------------------------------------//
	virtual void				Begin			( RTTIBase* self ) = 0;
	virtual void				End				( RTTIBase*	self ) = 0;
	virtual bool				TypeMatch		( RTTIBase* self ) = 0;
	virtual USFsmTrans::Type	Update			( RTTIBase*	self, float step ) = 0;

public:

	friend class USFsm;
	
	//----------------------------------------------------------------//
	USFsmStateBase () {
		this->SimpleRTTI < RTTIBase >( this );
	}
	
	//----------------------------------------------------------------//
	virtual ~USFsmStateBase () {
	}
};

//================================================================//
// USFsmState
//================================================================//
template < typename	TYPE >
class USFsmState :
	public USFsmStateBase {
private:

	TYPE* mSelf;

	//----------------------------------------------------------------//
	void Begin ( RTTIBase* self ) {
	
		this->mSelf = self->AsType < TYPE >();
		if ( this->mSelf ) {
			this->OnBegin ( *this->mSelf );
		}
	}
	
	//----------------------------------------------------------------//
	void End ( RTTIBase* self ) {
		if ( this->mSelf ) {
			this->OnEnd ( *this->mSelf );
		}
	}

	//----------------------------------------------------------------//
	bool TypeMatch ( RTTIBase* self ) {
		return ( self->AsType < TYPE >() != 0 );
	}

	//----------------------------------------------------------------//
	USFsmTrans::Type Update ( RTTIBase* self, float step ) {
		if ( this->mSelf ) {
			return this->OnUpdate ( *this->mSelf, step );
		}
		return USFsmTrans::END;
	}

public:
	
	//----------------------------------------------------------------//
	virtual void OnBegin ( TYPE& self ) {
	}
	
	//----------------------------------------------------------------//
	virtual void OnEnd ( TYPE& self ) {
	}
	
	//----------------------------------------------------------------//
	virtual USFsmTrans::Type OnUpdate ( TYPE& self, float step ) {
		return USFsmTrans::END;
	}
	
	//----------------------------------------------------------------//
	USFsmState () :
		mSelf ( 0 ) {
		this->SimpleRTTI < USFsmStateBase >( this );
	}
};

//================================================================//
// USFsm
//================================================================//
class USFsm {
private:

	USUnion < USFsmStateBase > mBuffers [ 2 ];
	u32 mID;

	USFsmStateBase* mState;
	USFsmStateBase* mNextState;

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
			
			if ( !this->mNextState->TypeMatch ( this->mSelf )) {
				this->Clear ();
				return;
			}
			
			// finish the current state
			if ( this->mState ) {
				this->mState->End ( this->mSelf );
				this->mState = 0;
			}
			
			// start the next state
			this->mState = this->mNextState;
			this->mNextState = 0;
			
			// next...
			this->mID = this->GetNextID ();
			
			this->mState->Begin ( this->mSelf );	
		}
	}

public:
	
	//----------------------------------------------------------------//
	void Clear () {
		
		if ( this->mState ) {
			this->mState->End ( this->mSelf );
		}
		
		this->mState = 0;
		this->mNextState = 0;
		this->mID = 0;
	}
	
	//----------------------------------------------------------------//
	USFsmStateBase* GetState () {
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
	template < typename STATE_TYPE >
	STATE_TYPE& SetState ( RTTIBase& self ) {
		
		this->mSelf = &self;
		USUnion < USFsmStateBase >& buffer = mBuffers [ this->mID ];
		USFsmStateBase& state = buffer.Set < STATE_TYPE >();
		this->mNextState = &state;
		return ( STATE_TYPE& )state;
	}
	
	//----------------------------------------------------------------//
	void Update ( float step ) {
		
		bool loop = true;
		while ( loop ) {
			
			this->GotoNextState ();
			if ( !this->mState ) return;
			
			u32 result = this->mState->Update ( this->mSelf, step );
			
			switch ( result) {
				
				case USFsmTrans::YIELD: {
					loop = false;
					return;
				}
				
				case USFsmTrans::END: {
					this->mState->End ( this->mSelf );
					this->mState = 0;
					return;
				}
			}
		}
	}
	
	//----------------------------------------------------------------//
	USFsm () :
		mID ( 0 ),
		mState ( 0 ),
		mNextState ( 0 ),
		mSelf ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	virtual ~USFsm () {
	}
};

#endif
