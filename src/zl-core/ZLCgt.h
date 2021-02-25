// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLCGT_H
#define	ZLCGT_H

#include <zl-core/STLString.h>
#include <zl-core/ZLLeanArray.h>

class ZLStream;

//================================================================//
// ZLCgtSymbol
//================================================================//
class ZLCgtSymbol {
private:
	
	friend class ZLCgt;
	friend class ZLParser;
	
	// Matches the native enumeration in the CGT
	enum {
		CGT_NONTERMINAL,
		CGT_TERMINAL,
		CGT_WHITESPACE,
		CGT_END,
		CGT_COMMENTSTART,
		CGT_COMMENTEND,
		CGT_COMMENTLINE,
		CGT_ERROR,
	};

	u16				mID;
	STLString		mName;
	u16				mKind;
	u32				mLineNo;
};

//================================================================//
// ZLCgtCharSet
//================================================================//
class ZLCgtCharSet {
private:
	
	friend class ZLCgt;
	friend class ZLParser;
	
	STLString		mCharacters;
};

//================================================================//
// ZLCgtRule
//================================================================//
class ZLCgtRule {
private:

	friend class ZLCgt;
	friend class ZLParser;

	u16					mRuleResult; // The nonterminal symbol this rule reduces to if matches
	ZLLeanArray < u16 >	mRuleSymbols; // The rule itself; may be by 0 to N symbols
};

//================================================================//
// ZLDfaStateEdge
//================================================================//
class ZLDfaStateEdge {
private:

	friend class ZLCgt;
	friend class ZLParser;

	u16		mCharSetID; // Character set to match for transition
	u16		mTargetStateID; // State to transition to if match found
};

//================================================================//
// ZLDfaState
//================================================================//
class ZLDfaState {
private:

	friend class ZLCgt;
	friend class ZLParser;

	u16		mID; // ID of this state
	bool	mAcceptState; // Does this state accept a terminal?
	u16		mAcceptSymbolID; // Index of symbol accepted by this state (if any)
	
	ZLLeanArray < ZLDfaStateEdge > mEdges;
};

//================================================================//
// ZLLalrAction
//================================================================//
class ZLLalrAction {
private:

	friend class ZLCgt;
	friend class ZLParser;

	// Matches the native enumeration in the CGT
	enum {
		LALR_ERROR = 0,
		LALR_SHIFT,
		LALR_REDUCE,
		LALR_GOTO,
		LALR_ACCEPT,
	};
	
	u16		mInputSymbolID; // Symbol to match
	u16		mActionType; // Type of action to take
	u16		mTarget; // Either the state ID on shift, or the rule ID on reduce
};

//================================================================//
// ZLLalrState
//================================================================//
class ZLLalrState {
private:

	friend class ZLCgt;
	friend class ZLParser;

	u16 mID; // ID of this state
	ZLLeanArray < ZLLalrAction > mActions;
};

//================================================================//
// ZLCgt
//================================================================//
class ZLCgt {
private:

	friend class ZLParser;

	//----------------------------------------------------------------//
	// header & grammer info
	STLString		mHeader;
	STLString		mName;
	STLString		mVersion;
	STLString		mAuthor;
	STLString		mAbout;
	bool			mCaseSensitive;
	u16				mLALRStartSymbol;

	//----------------------------------------------------------------//
	// Initial States
	u16		mDFAInitialStateID;
	u16		mLALRInitialStateID;

	//----------------------------------------------------------------//
	// Tables
	ZLLeanArray < ZLCgtSymbol >		mSymbolTable;
	ZLLeanArray < ZLCgtCharSet >	mCharSetTable;
	ZLLeanArray < ZLCgtRule >		mRuleTable;
	ZLLeanArray < ZLDfaState >		mDFAStateTable;
	ZLLeanArray < ZLLalrState >		mLALRStateTable;

	//----------------------------------------------------------------//
	// Functions for reading ZLCgt entries
	ZLCgtSymbol*	FindNonterminal		( cc8* symbolName );
	ZLCgtSymbol*	FindSymbol			( cc8* symbolName );
	ZLCgtSymbol*	FindTerminal		( cc8* symbolName );
	bool			ReadBoolEntry		( ZLStream& binFile );
	u8				ReadByteEntry		( ZLStream& binFile );
	void			ReadEmptyEntry		( ZLStream& binFile );
	u16				ReadIntEntry		( ZLStream& binFile );
	STLString		ReadStringEntry		( ZLStream& binFile ); // Converts to ASCII
	STLString		ReadUnicodeAsASCII	( ZLStream& binFile ); // Converts to ASCII

public:

	//----------------------------------------------------------------//
	// Initialization functions
	void				Load				( cc8* filename );
};

#endif
