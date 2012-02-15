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
	MOAITextStyle*		FindStyle				( cc8* styleName );
	MOAITextStyle*		GetDefault				();
						MOAITextStyleSet		();
						~MOAITextStyleSet		();
	void				SetDefault				( MOAITextStyle* style );
	void				SetStyle				( cc8* styleName, MOAITextStyle* style );
};

#endif
