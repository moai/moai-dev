// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADER_H
#define	MOAISHADER_H

#include <moaicore/MOAINode.h>

class MOAIDeck2D;
class MOAILayer2D;

//================================================================//
// MOAIShader
//================================================================//
/**	@brief Base class for shaders.
*/
class MOAIShader :
	public virtual MOAINode {
protected:
	
	//----------------------------------------------------------------//

public:
	
	//----------------------------------------------------------------//
	virtual void	Bind				();
					MOAIShader			();
					~MOAIShader			();
	STLString		ToString			();
};

#endif
