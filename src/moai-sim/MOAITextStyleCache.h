// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTSTYLECACHE_H
#define	MOAITEXTSTYLECACHE_H

class MOAINode;
class MOAITextStyle;
class MOAITextStyleParser;

//================================================================//
// MOAITextStyleRef
//================================================================//
class MOAITextStyleRef {
private:

	friend class MOAITextLabel;
	friend class MOAITextStyleCache;

	MOAITextStyle*			mStyle;
	MOAITextStyleState		mState;

public:

	//----------------------------------------------------------------//
				MOAITextStyleRef		();
				~MOAITextStyleRef		();
	bool		NeedsLayout				() const;
	void		UpdateState				();
};

//================================================================//
// MOAITextStyleCache
//================================================================//
class MOAITextStyleCache {
private:

	friend class MOAITextDesignParser;
	friend class MOAITextStyleParser;

	// set only if we care about LuaRelease, LuaRetain and setting dep links to styles
	MOAINode* mOwner;

	// style set - these are the styles the texbox knows about
	// only need to get these during text styling, so using an STLMap for now...
	typedef STLMap < STLString, MOAITextStyleRef >::iterator StyleSetIt;
	STLMap < STLString, MOAITextStyleRef > mStyleSet;
	
	// anonymous styles - these are created on the fly as text is being styled
	ZLLeanStack < MOAITextStyleRef, 8 > mAnonymousStyles;
	
	//----------------------------------------------------------------//
	MOAITextStyle*		AddAnonymousStyle		( MOAITextStyle* source );
	void				ReleaseStyle			( MOAITextStyle* style );
	void				RetainStyle				( MOAITextStyle* style );

public:

	GET_SET ( MOAINode*, Owner, mOwner )
	
	//----------------------------------------------------------------//
	bool				CheckStylesChanged		();
	void				Clear					();
	void				ClearAnonymousStyles	();
	void				ClearNamedStyles		();
	MOAITextStyle*		GetStyle				();
	MOAITextStyle*		GetStyle				( cc8* styleName );
						MOAITextStyleCache		();
						~MOAITextStyleCache		();
	void				SetStyle				( MOAITextStyle* style );
	void				SetStyle				( cc8* styleName, MOAITextStyle* style );
};

#endif
