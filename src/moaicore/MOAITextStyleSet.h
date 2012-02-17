// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTSTYLESET_H
#define	MOAITEXTSTYLESET_H

class MOAITextStyle;

//================================================================//
// MOAITextStyleSet
//================================================================//
class MOAITextStyleSet {
private:

	// for now
	STLMap < STLString, MOAITextStyle* > mStyleMap;
	MOAITextStyle* mDefault;
	
public:

	friend class MOAITextStyler;
	
	//----------------------------------------------------------------//
	MOAITextStyle*		GetStyle				();
	MOAITextStyle*		GetStyle				( cc8* styleName );
						MOAITextStyleSet		();
						~MOAITextStyleSet		();
	void				SetStyle				( MOAITextStyle* style );
	void				SetStyle				( cc8* styleName, MOAITextStyle* style );
};

#endif
