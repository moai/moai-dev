// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USStream.h>
#include <uslscore/USXmlReader.h>

//================================================================//
// USXmlElement
//================================================================//

//----------------------------------------------------------------//
STLString USXmlElement::GetAttribute ( cc8* name ) {

	if ( this->mAttributes.contains ( name )) {
		return this->mAttributes [ name ];
	}
	return "";
}

//----------------------------------------------------------------//
STLString USXmlElement::GetLocalPath ( USXmlElement* parent ) {

	if ( !parent ) return this->mPath;

	u32 parentPathSize = ( u32 )parent->mPath.size ();
	if ( strncmp ( this->mPath, parent->mPath, parentPathSize )) return this->mPath;

	return this->mPath.clip_to_back ( parentPathSize );
}

//----------------------------------------------------------------//
bool USXmlElement::HasAttribute ( cc8* name ) {

	return this->mAttributes.contains ( name );
}

//----------------------------------------------------------------//
bool USXmlElement::IsValue () {

	return ( this->mChildren == 0 );
}

//================================================================//
// callbacks
//================================================================//

//----------------------------------------------------------------//
void USXmlReader::_onElementBegin ( void *userData, const char *name, const char **atts ) {

	USXmlReader* reader = ( USXmlReader* )userData;
	reader->OnElementBegin ( name, atts );
}

//----------------------------------------------------------------//
void USXmlReader::_onElementEnd ( void *userData, const char *name ) {

	USXmlReader* reader = ( USXmlReader* )userData;
	reader->OnElementEnd ( name );
}

//----------------------------------------------------------------//
void USXmlReader::_onText ( void *userData, const char* text, int len ) {

	USXmlReader* reader = ( USXmlReader* )userData;
	reader->OnText ( text, len );
}

//================================================================//
// USXmlReader
//================================================================//

//----------------------------------------------------------------//
void USXmlReader::Clear () {

	if ( this->mParser ) {
		XML_ParserFree ( this->mParser );
		this->mParser = 0;
	}
	this->mStream = 0;
	
	while ( this->PopElement ());
	this->SetElement ( 0 );
}

//----------------------------------------------------------------//
USXmlElement* USXmlReader::GetElement () {

	switch ( this->mEvent ) {
		case ELEMENT_BEGIN:
			return this->GetTopElement ();
		
		case ELEMENT:
		case ELEMENT_END:
			return this->mElement;
	}
	
	return 0;
}

//----------------------------------------------------------------//
USXmlElement* USXmlReader::GetTopElement () {

	if ( this->mElementStack.size ()) {
		return this->mElementStack.back ();
	}
	return 0;
}

//----------------------------------------------------------------//
void USXmlReader::OnElementBegin ( const char* name, const char** atts ) {

	USXmlElement* element = this->PushElement ( name );
	for ( u32 i = 0; ( atts [ i ] && atts [ i + 1 ]); i += 2 ) {
		element->mAttributes [ atts [ i ]] = atts [ i + 1 ];
	}
	this->Suspend ( ELEMENT_BEGIN );
}

//----------------------------------------------------------------//
void USXmlReader::OnElementEnd ( const char* name ) {
	UNUSED ( name );

	this->PopElement ();

	if ( this->mSuspended ) {
		this->mEvent = ELEMENT;
	}
	else {
		this->Suspend ( ELEMENT_END );
	}
}

//----------------------------------------------------------------//
void USXmlReader::OnText ( const char* text, int len ) {

	USXmlElement* element = this->GetTopElement ();
	if ( element ) {
		if ( !element->mChildren ) {
			element->mText.append ( text, len );
		}
	}
}

//----------------------------------------------------------------//
USXmlElement* USXmlReader::Parse () {
	
	bool more = true;
	while ( more ) {
		
		int status = ( int )this->Resume ();
		
		switch ( status ) {
			
			case XML_STATUS_OK: {
				this->mInputLen = this->mStream->ReadBytes ( this->mBuffer, BUFFER_SIZE );
				if ( !this->mInputLen ) {
					this->mEvent = DONE;
					this->SetElement ( 0 );
					more = false;
				}
				break;
			}
			
			case XML_STATUS_SUSPENDED: {
				more = false;
				break;
			}
			
			case XML_ERROR_NOT_SUSPENDED:
			case XML_STATUS_ERROR: {
				fprintf ( stderr,
					"%s at line %d\n",
					XML_ErrorString ( XML_GetErrorCode ( this->mParser )),
					( int )XML_GetCurrentLineNumber ( this->mParser )
				);
				this->mEvent = XML_ERROR;
				this->SetElement ( 0 );
				more = false;
				break;
			}
		};
	}
	return this->GetElement ();
}

//----------------------------------------------------------------//
USXmlElement* USXmlReader::PopElement () {

	if ( this->mElementStack.size ()) {
		this->SetElement ( this->mElementStack.back ());
		this->mElementStack.pop_back ();
		return this->mElement;
	}

	this->SetElement ( 0 );
	return 0;
}

//----------------------------------------------------------------//
USXmlElement* USXmlReader::PushElement ( cc8* name ) {
	
	USXmlElement* top = this->GetTopElement ();
	if ( top ) {
		top->mChildren++;
		top->mText = "";
	}
	
	USXmlElement* element = new USXmlElement ();
	this->mElementStack.push_back ( element );
	
	element->mName = name;
	element->mDepth = ( u32 )this->mElementStack.size ();
	element->mChildren = 0;
	
	if ( top ) {
		element->mPath = top->mPath;
	}
	element->mPath.write ( "/%s", name );
	
	return element;
}

//----------------------------------------------------------------//
XML_Status USXmlReader::Resume () {

	this->mSuspended = false;
	this->mEvent = NONE;

	XML_ParsingStatus parsingStatus;
	XML_GetParsingStatus ( this->mParser, &parsingStatus );
	
	XML_Status status;
	
	if ( parsingStatus.parsing == XML_SUSPENDED ) {
		status = XML_ResumeParser ( this->mParser );
	}
	else {
		bool final = this->mInputLen < BUFFER_SIZE;
		status = XML_Parse ( this->mParser, this->mBuffer, this->mInputLen, final );
	}
	
	return status;
}

//----------------------------------------------------------------//
void USXmlReader::SetElement ( USXmlElement* element ) {

	if ( this->mElement ) {
		delete this->mElement;
	}
	this->mElement = element;
}

//----------------------------------------------------------------//
void USXmlReader::SetStream ( USStream& stream ) {

	this->Clear ();

	this->mStream = &stream;
	
	this->mParser = XML_ParserCreate ( NULL );
	XML_SetUserData ( this->mParser, this );
	XML_SetElementHandler ( this->mParser, _onElementBegin, _onElementEnd );
	XML_SetCharacterDataHandler ( this->mParser, _onText );
	
	this->mInputLen = this->mStream->ReadBytes ( this->mBuffer, BUFFER_SIZE );
}

//----------------------------------------------------------------//
void USXmlReader::Suspend ( u32 event ) {

	this->mEvent = event;
	XML_StopParser ( this->mParser, XML_TRUE );
	this->mSuspended = true;
}

//----------------------------------------------------------------//
USXmlReader::USXmlReader () :
	mParser ( 0 ),
	mElement ( 0 ) {
}

//----------------------------------------------------------------//
USXmlReader::~USXmlReader () {

	this->Clear ();
}
