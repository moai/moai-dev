// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIParser.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	buildTree
	@text	Parses the contents of a file and builds an abstract
			syntax tree.

	@in		MOAIParser self
	@in		string filename
	@out	table ast
*/
int MOAIParser::_buildTree ( lua_State* L ) {
	LUA_SETUP ( MOAIParser, "US" )
	
	cc8* filename = state.GetValue < cc8* >( 2, "" );
	
	USParser parser;
	parser.Init ( self->mCGT, 0 );
	USSyntaxNode* ast = parser.Parse ( filename, true );
	
	self->SetAST ( ast );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadRules
	@text	Parses the contents of the specified CGT.

	@in		MOAIParser self
	@in		string filename			The path of the file to read the CGT data from.
	@out	nil
*/
int MOAIParser::_loadRules ( lua_State* L ) {
	LUA_SETUP ( MOAIParser, "US" )

	cc8* filename = state.GetValue < cc8* >( 2, "" );
	self->mCGT.Load ( filename );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setCallbacks
	@text	Set Lua syntax tree node handlers for tree traversal.

	@in		MOAIParser self
	@opt	function onStartNonterminal		Default value is nil.
	@opt	function onEndNonterminal		Default value is nil.
	@opt	function onTerminal				Default value is nil.
	@out	nil
*/
int MOAIParser::_setCallbacks ( lua_State* L ) {
	LUA_SETUP ( MOAIParser, "U" )
	
	self->mOnStartNonterminal.SetRef ( state, 2, false );
	self->mOnEndNonterminal.SetRef ( state, 3, false );
	self->mOnTerminal.SetRef ( state, 4, false );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	traverse
	@text	Top down traversal of the abstract syntax tree.

	@in		MOAIParser self
	@out	nil
*/
int MOAIParser::_traverse ( lua_State* L ) {
	LUA_SETUP ( MOAIParser, "U" )
	
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
		RTTI_EXTEND ( USLuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIParser::~MOAIParser () {

	this->SetAST ( 0 );
}

//----------------------------------------------------------------//
void MOAIParser::OnEndNonterminal ( USSyntaxNode* node ) {

	if ( !this->mOnEndNonterminal ) return;
	USLuaStateHandle state = this->mOnEndNonterminal.GetSelf ();
	
	state.Push ( node->GetID () );
	state.DebugCall ( 1, 0 );
}

//----------------------------------------------------------------//
void MOAIParser::OnStartNonterminal ( USSyntaxNode* node ) {

	if ( !this->mOnStartNonterminal ) return;
	USLuaStateHandle state = this->mOnStartNonterminal.GetSelf ();
	
	state.Push ( node->GetID ());
	state.Push ( node->GetLine ());
	state.Push ( node->GetName ());
	
	state.DebugCall ( 3, 0 );
}

//----------------------------------------------------------------//
void MOAIParser::OnTerminal ( USSyntaxNode* node ) {

	if ( !this->mOnTerminal ) return;
	USLuaStateHandle state = this->mOnTerminal.GetSelf ();
	
	state.Push ( node->GetID ());
	state.Push ( node->GetLine ());
	state.Push ( node->GetText ());
	
	state.DebugCall ( 3, 0 );
}

//----------------------------------------------------------------//
void MOAIParser::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIParser::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable[] = {
		{ "buildTree",			_buildTree },
		{ "loadRules",			_loadRules },
		{ "setCallbacks",		_setCallbacks },
		{ "traverse",			_traverse },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParser::SetAST ( USSyntaxNode* ast ) {

	if ( this->mAST ) {
		delete this->mAST;
	}
	this->mAST = ast;
}

//----------------------------------------------------------------//
void MOAIParser::Traverse ( USSyntaxNode* node ) {

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
