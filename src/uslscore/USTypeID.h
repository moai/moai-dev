// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ROOT_TYPEID_H
#define	ROOT_TYPEID_H

//================================================================//
// USTypeIDBase
//================================================================//
class USTypeIDBase {
protected:

	//----------------------------------------------------------------//
	static u32 GetUniqueID ()	{
		static u32 counter = 1;	
		return counter++;
	};
};

//================================================================//
// USTypeID
//================================================================//
template < typename	TYPE >
class USTypeID :
	public USTypeIDBase {
	public:
	
	//----------------------------------------------------------------//
	static u32 GetID ( void	) {
	
		static u32 type	= GetUniqueID ();
		return type;
	};
};

#endif
