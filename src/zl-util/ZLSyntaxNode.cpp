// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLSyntaxNode.h>

//================================================================//
// ZLSyntaxNode
//================================================================//

//----------------------------------------------------------------//
size_t ZLSyntaxNode::CountChildren () {

	return this->mChildren.Size ();
}

//----------------------------------------------------------------//
ZLSyntaxNode* ZLSyntaxNode::GetChild ( size_t i ) {

	if ( i < this->mChildren.Size ()) {
		return this->mChildren [ i ];
	}
	return 0;
}

//----------------------------------------------------------------//
double ZLSyntaxNode::GetNumber () {
	return atof ( this->mTerminal );
}

//----------------------------------------------------------------//
STLString ZLSyntaxNode::GetText () {
	return this->mTerminal;
}

//----------------------------------------------------------------//
bool ZLSyntaxNode::IsNonterminal () {

	return ( this->mChildren.Size () != 0 );
}

//----------------------------------------------------------------//
bool ZLSyntaxNode::IsTerminal () {

	return ( this->mChildren.Size () == 0 );
}

//----------------------------------------------------------------//
void ZLSyntaxNode::Print ( u32 indent, u32 offset ) {

	for ( u32 i = 0; i < indent; ++i )
		printf ( " " );
	
	if ( this->mChildren.Size ()) {
		printf ( "%s <%d>\n", this->mName.c_str (), this->mID );
	
		for ( size_t i = 0; i < this->mChildren.Size (); ++i )
			this->mChildren [ i ]->Print ( indent + offset, offset );
	}
	else {
		printf ( "%s:%s\n", this->mName.c_str (), this->mTerminal.c_str ());
	}
}

//----------------------------------------------------------------//
ZLSyntaxNode::ZLSyntaxNode () {
}

//----------------------------------------------------------------//
ZLSyntaxNode::~ZLSyntaxNode () {

	for ( size_t i = 0; i < this->mChildren.Size (); ++i ) {
		delete this->mChildren [ i ];
	}
}
