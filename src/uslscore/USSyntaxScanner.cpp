// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USSyntaxNode.h>
#include <uslscore/USSyntaxScanner.h>

//================================================================//
// USSyntaxScanner
//================================================================//

//----------------------------------------------------------------//
void USSyntaxScanner::ScanSyntax ( USSyntaxNode* node ) {

	if ( !node ) return;

	if ( node->IsNonterminal ()) {
	
		u32 nChildren = node->CountChildren ();
		for ( u32 i = 0; i < nChildren; ++i ) {
			this->ScanSyntax ( node->GetChild ( i ));
		}
		this->OnRule ( node->GetID (), *node );
	}
	else {
	
		this->OnTerminal ( node->GetID (), *node );
	}
}

//----------------------------------------------------------------//
USSyntaxScanner::USSyntaxScanner () {
}

//----------------------------------------------------------------//
USSyntaxScanner::~USSyntaxScanner () {
}
