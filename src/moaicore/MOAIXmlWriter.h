// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIXMLWRITER_H
#define	MOAIXMLWRITER_H

#include <tinyxml.h>

//================================================================//
// MOAIXmlWriter
//================================================================//
/**	@name	MOAIXmlWriter
	@text	Provides functions to programmatically write an XML document.
*/
class MOAIXmlWriter {
private:
	
	// current working node
	TiXmlElement*				mCurrent;

	// document
	TiXmlDocument				mDoc;

	// list of nodes; used as a stack
	STLList < TiXmlElement* >	mNodes;

	//----------------------------------------------------------------//
	//TODO: Add lua binding
	
	//----------------------------------------------------------------//


public:

	void		AddAttribute				( cc8* name, cc8* value );
	void		AddAttribute				( cc8* name, int value );
	void		AddAttribute				( cc8* name, double value );
	void		AddElement					( cc8* name );
	void		AddText						( cc8* text );
	void		CloseElement				();
				MOAIXmlWriter				();
				~MOAIXmlWriter				();
	void		SaveDocument				( cc8* filename );

	//TODO: Make it work with file streams??
};

#endif