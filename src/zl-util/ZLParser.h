// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLPARSER_H
#define	ZLPARSER_H

#include <zl-util/STLList.h>
#include <zl-util/ZLCgt.h>

class ZLLexStream;
class ZLSyntaxNode;

//================================================================//
// ZLDfaToken
//================================================================//
class ZLDfaToken {
public:

	ZLCgtSymbol		mSymbol;		// Copy of symbol this token matches
	string			mData;			// The token string
	u16				mLALRStateID;	// ID of the state
	ZLSyntaxNode*	mSyntaxNode;
	
	u32				mLine;
	
	//----------------------------------------------------------------//
	ZLDfaToken ();
};

//================================================================//
// ZLParser
//================================================================//
class ZLParser {
private:
	
	u16		mCurrentLALRStateID;
	u32		mLine;
	ZLCgt*	mCGT;
	
	typedef STLList < ZLDfaToken >::iterator TokenStackIt;
	STLList < ZLDfaToken > mTokenStack;
	
	bool mHandleErrors;
	u16 mErrorSymbolID;

	//----------------------------------------------------------------//
	ZLSyntaxNode*		Parse				( ZLLexStream* scanner, bool trimReductions );
	u32					ParseToken			( ZLDfaToken* token, bool trimReductions );
	void				RetrieveToken		( ZLDfaToken* token, ZLLexStream* scanner );

public:

	//----------------------------------------------------------------//
	void				Init				( ZLCgt& cgt, cc8* errorTerminal );
	ZLSyntaxNode*		Parse				( ZLStream& stream );
						ZLParser			();
						~ZLParser			();
};

#endif
