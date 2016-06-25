// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIXmlWriter.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_TINYXML

#include <tinyxml.h>

//================================================================//
// local
//================================================================//

//================================================================//
// MOAIXmlBuilder
//================================================================//

//----------------------------------------------------------------//
void MOAIXmlWriter::AddAttribute ( cc8* name, cc8* value ) {

	if ( this->mCurrent ) {

		(( TiXmlElement* )this->mCurrent )->SetAttribute ( name, value );
	}
}

//----------------------------------------------------------------//
void MOAIXmlWriter::AddAttribute ( cc8* name, int value ) {
	
	if ( this->mCurrent ) {

		(( TiXmlElement* )this->mCurrent )->SetAttribute ( name, value );
	}
}

//----------------------------------------------------------------//
void MOAIXmlWriter::AddAttribute ( cc8* name, double value ) {
	
	if ( this->mCurrent ) {

		(( TiXmlElement* )this->mCurrent )->SetDoubleAttribute ( name, value );
	}
}

//----------------------------------------------------------------//
void MOAIXmlWriter::AddElement ( cc8* name ) {

    TiXmlElement* newEle = new TiXmlElement ( name );
	if ( this->mCurrent ) {
		
		mNodes.push_back ( this->mCurrent );
		(( TiXmlElement* )this->mCurrent )->LinkEndChild ( newEle );

	} else {

		(( TiXmlDocument* )this->mDoc )->LinkEndChild ( newEle );
	}
	this->mCurrent = ( MOAITiXmlElement )newEle;
}

//----------------------------------------------------------------//
void MOAIXmlWriter::AddText ( cc8* text ) {

	if ( mCurrent ) {
		
		TiXmlText* textEle = new TiXmlText ( text );
		(( TiXmlElement* )this->mCurrent )->LinkEndChild ( textEle );
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

	TiXmlDocument* doc = new TiXmlDocument ();
	doc->LinkEndChild ( new TiXmlDeclaration ( "1.0", "UTF-8", "" ));
	this->mDoc = ( MOAITiXmlDocument )doc;
}

//----------------------------------------------------------------//
MOAIXmlWriter::~MOAIXmlWriter () {

	// once nodes are passed off to tiny xml, it cleans up memory in the following call.
	(( TiXmlDocument* )this->mDoc )->Clear ();
	delete (( TiXmlDocument* )this->mDoc );
}

//----------------------------------------------------------------//
void MOAIXmlWriter::SaveDocument ( cc8* filename ) {

	(( TiXmlDocument* )this->mDoc )->SaveFile ( filename );
}

#endif