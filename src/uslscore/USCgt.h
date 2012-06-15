// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USCGT_H
#define	USCGT_H

#include <uslscore/STLString.h>
#include <uslscore/USLeanArray.h>

class USStream;

//================================================================//
// USCgtSymbol
//================================================================//
class USCgtSymbol {
private:
	
	friend class USCgt;
	friend class USParser;
	
	// Matches the native enumeration in the CGT
	enum {
		NONTERMINAL,
		TERMINAL,
		WHITESPACE,
		END,
		COMMENTSTART,
		COMMENTEND,
		COMMENTLINE,
		ERROR,
	};

	u16		mID;
	string	mName;
	u16		mKind;
	u32		mLineNo;
};

//================================================================//
// USCgtCharSet
//================================================================//
class USCgtCharSet {
private:
	
	friend class USCgt;
	friend class USParser;
	
	string	mCharacters;
};

//================================================================//
// USCgtRule
//================================================================//
class USCgtRule {
private:

	friend class USCgt;
	friend class USParser;

	u16					mRuleResult; // The nonterminal symbol this rule reduces to if matches
	USLeanArray < u16 >	mRuleSymbols; // The rule itself; may be by 0 to N symbols
};

//================================================================//
// USDfaStateEdge
//================================================================//
class USDfaStateEdge {
private:

	friend class USCgt;
	friend class USParser;

	u16		mCharSetID; // Character set to match for transition
	u16		mTargetStateID; // State to transition to if match found
};

//================================================================//
// USDfaState
//================================================================//
class USDfaState {
private:

	friend class USCgt;
	friend class USParser;

	u16		mID; // ID of this state
	bool	mAcceptState; // Does this state accept a terminal?
	u16		mAcceptSymbolID; // Index of symbol accepted by this state (if any)
	
	USLeanArray < USDfaStateEdge > mEdges;
};

//================================================================//
// USLalrAction
//================================================================//
class USLalrAction {
private:

	friend class USCgt;
	friend class USParser;

	// Matches the native enumeration in the CGT
	enum {
		ERROR = 0,
		SHIFT,
		REDUCE,
		GOTO,
		ACCEPT,
	};
	
	u16		mInputSymbolID; // Symbol to match
	u16		mActionType; // Type of action to take
	u16		mTarget; // Either the state ID on shift, or the rule ID on reduce
};

//================================================================//
// USLalrState
//================================================================//
class USLalrState {
private:

	friend class USCgt;
	friend class USParser;

	u16 mID; // ID of this state
	USLeanArray < USLalrAction > mActions;
};

//================================================================//
// USCgt
//================================================================//
class USCgt {
private:

	friend class USParser;

	//----------------------------------------------------------------//
	// Header & grammer info
	string	mHeader;
	string	mName;
	string	mVersion;
	string	mAuthor;
	string	mAbout;
	bool	mCaseSensitive;
	u16		mLALRStartSymbol;

	//----------------------------------------------------------------//
	// Initial States
	u16		mDFAInitialStateID;
	u16		mLALRInitialStateID;

	//----------------------------------------------------------------//
	// Tables
	USLeanArray < USCgtSymbol >		mSymbolTable;
	USLeanArray < USCgtCharSet >	mCharSetTable;
	USLeanArray < USCgtRule >		mRuleTable;
	USLeanArray < USDfaState >		mDFAStateTable;
	USLeanArray < USLalrState >		mLALRStateTable;

	//----------------------------------------------------------------//
	// Functions for reading USCgt entries
	USCgtSymbol*	FindNonterminal		( cc8* symbolName );
	USCgtSymbol*	FindSymbol			( cc8* symbolName );
	USCgtSymbol*	FindTerminal		( cc8* symbolName );
	bool			ReadBoolEntry		( USStream& binFile );
	u8				ReadByteEntry		( USStream& binFile );
	void			ReadEmptyEntry		( USStream& binFile );
	u16				ReadIntEntry		( USStream& binFile );
	STLString		ReadStringEntry		( USStream& binFile ); // Converts to ASCII
	STLString		ReadUnicodeAsASCII	( USStream& binFile ); // Converts to ASCII

public:

	//----------------------------------------------------------------//
	// Initialization functions
	void				Load				( cc8* filename );
};

#endif
