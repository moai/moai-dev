// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIParser.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_GPB

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	loadFile
	@text	Parses the contents of a file and builds an abstract
			syntax tree.

	@in		MOAIParser self
	@in		string filename
	@out	table ast
*/
int MOAIParser::_loadFile ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParser, "US" )
	
	cc8* filename = state.GetValue < cc8* >( 2, "" );
	if ( MOAILogMessages::CheckFileExists ( filename, L )) {
	
		ZLFileStream stream;
		if ( stream.OpenRead ( filename )) {
			
			ZLParser parser;
			parser.Init ( self->mCGT, 0 );
			ZLSyntaxNode* ast = parser.Parse ( stream );
			
			self->SetAST ( ast );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	loadRules
	@text	Parses the contents of the specified CGT.

	@in		MOAIParser self
	@in		string filename			The path of the file to read the CGT data from.
	@out	nil
*/
int MOAIParser::_loadRules ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParser, "US" )

	cc8* filename = state.GetValue < cc8* >( 2, "" );

	if ( MOAILogMessages::CheckFileExists ( filename, L )) {
		self->mCGT.Load ( filename );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	loadString
	@text	Parses the contents of a string and builds an abstract
			syntax tree.

	@in		MOAIParser self
	@in		string filename
	@out	table ast
*/
int MOAIParser::_loadString ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParser, "US" )
	
	size_t len;
	cc8* str = lua_tolstring ( state, 2, &len );
	
	ZLByteStream stream;
	stream.SetBuffer (( void* )str, len );
	stream.SetLength ( len );
		
	ZLParser parser;
	parser.Init ( self->mCGT, 0 );
	ZLSyntaxNode* ast = parser.Parse ( stream );
	
	self->SetAST ( ast );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCallbacks
	@text	Set Lua syntax tree node handlers for tree traversal.

	@in		MOAIParser self
	@opt	function onStartNonterminal		Default value is nil.
	@opt	function onEndNonterminal		Default value is nil.
	@opt	function onTerminal				Default value is nil.
	@out	nil
*/
int MOAIParser::_setCallbacks ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParser, "U" )
	
	self->mOnStartNonterminal.SetRef ( *self, state, 2 );
	self->mOnEndNonterminal.SetRef ( *self, state, 3 );
	self->mOnTerminal.SetRef ( *self, state, 4 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	traverse
	@text	Top-down traversal of the abstract syntax tree.

	@in		MOAIParser self
	@out	nil
*/
int MOAIParser::_traverse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParser, "U" )
	
	self->Traverse ( self->mAST );
	
	return 0;
}

//================================================================//
// MOAIParser
//================================================================//

//----------------------------------------------------------------//
MOAIParser::MOAIParser () :
	mAST ( 0 ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIParser::~MOAIParser () {

	this->SetAST ( 0 );
}

//----------------------------------------------------------------//
void MOAIParser::OnEndNonterminal ( ZLSyntaxNode* node ) {

	if ( this->mOnEndNonterminal ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mOnEndNonterminal.PushRef ( state )) {
			state.Push ( node->GetID ());
			state.DebugCall ( 1, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIParser::OnStartNonterminal ( ZLSyntaxNode* node ) {

	if ( this->mOnStartNonterminal ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mOnStartNonterminal.PushRef ( state )) {
			state.Push ( node->GetID ());
			state.Push ( node->GetLine ());
			state.Push ( node->GetName ());
			state.DebugCall ( 3, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIParser::OnTerminal ( ZLSyntaxNode* node ) {

	if ( this->mOnTerminal ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mOnTerminal.PushRef ( state )) {
			state.Push ( node->GetID ());
			state.Push ( node->GetLine ());
			state.Push ( node->GetText ());
			state.DebugCall ( 3, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIParser::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIParser::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "loadFile",			_loadFile },
		{ "loadRules",			_loadRules },
		{ "loadString",			_loadString },
		{ "setCallbacks",		_setCallbacks },
		{ "traverse",			_traverse },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParser::SetAST ( ZLSyntaxNode* ast ) {

	if ( this->mAST ) {
		delete this->mAST;
	}
	this->mAST = ast;
}

//----------------------------------------------------------------//
void MOAIParser::Traverse ( ZLSyntaxNode* node ) {

	if ( !node ) return;
	
	if ( node->IsNonterminal ()) {
		
		this->OnStartNonterminal ( node );
		
		u32 nChildren = node->CountChildren ();
		for ( u32 i = 0; i < nChildren; ++i ) {
			this->Traverse ( node->GetChild ( i ));
		}
		
		this->OnEndNonterminal ( node );
	}
	else {
		this->OnTerminal ( node );
	}
}

#endif