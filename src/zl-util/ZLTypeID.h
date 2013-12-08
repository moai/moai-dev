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
	static u32 GetID ( void	) {
	
		static u32 type	= GetUniqueID ();
		return type;
	};
};

#endif
