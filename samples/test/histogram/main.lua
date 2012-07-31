----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

MOAISim.setHistogramEnabled ( true )

function alloc ( class, count )

	count = count or 1

	for i = 1, count do
		class.new ()
	end
end

alloc ( MOAITexture,	1 )
alloc ( MOAIGfxQuad2D,	2 )
alloc ( MOAIProp2D,		3 )
alloc ( MOAILayer2D,	4 )

MOAISim.reportHistogram ()



