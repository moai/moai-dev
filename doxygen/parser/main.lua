----------------------------------------------------------------
function parseParam ( v )

	local func = string.gmatch ( v.text, "[^%s]+" )
	v.paramType = func ()
	v.paramName = func ()
	v.paramText = ""
	
	local s = func ()
	while s do
		v.paramText = v.paramText .. " " .. s
		s = func ()
	end
end

----------------------------------------------------------------
function printf ( ... )

	return io.stdout:write ( string.format ( ... ))
end

----------------------------------------------------------------
function removeFormatting ( str )

	local ret = ""
	local func = string.gmatch ( str, "[^\n\r\f]" )
	local s = func ()
	
	if not s then 
		ret = str
	else
		while s do
			ret = ret .. s
			s = func ()
		end
	end
	
	local ret2 = ""
	for i = 1, #ret do
	
		s = string.sub ( ret, i, i )
		if ( s == "\t" ) then
			ret2 = ret2 .. " "
		else
			ret2 = ret2 .. s
		end
	end
		
	return ret2
end

----------------------------------------------------------------
function trim ( str )

	local nonWhiteSpacePattern = "[^%s]"
	local startIdx = string.find ( str, nonWhiteSpacePattern )
	local endIdx = 1 + ( #str - string.find ( string.reverse ( str ), nonWhiteSpacePattern ))
	return string.sub ( str, startIdx, endIdx )
end

----------------------------------------------------------------
doxygenBlock = {}

function handleDoxygenBlock ()

	funcName = ""
	funcText = ""
	inParams = {}
	optParams = {}
	outParams = {}
	curDef = {}

	for i,v in ipairs ( doxygenBlock ) do
		
		if v.tag == "@name" then
			
			funcName = v.text
		
		elseif v.tag == "@text" then
			
			funcText = v.text
		
		elseif v.tag == "@in" then

			parseParam ( v )
			table.insert ( inParams, v )
			
		elseif v.tag == "@out" then

			parseParam ( v )
			if v.paramName == nil then v.paramName = "" end			
			table.insert ( outParams, v )
		
		elseif v.tag == "@opt" then

			parseParam ( v )
			table.insert ( optParams, v )
			
		elseif v.tag == "@overload" then
			print ( "overload" )
		end
	end
	
	local argList = ""
	for i,v in ipairs ( inParams ) do
		if i ~= 1 then argList = argList .. ", " end
		argList = argList .. v.paramType .. " " .. v.paramName
	end
	
	if #optParams > 0 then
		
		argList = argList .. " [, "
		
		for i,v in ipairs ( optParams ) do
			if i ~= 1 then argList = argList .. ", " end
			argList = argList .. v.paramType .. " " .. v.paramName
		end
		
		argList = argList .. " ]"
	end
	
	local doxy = "\t@brief\n\t<tt>function " .. funcName .. " ( " .. argList .. " )</tt>\n\n"
	doxy = doxy .. "\t" .. funcText .. "\n"
	
	for i,v in ipairs ( inParams ) do
		doxy = doxy .. "\t@param " .. v.paramType .. " " .. v.paramName .. "\t\t" .. v.paramText .. "\n"
	end

	for i,v in ipairs ( optParams ) do
		doxy = doxy .. "\t@param " .. v.paramType .. " " .. v.paramName .. "\t<em>Optional.</em> " .. v.paramText .. "\n"
	end

	local returnList = ""
	for i,v in ipairs ( outParams ) do
		if i ~= 1 then returnList = returnList .. ", " end
		returnList = returnList .. v.paramType .. " " .. v.paramName
	end
	
	doxy = doxy .. "\t@return " .. returnList .. "\n"
	
	-- output formatted doxygen stuff to file
	io.write ( doxy )
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
	else
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
