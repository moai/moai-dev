// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USSyntaxNode.h>

//================================================================//
// USSyntaxNode
//================================================================//

//----------------------------------------------------------------//
u32 USSyntaxNode::CountChildren () {

	return this->mChildren.Size ();
}

//----------------------------------------------------------------//
USSyntaxNode* USSyntaxNode::GetChild ( u32 i ) {

	if ( i < this->mChildren.Size ()) {
		return this->mChildren [ i ];
	}
	return 0;
}

//----------------------------------------------------------------//
double USSyntaxNode::GetNumber () {
	return atof ( this->mTerminal );
}

//----------------------------------------------------------------//
STLString USSyntaxNode::GetText () {
	return this->mTerminal;
}

//----------------------------------------------------------------//
bool USSyntaxNode::IsNonterminal () {

	return ( this->mChildren.Size () != 0 );
}

//----------------------------------------------------------------//
bool USSyntaxNode::IsTerminal () {

	return ( this->mChildren.Size () == 0 );
}

//----------------------------------------------------------------//
void USSyntaxNode::Print ( u32 indent, u32 offset ) {

	for ( u32 i = 0; i < indent; ++i )
		printf ( " " );
	
	if ( this->mChildren.Size ()) {
		printf ( "%s <%d>\n", this->mName.c_str (), this->mID );
	
		for ( u32 i = 0; i < this->mChildren.Size (); ++i )
			this->mChildren [ i ]->Print ( indent + offset, offset );
	}
	else {
		printf ( "%s:%s\n", this->mName.c_str (), this->mTerminal.c_str ());
	}
}

//----------------------------------------------------------------//
USSyntaxNode::USSyntaxNode () {
}

//----------------------------------------------------------------//
USSyntaxNode::~USSyntaxNode () {

	for ( u32 i = 0; i < this->mChildren.Size (); ++i ) {
		delete this->mChildren [ i ];
	}
}
