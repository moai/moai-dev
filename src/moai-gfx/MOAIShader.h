// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADER_H
#define	MOAISHADER_H

//================================================================//
// MOAIShader
//================================================================//
class MOAIShader :
	public virtual MOAILuaObject {
	
	//----------------------------------------------------------------//
	virtual bool	MOAIShader_IsReady		() const = 0;
	
public:

	//----------------------------------------------------------------//
	bool			HasProgram				() const;
	bool			IsReady					() const;
					MOAIShader				();
					~MOAIShader				();
	void			ScheduleTextures		();
	void			UpdateUniforms			();
};

#endif
