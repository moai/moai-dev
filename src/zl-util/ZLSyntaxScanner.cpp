// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLSyntaxNode.h>
#include <zl-util/ZLSyntaxScanner.h>

//================================================================//
// ZLSyntaxScanner
//================================================================//

//----------------------------------------------------------------//
void ZLSyntaxScanner::ScanSyntax ( ZLSyntaxNode* node ) {

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
ZLSyntaxScanner::ZLSyntaxScanner () {
}

//----------------------------------------------------------------//
ZLSyntaxScanner::~ZLSyntaxScanner () {
}
