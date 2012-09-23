// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIXmlWriter.h>

//================================================================//
// local
//================================================================//

//================================================================//
// MOAIXmlBuilder
//================================================================//

//----------------------------------------------------------------//
void MOAIXmlWriter::AddAttribute ( cc8* name, cc8* value ) {

	if ( mCurrent ) {

		mCurrent->SetAttribute ( name, value );
	}
}

//----------------------------------------------------------------//
void MOAIXmlWriter::AddAttribute ( cc8* name, int value ) {
	
	if ( mCurrent ) {

		mCurrent->SetAttribute ( name, value );
	}
}

//----------------------------------------------------------------//
void MOAIXmlWriter::AddAttribute ( cc8* name, double value ) {
	
	if ( mCurrent ) {

		mCurrent->SetDoubleAttribute ( name, value );
	}
}

//----------------------------------------------------------------//
void MOAIXmlWriter::AddElement ( cc8* name ) {

    TiXmlElement* newEle = new TiXmlElement ( name );
	if ( mCurrent ) {
		
		mNodes.push_back ( mCurrent );
		mCurrent->LinkEndChild ( newEle );

	} else {

		mDoc.LinkEndChild ( newEle );
	}
	mCurrent = newEle;
}

//----------------------------------------------------------------//
void MOAIXmlWriter::AddText ( cc8* text ) {

	if ( mCurrent ) {
		
		TiXmlText* textEle = new TiXmlText ( text );
		mCurrent->LinkEndChild ( textEle );
	}
}

//----------------------------------------------------------------//
void MOAIXmlWriter::CloseElement ( ) {

	if ( mNodes.size () > 0 ) {

		mCurrent = mNodes.back ();
		mNodes.pop_back ();
	} else {
	
		mCurrent = 0;
	}
}

//----------------------------------------------------------------//
MOAIXmlWriter::MOAIXmlWriter () :
	mCurrent ( 0 ) {

	TiXmlDeclaration* decl = new TiXmlDeclaration ( "1.0", "UTF-8", "");
	mDoc.LinkEndChild ( decl );
}

//----------------------------------------------------------------//
MOAIXmlWriter::~MOAIXmlWriter () {

	// once nodes are passed off to tiny xml, it cleans up memory in the following call.
	mDoc.Clear ();
}

//----------------------------------------------------------------//
void MOAIXmlWriter::SaveDocument ( cc8* filename ) {

	mDoc.SaveFile ( filename );
}