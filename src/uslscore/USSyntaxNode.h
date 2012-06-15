// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USSYNTAXNODE_H
#define	USSYNTAXNODE_H

#include <uslscore/USAccessors.h>
#include <uslscore/STLString.h>
#include <uslscore/USLeanArray.h>

//================================================================//
// USSyntaxNode
//================================================================//
class USSyntaxNode {
private:
	
	friend class USParser;
	
	u16	mID;
	u32	mLine;
	
	STLString	mName;
	STLString	mTerminal;

	USLeanArray < USSyntaxNode* > mChildren; // If no children, it's a terminal

public:

	GET ( u32, ID, mID )
	GET ( u32, Line, mLine )
	GET ( STLString, Name, mName )
	GET ( STLString, Terminal, mTerminal )

	//----------------------------------------------------------------//
	u32					CountChildren			();
	USSyntaxNode*		GetChild				( u32 i );
	double				GetNumber				();
	STLString			GetText					();
	bool				IsNonterminal			();
	bool				IsTerminal				();
	void				Print					( u32 indent, u32 offset );
						USSyntaxNode			();
						~USSyntaxNode			();
};

#endif
