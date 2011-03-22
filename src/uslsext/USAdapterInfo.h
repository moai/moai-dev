//[MOAI LICENSE]

#ifndef	USADAPTERINFO_H
#define	USADAPTERINFO_H

//================================================================//
// USAdapterInfo
//================================================================//
class USAdapterInfo {
private:

	STLString mName;

public:

	friend class USAdapterInfoList;

	GET ( STLString, Name, mName )

	//----------------------------------------------------------------//
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
