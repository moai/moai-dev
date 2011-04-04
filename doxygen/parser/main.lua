----------------------------------------------------------------
function printf ( ... )

	return io.stdout:write ( string.format ( ... ))
end

----------------------------------------------------------------
function removeFormatting ( str )

	local ret = ""
	local func = string.gmatch ( str, "[^\t\n\r]" )
	local s = func ()
	if not s then return str end
	
	while s do
		ret = ret .. s
		s = func ()
	end
	
	return ret
end

----------------------------------------------------------------
function trim ( str )

	local nonWhiteSpacePattern = "[^%s]"
	local startIdx = string.find ( str, nonWhiteSpacePattern )
	local endIdx = 1 + ( #str - string.find ( string.reverse ( str ), nonWhiteSpacePattern ))
	local ret = string.sub ( str, startIdx, endIdx )
	
	return ret
end

----------------------------------------------------------------
doxygenBlock = {}

funcName = ""
funcText = ""
inParams = {}
optParams = {}
outParams = {}

function handleDoxygenBlock ()

	inParams = {}
	outParams = {}

	for i,v in ipairs ( doxygenBlock ) do
		
		if v.tag == "@name" then
			funcName = v.text
		elseif v.tag == "@text" then
			funcText = v.text
		elseif v.tag == "@in" then
			table.insert ( inParams, v )
		elseif v.tag == "@out" then
			table.insert ( outParams, v )
		elseif v.tag == "@opt" then
			table.insert ( optParams, v )
		elseif v.tag == "@overload" then
			print ( "overload" )
		end
	end
	
	-- output formatted doxygen stuff to file
	io.write ( "DOXYGEN!!!!!!11111ONEone" )
end

----------------------------------------------------------------
-- predox parser
----------------------------------------------------------------

predox = {}
predox.DOXY_START = 4
predox.DOXY_END = 5
predox.TEXT = 7
predox.echo = false

----------------------------------------------------------------
predox.onStartRule = function ( id, line, name )
	
	if id == predox.DOXY_END then
		predox.echo = false
		doxygenBlock = {}
		luadoxParser:loadString ( predox.text )
		luadoxParser:traverse ()
		handleDoxygenBlock ()
	end
end

----------------------------------------------------------------
predox.onEndRule = function ( id )

	if id == predox.DOXY_START then
		predox.echo = true
		predox.text = ""
	end
end

----------------------------------------------------------------
predox.onTerminal = function ( id, line, text )

	if predox.echo then
		predox.text = predox.text .. text
	elseif text ~= "\r" then
		io.write ( text )
	end
end

----------------------------------------------------------------
-- luadox parser
----------------------------------------------------------------

luadox = {}

luadox.TAG				= 3
luadox.TEXT				= 4
luadox.COMMAND			= 4

----------------------------------------------------------------
luadox.onStartRule = function ( id, line, name )
end

----------------------------------------------------------------
luadox.onEndRule = function ( id )

	if id == luadox.COMMAND then
	
		if luadox.tag and luadox.text then
			table.insert ( doxygenBlock, { tag = trim ( luadox.tag ), text = removeFormatting ( trim ( luadox.text ))})
		end
		
		luadox.tag = nil
		luadox.text = nil
	end
end

----------------------------------------------------------------
luadox.onTerminal = function ( id, line, text )
	
	if id == luadox.TAG then
		luadox.tag = text
	end

	if id == luadox.TEXT then
		luadox.text = text
	end
end

----------------------------------------------------------------
-- create the output file
----------------------------------------------------------------
io.output ( "output.cpp" )

----------------------------------------------------------------
-- do the parsing
----------------------------------------------------------------
luadoxParser = MOAIParser.new ()
luadoxParser:setCallbacks ( luadox.onStartRule, luadox.onEndRule, luadox.onTerminal )
luadoxParser:loadRules ( 'luadox/luadox.cgt' )

predoxParser = MOAIParser.new ()
predoxParser:setCallbacks ( predox.onStartRule, predox.onEndRule, predox.onTerminal )
predoxParser:loadRules ( 'predox/predox.cgt' )

predoxParser:loadFile ( 'input.cpp' )
predoxParser:traverse ()
io.close ()


