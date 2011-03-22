// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USATTRMAP_H
#define USATTRMAP_H

class USAttributed;

//================================================================//
// USAttrMap
//================================================================//
class USAttrMap {
private:

	typedef STLMap < u32, u32 >::iterator MapIt;
	STLMap < u32, u32 > mMap;

public:

	//----------------------------------------------------------------//
	void		Clear				();
	void		MapAttr				( u32 primeID, u32 subID );
	void		ReadAttributes		( USAttributed& prime, USAttributed& sub );
				USAttrMap			();
				~USAttrMap			();
	void		WriteAttributes		( USAttributed& prime, USAttributed& sub );
};

#endif
