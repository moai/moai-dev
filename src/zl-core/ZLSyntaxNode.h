// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLSYNTAXNODE_H
#define	ZLSYNTAXNODE_H

#include <zl-core/ZLAccessors.h>
#include <zl-core/STLString.h>
#include <zl-core/ZLLeanArray.h>

//================================================================//
// ZLSyntaxNode
//================================================================//
class ZLSyntaxNode {
private:
	
	friend class ZLParser;
	
	u16	mID;
	u32	mLine;
	
	STLString	mName;
	STLString	mTerminal;

	ZLLeanArray < ZLSyntaxNode* > mChildren; // If no children, it's a terminal

public:

	GET ( u32, ID, mID )
	GET ( u32, Line, mLine )
	GET ( STLString, Name, mName )
	GET ( STLString, Terminal, mTerminal )

	//----------------------------------------------------------------//
	ZLSize				CountChildren			();
	ZLSyntaxNode*		GetChild				( ZLIndex i );
	double				GetNumber				();
	STLString			GetText					();
	bool				IsNonterminal			();
	bool				IsTerminal				();
	void				Print					( u32 indent, u32 offset );
						ZLSyntaxNode			();
						~ZLSyntaxNode			();
};

#endif
