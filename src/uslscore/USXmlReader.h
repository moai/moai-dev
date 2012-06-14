// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USXMLREADER_H
#define	USXMLREADER_H

#include <expat.h>
#include <uslscore/STLList.h>
#include <uslscore/STLMap.h>
#include <uslscore/STLString.h>
#include <uslscore/USAccessors.h>

class USStream;

//================================================================//
// USXmlElement
//================================================================//
class USXmlElement {
private:

	STLString		mName;
	STLString		mText;
	STLString		mPath;
	
	u32				mDepth;
	u32				mChildren;

	STLMap < STLString, STLString > mAttributes;

public:

	friend class USXmlReader;

	GET ( u32, Depth, mDepth )
	GET ( STLString, Name, mName )
	GET ( STLString, Text, mText )
	GET ( STLString, Path, mPath )
	
	//----------------------------------------------------------------//
	STLString	GetAttribute	( cc8* name );
	STLString	GetLocalPath	( USXmlElement* parent );
	bool		HasAttribute	( cc8* name );
	bool		IsValue			();
};

//================================================================//
// USXmlReader
//================================================================//
class USXmlReader {
private:

	enum {
		BUFFER_SIZE = 512,
	};

	XML_Parser	mParser;
	USStream*	mStream;
	
	u32			mInputLen;
	char		mBuffer [ BUFFER_SIZE ];

	STLList < USXmlElement* > mElementStack;
	USXmlElement* mElement;
	
	bool			mSuspended;
	u32				mEvent;

	//----------------------------------------------------------------//
	static void		_onElementBegin		( void* userData, const char* name, const char** atts );
	static void		_onElementEnd		( void* userData, const char* name );
	static void		_onText				( void* userData, const char* text, int len );

	//----------------------------------------------------------------//
	USXmlElement*			GetTopElement		();
	void					OnElementBegin		( const char* name, const char** atts );
	void					OnElementEnd		( const char* name );
	void					OnText				( const char* text, int len );
	USXmlElement*			PopElement			();
	USXmlElement*			PushElement			( cc8* name );
	XML_Status				Resume				();
	void					SetElement			( USXmlElement* element );
	void					Suspend				( u32 event );

public:

	enum {
		DONE,
		ELEMENT,
		ELEMENT_BEGIN,
		ELEMENT_END,
		XML_ERROR,
		NONE,
	};

	GET ( u32, Event, mEvent )
	GET ( u32, Depth, ( u32 )mElementStack.size ())

	//----------------------------------------------------------------//
	void			Clear			();
	USXmlElement*	GetElement		();
	USXmlElement*	Parse			();
	void			SetStream		( USStream& stream );
					USXmlReader		();
					~USXmlReader	();
};

#endif
