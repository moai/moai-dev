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
public:

	typedef STLMap < STLString, STLString > AttributeMap;
	typedef AttributeMap::iterator AttributeMapIt;
	typedef AttributeMap::const_iterator AttributeMapConstIt;

private:

	STLString		mName;
	STLString		mText;

	u32				mDepth;
	u32				mChildren;

	AttributeMap mAttributes;

public:

	friend class ZLXmlReader;

	GET ( u32, Depth, mDepth )
	GET ( const STLString&, Name, mName )
	GET ( const STLString&, Text, mText )
	GET ( const AttributeMap&, Attributes, mAttributes )
	
	//----------------------------------------------------------------//
	STLString	GetAttribute	( cc8* name );
	bool		HasAttribute	( cc8* name );
	bool		IsValue			();
};

//================================================================//
// ZLXmlReader
//================================================================//
// This class implements an event-loop style SAX parser. To use, set
// the stream then loop until Parse () returns nil. Every loop check
// the event and respond accordingly. 
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
	int				GetLineNumber	();
	ZLXmlElement*	Parse			();
	void			SetStream		( ZLStream& stream );
					ZLXmlReader		();
					~ZLXmlReader	();
};

#endif
#endif