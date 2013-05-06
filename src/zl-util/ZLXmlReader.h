// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLXMLREADER_H
#define	ZLXMLREADER_H

#if MOAI_WITH_EXPAT

#include <expat.h>
#include <zl-util/STLList.h>
#include <zl-util/STLMap.h>
#include <zl-util/STLString.h>
#include <zl-util/ZLAccessors.h>

class ZLStream;

//================================================================//
// ZLXmlElement
//================================================================//
class ZLXmlElement {
private:

	STLString		mName;
	STLString		mText;
	STLString		mPath;
	
	u32				mDepth;
	u32				mChildren;

	STLMap < STLString, STLString > mAttributes;

public:

	friend class ZLXmlReader;

	GET ( u32, Depth, mDepth )
	GET ( STLString, Name, mName )
	GET ( STLString, Text, mText )
	GET ( STLString, Path, mPath )
	
	//----------------------------------------------------------------//
	STLString	GetAttribute	( cc8* name );
	STLString	GetLocalPath	( ZLXmlElement* parent );
	bool		HasAttribute	( cc8* name );
	bool		IsValue			();
};

//================================================================//
// ZLXmlReader
//================================================================//
class ZLXmlReader {
private:

	enum {
		BUFFER_SIZE = 512,
	};

	XML_Parser	mParser;
	ZLStream*	mStream;
	
	u32			mInputLen;
	char		mBuffer [ BUFFER_SIZE ];

	STLList < ZLXmlElement* > mElementStack;
	ZLXmlElement* mElement;
	
	bool			mSuspended;
	u32				mEvent;

	//----------------------------------------------------------------//
	static void		_onElementBegin		( void* userData, const char* name, const char** atts );
	static void		_onElementEnd		( void* userData, const char* name );
	static void		_onText				( void* userData, const char* text, int len );

	//----------------------------------------------------------------//
	ZLXmlElement*			GetTopElement		();
	void					OnElementBegin		( const char* name, const char** atts );
	void					OnElementEnd		( const char* name );
	void					OnText				( const char* text, int len );
	ZLXmlElement*			PopElement			();
	ZLXmlElement*			PushElement			( cc8* name );
	XML_Status				Resume				();
	void					SetElement			( ZLXmlElement* element );
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
	ZLXmlElement*	GetElement		();
	ZLXmlElement*	Parse			();
	void			SetStream		( ZLStream& stream );
					ZLXmlReader		();
					~ZLXmlReader	();
};

#endif
#endif