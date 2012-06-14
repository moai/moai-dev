// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USPARSER_H
#define	USPARSER_H

#include <uslscore/STLList.h>
#include <uslscore/USCgt.h>

class USLexStream;
class USSyntaxNode;

//================================================================//
// USDfaToken
//================================================================//
class USDfaToken {
public:

	USCgtSymbol		mSymbol;		// Copy of symbol this token matches
	string			mData;			// The token string
	u16				mLALRStateID;	// ID of the state
	USSyntaxNode*	mSyntaxNode;
	
	u32				mLine;
	
	//----------------------------------------------------------------//
	USDfaToken ();
};

//================================================================//
// USParser
//================================================================//
class USParser {
private:
	
	u16		mCurrentLALRStateID;
	u32		mLine;
	USCgt*	mCGT;
	
	typedef STLList < USDfaToken >::iterator TokenStackIt;
	STLList < USDfaToken > mTokenStack;
	
	bool mHandleErrors;
	u16 mErrorSymbolID;

	//----------------------------------------------------------------//
	USSyntaxNode*		Parse				( USLexStream* scanner, bool trimReductions );
	u32					ParseToken			( USDfaToken* token, bool trimReductions );
	void				RetrieveToken		( USDfaToken* token, USLexStream* scanner );

public:

	//----------------------------------------------------------------//
	void				Init				( USCgt& cgt, cc8* errorTerminal );
	USSyntaxNode*		Parse				( USStream& stream );
						USParser			();
						~USParser			();
};

#endif
