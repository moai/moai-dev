//[MOAI LICENSE]

#ifndef	ZLADAPTERINFO_H
#define	ZLADAPTERINFO_H

#include <zl-core/STLString.h>
#include <zl-core/ZLAccessors.h>
#include <zl-core/ZLLeanArray.h>

//================================================================//
// ZLAdapterInfo
//================================================================//
class ZLAdapterInfo {
private:

	STLString mName;

public:

	struct USMacAddress {
		unsigned char bytes [6];
	};
	
	friend class USAdapterInfoList;

	GET ( STLString, Name, mName )

	//----------------------------------------------------------------//
	static STLString GetMACAddress ();
	
	void		SetNameFromMACAddress		( u8* address, u32 length );
};

//================================================================//
// USAdapterInfoList
//================================================================//
class USAdapterInfoList :
	public ZLLeanArray < ZLAdapterInfo > {
public:

	//----------------------------------------------------------------//
	void			EnumerateAdapters		();
};

#endif
