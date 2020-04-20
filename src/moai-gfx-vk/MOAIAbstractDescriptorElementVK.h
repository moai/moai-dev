// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTDESCRIPTORELEMENTVK_H
#define MOAIABSTRACTDESCRIPTORELEMENTVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>

class MOAIDescriptorSetSnapshotVK;

//================================================================//
// MOAIAbstractDescriptorElementVK
//================================================================//
class MOAIAbstractDescriptorElementVK :
	public virtual MOAIAbstractSnapshotVK {
protected:

	friend class MOAIDescriptorSetSnapshotVK;

	STLSet < MOAIDescriptorSetSnapshotVK* >					mDescriptorSets;

	//----------------------------------------------------------------//
	void		AffirmDescriptorSet					( MOAIDescriptorSetSnapshotVK& set );
	void		Invalidate							();
	void		RemoveDescriptorSet					( MOAIDescriptorSetSnapshotVK& set );

public:
	
	//----------------------------------------------------------------//
				MOAIAbstractDescriptorElementVK		();
				~MOAIAbstractDescriptorElementVK	();
};

#endif
