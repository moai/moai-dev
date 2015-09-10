----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local STATE_FILENAME		= 'count.lua'
local MARKED_FILENAME		= 'marked.lua'
local SAMPLES_DIRECTORY		= MOAI_SDK_HOME .. '/samples/'

local gCount	= MOAIFileSystem.checkFileExists ( STATE_FILENAME ) and dofile ( STATE_FILENAME ).count or 1
local gMarked	= MOAIFileSystem.checkFileExists ( MARKED_FILENAME ) and dofile ( MARKED_FILENAME ) or {}

local gDirectories = MOAIFileSystem.listDirectories ( SAMPLES_DIRECTORY );

local PREV		= -1
local NONE		= 0
local NEXT		= 1

local gRun		= true
local gStep		= NONE

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do

	if ( escape == 'c' or escape == 'clear' ) or (( escape == 'C' or escape == 'CLEAR' ) and param ) then

		gRun = false
		
		sample = param or gDirectories [ gCount ]
		if sample then
			gMarked [ sample ] = nil
			MOAISerializer.serializeToFile ( MARKED_FILENAME, gMarked )
		end
	end

	if ( escape == 'g' or escape == 'goto' ) and param then

		for i, v in ipairs ( gDirectories ) do
			if v == param then
				gCount = i
				break
			end
		end
	end

	if escape == 'l' or escape == 'list' then

		gRun = false

		local known = {}

		for i, v in ipairs ( gDirectories ) do
			if gMarked [ v ] then
				known [ v ] = v
				print ( v )
			end
		end

		for k, v in pairs ( gMarked ) do
			if not known [ v ] then
				print ( '?', v )
			end
		end
	end

	if ( escape == 'm' or escape == 'mark' ) or (( escape == 'M' or escape == 'MARK' ) and param ) then

		gRun = false

		sample = param or gDirectories [ gCount ]
		if sample then
			gMarked [ sample ] = sample
			MOAISerializer.serializeToFile ( MARKED_FILENAME, gMarked )
		end
	end
	
	if escape == 'r' or escape == 'restart' then
		gRun = false
		gCount = 1
	end

	if escape == 'n' or escape == 'next' then
		gStep = NEXT
	end

	if escape == 'p' or escape == 'prev' then
		gStep = PREV
	end

	if escape == 's' or escape == 'skip' then
		gRun = false
		gStep = NEXT
	end

	if escape == 'x' or escape == 'xclear' then

		gRun = false
		
		gMarked = {}
		MOAISerializer.serializeToFile ( MARKED_FILENAME, gMarked )
	end
end

if gStep == NEXT then
	gCount = gCount + 1
end

if gStep == PREV then
	gCount = gCount - 1
end

if gCount < 1 then
	gCount = #gDirectories
end

if not gDirectories [ gCount ] then
	gCount = 1
end

MOAISerializer.serializeToFile ( STATE_FILENAME, { count = gCount })

if gRun then

	sample = gDirectories [ gCount ]

	if sample then
		MOAIFileSystem.setWorkingDirectory ( SAMPLES_DIRECTORY .. sample )

		print ( 'RUNNING SAMPLE:', sample, '\n' )

		if MOAIFileSystem.checkFileExists ( 'main.lua') then
			dofile ( 'main.lua' )
		else
			print ( 'WARNING: no main.lua found' )
		end
	end
end
