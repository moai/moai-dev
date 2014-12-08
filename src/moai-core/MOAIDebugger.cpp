// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-core/MOAIDebugger.h>

//================================================================//
// MOAIPathDictionary
//================================================================//

//----------------------------------------------------------------//
int MOAIPathDictionary::FindIdentifierAbsolute ( unsigned int hash, const char path []) {

	size_t bucketIndex = hash & ( mBuckets.size () - 1 );
	for ( int cursor = mBuckets [ bucketIndex ]; cursor >= 0; cursor = this->mEntries [ cursor ].next ) {
		const MOAIPathDictionary::Entry& entry = this->mEntries [ cursor ];
		if ( entry.hash == hash && strcmp ( path, &this->mStringPool [ entry.idAbs ]) == 0 ) {
			return entry.idAbs;
		}
	}
	return -1;
}

//----------------------------------------------------------------//
int MOAIPathDictionary::FindIdentifierRelative(unsigned int hash, const char path[]) {

	size_t bucketIndex = hash & ( this->mBuckets.size () - 1 );
	for (int cursor = this->mBuckets [ bucketIndex ]; cursor >= 0; cursor = this->mEntries [ cursor ].next ) {
		const MOAIPathDictionary::Entry& entry = this->mEntries [ cursor ];
		if ( entry.hash == hash && strcmp ( path, &this->mStringPool [ entry.idRel ]) == 0 ) {
			return entry.idAbs;
		}
	}
	return -1;
}

//----------------------------------------------------------------//
uint MOAIPathDictionary::GetHash ( const char path[], size_t* pathLength ) {

	// Using djb2 (see http://www.cse.yorku.ca/~oz/hash.html)
	
	size_t i = 0;
	int c;
	unsigned long hash = 5381;
	
	for (; ( c = path [ i ]); ++i ) {
		hash = (( hash << 5 ) + hash) + c; // hash * 33 + c
	}
	*pathLength = i;
	return ( uint )hash;
}

//----------------------------------------------------------------//
int MOAIPathDictionary::GetIdentifier ( const char path []) {

	size_t pathLength;
	unsigned int pathHash = GetHash ( path, &pathLength );
	int id = this->FindIdentifierRelative ( pathHash, path );
	if ( id < 0 ) {
	
		std::string absPathStr = ZLFileSys::GetAbsoluteFilePath ( path );
		size_t absPathLength;
		unsigned int absPathHash = this->GetHash ( absPathStr.c_str (), &absPathLength );
		id = this->FindIdentifierAbsolute ( absPathHash, absPathStr.c_str ());
		if ( id < 0 ) {
			id = this->MakeIdentifier ( absPathStr.c_str (), absPathLength );
			this->SetIdentifier ( absPathHash, -1, id );
		}
		int idRel = this->MakeIdentifier ( path, pathLength );
		this->SetIdentifier ( pathHash, idRel, id );
	}
	return id;
}

//----------------------------------------------------------------//
void MOAIPathDictionary::Grow() {

	size_t newSize = this->mEntries.size () << 1;
	mEntries.resize ( newSize );

	std::vector < int > newBuckets;
	newBuckets.resize ( newSize, -1 );
	for (int i = 0; i < this->mCount; ++i ) {
		Entry& entry = this->mEntries [ i ];
		size_t bucketIndex = entry.hash & ( newSize - 1 );
		entry.next = newBuckets [ bucketIndex ];
		newBuckets [ bucketIndex ] = i;
	}
	this->mBuckets.swap ( newBuckets );
}

//----------------------------------------------------------------//
int MOAIPathDictionary::MakeIdentifier ( const char path [], size_t length ) {

	int id = ( int )this->mStringPool.size ();
	this->mStringPool.insert ( this->mStringPool.end (), path, path + length + 1 );
	return id;
}

//----------------------------------------------------------------//
MOAIPathDictionary::MOAIPathDictionary () :
	mCount ( 0 ) {
	
	const size_t INITIAL_SIZE = 1 << 6; // Must be a power of 2
	mBuckets.resize ( INITIAL_SIZE, -1 );
	mEntries.resize ( INITIAL_SIZE );
}

//----------------------------------------------------------------//
MOAIPathDictionary::~MOAIPathDictionary () {
}

