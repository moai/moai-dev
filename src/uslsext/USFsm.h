// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	FSM_H
#define	FSM_H

//================================================================//
// USFsmTrans
//================================================================//
/**	@brief Namespace for transition modes enum.
*/
namespace USFsmTrans {

	/* @brief State transition modes. */
	enum Type {
		CONTINUE,	/**< Continue the update processing with the next pending state. */
		YIELD,		/**< Stop update processing until the next update step. */
		END			/**< Stop update processing and clear the USFsm. */
	};
}

//================================================================//
// USFsmStateBase
//================================================================//
/**	@brief Abstract base class for all USFsm states.
*/
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
	/**	@brief Constructor.
	*/
	USFsmStateBase () {
		this->SimpleRTTI < RTTIBase >( this );
	}
	
	//----------------------------------------------------------------//
	/**	@brief Destructor.
	*/
	virtual ~USFsmStateBase () {
	}
};

//================================================================//
// USFsmState
//================================================================//
/**	@brief Base class for states to be managed by an USFsm.  This is a helper template class that binds
	a specific state type to the more abstract USFsmStateBase which relies on void pointers instead of
	statically typed parameters.
	
	@tparam TYPE The actor type on which this state will act (see USFsm).
*/
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
	/**	@brief Overload this method to handle state initialization.
		@param self The actor class.
	*/
	virtual void OnBegin ( TYPE& self ) {
	}
	
	//----------------------------------------------------------------//
	/**	@brief Overload this method to handle state finalization.
		@param self The actor class.
	*/
	virtual void OnEnd ( TYPE& self ) {
	}
	
	//----------------------------------------------------------------//
	/**	@brief Overload this method to handle state update processing.
		@param self The actor class.
		@param step The current update step.
		@return Transition mode for the state.  One of USFsmTrans::CONTINUE, USFsmTrans::YIELD or USFsmTrans::END.
	*/
	virtual USFsmTrans::Type OnUpdate ( TYPE& self, float step ) {
		return USFsmTrans::END;
	}
	
	//----------------------------------------------------------------//
	/**	@brief Constructor.
	*/
	USFsmState () :
		mSelf ( 0 ) {
		this->SimpleRTTI < USFsmStateBase >( this );
	}
};

//================================================================//
// USFsm
//================================================================//
/** @brief A finite state machine class. This simple, powerful template class formalizes the notion of
	multi-step behaviors and state transitions: it is intended
	to help simulating complex behaviors of objects over a number of
	time steps.
	
	@details USFsm manages the application of behavior states to an 'actor' class.  Because USFsm is a
	template class, any class may be used as a target actor.  In addition, the actor on which the USFsm
	operates may be changed at any time by the user.  When using an USFsm, it is important to remember
	that the USFsm and its states are external to the actor, which must be passed in to the USFsm via
	SetState().
	
	@tparam TYPE The type of the actor class on which the USFsm will operate.
	@tparam STATE_BASE_TYPE The type of class returned by GetState().  Must derive from USFsmState<>.
*/
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
	/**	@brief Ends the current state (if any) and clears the USFsm.
	*/
	void Clear () {
		
		if ( this->mState ) {
			this->mState->End ( this->mSelf );
		}
		
		this->mState = 0;
		this->mNextState = 0;
		this->mID = 0;
	}
	
	//----------------------------------------------------------------//
	/**	@brief Returns the current state (if any).
		@return A pointer to the current state or nil (if USFsm is inactive).
	*/
	USFsmStateBase* GetState () {
		return this->mState;
	}
	
	//----------------------------------------------------------------//
	/**	@brief Returns the current state (if any) dynamically cast to the desired type.
		@tparam STATE_TYPE The type to dynamically cast the state to.
		@return A pointer to the current state or nil.
	*/
	template < typename STATE_TYPE >
	STATE_TYPE* GetState () {
		if ( this->mState ) {
			return this->mState->template AsType < STATE_TYPE >();
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	/**	@brief Scedules a transition to a new state.
		@details The transition will occur the next time the state is updated.
		@tparam[in] STATE_TYPE	The class type of the new state.
		@param[in] self The target object.
		@return A reference to the new state object.
	*/
	template < typename STATE_TYPE >
	STATE_TYPE& SetState ( RTTIBase& self ) {
		
		this->mSelf = &self;
		USUnion < USFsmStateBase >& buffer = mBuffers [ this->mID ];
		USFsmStateBase& state = buffer.Set < STATE_TYPE >();
		this->mNextState = &state;
		return ( STATE_TYPE& )state;
	}
	
	//----------------------------------------------------------------//
	/**	@brief Updates the current state by a single step.
		@param[in]	step	The step by which to advance the current state.
	*/
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
	/**	@brief Constructor.
	*/
	USFsm () :
		mID ( 0 ),
		mState ( 0 ),
		mNextState ( 0 ),
		mSelf ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	/**	@brief Destructor.
	*/
	virtual ~USFsm () {
	}
};

#endif
