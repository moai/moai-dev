// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLTYPEID_H
#define	ZLTYPEID_H

//================================================================//
// ZLTypeIDBase
//================================================================//
class ZLTypeIDBase {
protected:

	//----------------------------------------------------------------//
	static u32 GetUniqueID ()	{
		static u32 counter = 1;	
		return counter++;
	};
};

//================================================================//
// ZLTypeID
//================================================================//
template < typename	TYPE >
class ZLTypeID :
	public ZLTypeIDBase {
	public:
	
	//----------------------------------------------------------------//
	static const TYPE& Dummy () {
	
		static TYPE dummy = TYPE ();
		return dummy;
	}
	
	//----------------------------------------------------------------//
	static TYPE& DummyRef () {
	
		// intent is to create an object that will crash if accessed.
		// on system withouth memory protection (are there sill any?) this
		// should be redefined to just return a statically declared sentinel.
	
		return *( TYPE* )0;
	}
	
	//----------------------------------------------------------------//
	static u32 GetID ( void	) {
	
		static u32 type	= GetUniqueID ();
		return type;
	};
};

#endif