//----------------------------------------------------------------//
void MOAIPathDictionary::SetIdentifier ( unsigned int hash, int idRel, int idAbs ) {

	size_t bucketIndex = hash & ( mBuckets.size () - 1 );

	if ( this->mCount == ( int )this->mEntries.size ()) {
		this->Grow ();
		bucketIndex = hash & ( this->mBuckets.size () - 1 );
	}

	Entry& entry = this->mEntries [ this->mCount ];
	entry.hash = hash;
	entry.next = this->mBuckets [ bucketIndex ];
	entry.idRel = idRel;
	entry.idAbs = idAbs;
	this->mBuckets [ bucketIndex ] = this->mCount;
	++this->mCount;
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
void MOAIDebugger::_callback ( lua_State *L, lua_Debug *ar ) {

	MOAIDebugger::Get ().Callback ( L, ar );
}

//----------------------------------------------------------------//
//int MOAIDebugger::_sendMessage ( lua_State* L ) {
//
//	// Read the message off of the top of the stack
//	json_t* message = MOAIDebugger::ConvertStackIndexToJSON ( L, lua_gettop ( L ));
//
//	// Send the message back to the IDE.
//	json_t* msg = json_object ();
//	json_object_set_new ( msg, "ID", json_string ( "message" ));
//	json_object_set_new ( msg, "Value", message );
//	char* data = json_dumps ( msg, 0 );
//	MOAIDebugger::SendData ( std::string ( data ));
//	free ( data );
//
//	// Done!
//	return 0;
//}

//================================================================//
// MOAIDebugger
//================================================================//

//----------------------------------------------------------------//
void MOAIDebugger::Callback ( lua_State *L, lua_Debug *ar ) {

	// Fill debugging information.
	lua_getinfo ( L, "nSl", ar );
	
	const char *source = ar->source;
	const unsigned int currentline = ar->currentline;
	
	bool needBreak = false;

	// Handle step operations
	if ( MOAIDebugger::mStepMode != MOAIDebugger::RUN && MOAIDebugger::mStepState == L ) {
	
		// Update our stack depth
		switch ( ar->event ) {
		
			case LUA_HOOKCALL:
				++this->mStepDepth;
				return;
			
			case LUA_HOOKRET:
			case LUA_HOOKTAILRET:
				--this->mStepDepth;
				return;
		}

		// Determine whether this qualifies as a step based on our step mode
		switch ( this->mStepMode ) {
		
			case MOAIDebugger::STEP_OVER:
				if ( this->mStepDepth <= 0 ) needBreak = true;
				break;
			
			case MOAIDebugger::STEP_OUT:
				if ( this->mStepDepth < 0 ) needBreak = true;
				break;
			
			case MOAIDebugger::STEP_INTO:
				needBreak = true;
				break;
		}
	}

	// Compare against any currently-set breakpoints
	if ( !needBreak && ar->event == LUA_HOOKLINE && source [ 0 ] == '@' && !this->mBreakpoints.empty ()) {
	
		int identifier = this->mPathDictionary.GetIdentifier ( source + 1 );
	
		for ( std::vector < MOAIBreakpoint >::const_iterator i = this->mBreakpoints.begin (); i < this->mBreakpoints.end (); i++ ) {
			if (( *i ).identifier == identifier && ( *i ).line == currentline ) {
				needBreak = true;
				break;
			}
		}
	}

	// Trigger a break if necessary
	if ( needBreak ) {
	
//		const char *name = ar->name;
//		const char *what = ar->what;
//		const char *short_src = ar->short_src;
	
//		if ( std::string ( what ) == "main" ) {
//			this->SendBreak ( L, "<main>", ( unsigned int )currentline, short_src );
//		}
//		else if (std::string ( what ) == "C" ) {
//			this->SendBreak ( L, "<native>", ( unsigned int )currentline, short_src );
//		}
//		else if (std::string ( what ) == "tail" ) {
//			this->SendBreak ( L, "<tail call>", ( unsigned int )currentline, short_src );
//		}
//		else if (std::string ( what ) == "Lua" && name == NULL) {
//			this->SendBreak ( L, "<anonymous>", ( unsigned int )currentline, short_src );
//		}
//		else if (std::string ( what ) == "Lua" ) {
//			this->SendBreak ( L, name, ( unsigned int )currentline, short_src );
//		}
		
		MOAIDebugger::Pause ( L );
	}
}

//----------------------------------------------------------------//
MOAIDebugger::MOAIDebugger () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIDebugger::~MOAIDebugger () {
}

//----------------------------------------------------------------//
void MOAIDebugger::RegisterLuaClass ( MOAILuaState& state ) {
	luaL_Reg regTable [] = {
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDebugger::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIDebugger::HookLua ( lua_State* L ) {

	MOAIDebugger::mEnginePaused = false;

	// Initalize the Lua hooks.
	lua_sethook ( L, MOAIDebugger::_callback, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE, 0);

	// At this point the debug hooks are initalized, but we don't want to proceed until the IDE tells
	// us to continue (in case it wants to set up initial breakpoints).
	//this->SendWait();
	//this->Pause(L);

	// After we have done the wait-and-pause cycle, we are ready to give control back to the engine.
	return;
}

//----------------------------------------------------------------//
void MOAIDebugger::Pause ( lua_State * L ) {
	UNUSED ( L );
//	printf("debug harness: Waiting to receive messages from debugging interface.\n");
//	MOAIDebugger::mEnginePaused = true;
//	MOAIDebugger::mStepMode = MOAIDebugger::RUN;
//	MOAIDebugger::mStepState = NULL;
//	MOAIDebugger::mStepDepth = 0;
//	while (MOAIDebugger::mEnginePaused)
//	{
//		// Receive and handle IDE messages until we get a continue.
//		MOAIDebugger::ReceiveMessage(L);
//	}
//	printf("debug harness: Continuing execution of program.\n");
//	MOAIDebugger::SendResume();
}
