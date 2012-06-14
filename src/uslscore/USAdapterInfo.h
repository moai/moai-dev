//[MOAI LICENSE]

#ifndef	USADAPTERINFO_H
#define	USADAPTERINFO_H

#include <uslscore/STLString.h>
#include <uslscore/USAccessors.h>
#include <uslscore/USLeanArray.h>

//================================================================//
// USAdapterInfo
//================================================================//
class USAdapterInfo {
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
	public USLeanArray < USAdapterInfo > {
public:

	//----------------------------------------------------------------//
	void			EnumerateAdapters		();
};

#endif
