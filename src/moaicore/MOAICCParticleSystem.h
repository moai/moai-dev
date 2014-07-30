//
//  MOAICCParticleSystem.h
//  libmoai
//
//  Created by Isaac Barrett on 7/30/14.
//
//

#ifndef MOAICCPARTICLESYSTEM_H
#define MOAICCPARTICLESYSTEM_H

#include <moaicore/MOAIProp.h>

class MOAICCParticleSystem : public virtual MOAIProp {
private:
	
	
public:
	DECL_LUA_FACTORY ( MOAICCParticleSystem )
	
					MOAICCParticleSystem	();
					~MOAICCParticleSystem	();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};


#endif /* defined(MOAICCPARTICLESYSTEM_H) */
