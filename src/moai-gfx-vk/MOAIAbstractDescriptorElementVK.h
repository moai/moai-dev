// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTDESCRIPTORELEMENTVK_H
#define MOAIABSTRACTDESCRIPTORELEMENTVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>

class MOAIDescriptorSetVK;

//================================================================//
// MOAIAbstractDescriptorElementVK
//================================================================//
class MOAIAbstractDescriptorElementVK :
	public virtual MOAIAbstractSnapshotVK {
protected:

	friend class MOAIDescriptorSetVK;

	STLSet < MOAIDescriptorSetVK* >					mDescriptorSets;

	//----------------------------------------------------------------//
	void		AffirmDescriptorSet					( MOAIDescriptorSetVK& set );
	void		Invalidate							();
	void		RemoveDescriptorSet					( MOAIDescriptorSetVK& set );

public:
	
	//----------------------------------------------------------------//
				MOAIAbstractDescriptorElementVK		();
				~MOAIAbstractDescriptorElementVK	();
};

#endif
