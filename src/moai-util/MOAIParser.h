// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARSER_H
#define	MOAIPARSER_H
#if MOAI_WITH_GPB

//================================================================//
// MOAIParser
//================================================================//
/**	@lua	MOAIParser
	@text	Parses strings using a LALR parser. Generates an
			abstract syntax tree that may then be traversed in Lua.
			
			To use, load a CGT file generated by GOLD Parser Builder.
			Thanks to Devin Cook:
			http://www.devincook.com/goldparser
*/
class MOAIParser :
	public virtual MOAILuaObject {
private:

	ZLCgt			mCGT;
	ZLSyntaxNode*	mAST;

	MOAILuaMemberRef		mOnStartNonterminal;
	MOAILuaMemberRef		mOnEndNonterminal;
	MOAILuaMemberRef		mOnTerminal;

	//----------------------------------------------------------------//
	static int		_loadFile				( lua_State* L );
	static int		_loadRules				( lua_State* L );
	static int		_loadString				( lua_State* L );
	static int		_setCallbacks			( lua_State* L );
	static int		_traverse				( lua_State* L );

	//----------------------------------------------------------------//
	void			OnEndNonterminal		( ZLSyntaxNode* node );
	void			OnStartNonterminal		( ZLSyntaxNode* node );
	void			OnTerminal				( ZLSyntaxNode* node );
	void			SetAST					( ZLSyntaxNode* ast );
	void			Traverse				( ZLSyntaxNode* node );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIParser )
	
	//----------------------------------------------------------------//
					MOAIParser				( ZLContext& context );
					~MOAIParser				();
};

#endif
#endif

