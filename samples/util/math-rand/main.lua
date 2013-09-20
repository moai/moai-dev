----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIMath.seedSFMT ( 0 )

local file = io.open ( 'rand.csv', 'w' )

function buildHistogram ( randFunc, samples, range )
	local histogram = {}
	
	for i = 0, range do
		histogram [ i ] = 0
	end
	
	for i = 1, samples do
		local r = math.floor (( randFunc () * range ) + 0.5 )
		histogram [ r ] = histogram [ r ] + 1
	end
	return histogram
end

local SAMPLES = 1000000
local RANGE = 100

local histogram_rand = buildHistogram ( math.random, SAMPLES, RANGE )
local histogram_sfmt = buildHistogram ( MOAIMath.randSFMT, SAMPLES, RANGE )

for i = 0, RANGE do
	local rand = histogram_rand [ i ]
	local sfmt = histogram_sfmt [ i ]
	print ( rand, sfmt )
	file:write ( string.format ( '%g,%g\n', rand, sfmt ))
end

file:close ()
