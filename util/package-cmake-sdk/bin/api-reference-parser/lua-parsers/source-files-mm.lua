function dumpDef ( def, i )

	if i == 1 then
		dumpFunctionText ( def )
	end

	dumpFunctionDef ( def )

	for i,v in ipairs ( def.inParams ) do
		dumpParam ( v )
	end

	for i,v in ipairs ( def.optParams ) do
		dumpOptParam ( v )
	end

	dumpReturn ( def )
	
	if foundOverload then
		if def.overloadText then
			doxy = doxy .. "@note " .. def.overloadText .. "\n\n"
		end
	end
end

----------------------------------------------------------------
function dumpFunctionDef ( def )

	local argList = ""
	for i,v in ipairs ( def.inParams ) do
		if i ~= 1 then argList = argList .. ", " end

		if not v.paramType then v.paramType = "" end
		if not v.paramName then v.paramName = "" end
		
		argList = argList .. v.paramType .. " " .. v.paramName
	end

	if #def.optParams > 0 then
		
		if #def.inParams == 0 then
			argList = argList .. " [ " 
		else
			argList = argList .. " [, "
		end
		
		for i,v in ipairs ( def.optParams ) do
			if i ~= 1 then argList = argList .. ", " end
			argList = argList .. v.paramType .. " " .. v.paramName
		end
		
		argList = argList .. " ]"
	end
	
	local argText = " ( " .. argList .. " )"
	
	if argList == "" then
		argText = " ()"
	end
	
	doxy = doxy .. "\n\n<hr>\t"
	doxy = doxy .. "<tt>function " .. def.funcName .. argText .. "</tt>\n\n"
end

----------------------------------------------------------------
function dumpFunctionText ( def )

	doxy = doxy .. "\t" .. def.funcText .. "\n\n"
end

----------------------------------------------------------------
function dumpOptParam ( v )

	doxy = doxy .. "\t@param " .. v.paramName .. " ( " .. v.paramType .. " ) <em>Optional.</em> " .. v.paramText .. "\n"
end

----------------------------------------------------------------
function dumpParam ( v )

	doxy = doxy .. "\t@param " .. v.paramName .. " ( " .. v.paramType .. " ) " .. v.paramText .. "\n"
end

----------------------------------------------------------------
function dumpReturn ( def )

	local returnList = ""
	for i,v in ipairs ( def.outParams ) do
		if i ~= 1 then returnList = returnList .. ", " end
		
		if v.paramName == "" then
			returnList = returnList .. v.paramType
		else
			returnList = returnList .. v.paramName .. " ( " .. v.paramType .. " )"
		end
	end
	
	doxy = doxy .. "\t@return " .. returnList .. "\n"
end

----------------------------------------------------------------
function parseParam ( v )

	local func = string.gmatch ( v.text, "[^%s]+" )
	v.paramType = func ()
	
	if ( v.paramType == "nil" ) or ( v.paramType == "..." ) then
		v.paramName = ""
	else
		v.paramName = func ()
	end
	
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
function pushDef ()

	curDef = {}
	curDef.funcName = ""
	curDef.funcText = ""
	curDef.inParams = {}
	curDef.optParams = {}
	curDef.outParams = {}
	table.insert ( defs, curDef )
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
	
	if not startIdx then
		return ""
	end
	
	local endIdx = 1 + ( #str - string.find ( string.reverse ( str ), nonWhiteSpacePattern ))
	return string.sub ( str, startIdx, endIdx )
end

----------------------------------------------------------------
doxygenBlock = {}

function handleDoxygenBlock ()

	curDef = nil
	defs = nil
	foundOverload = false
	
	defs = {}
	pushDef ()
	
	for i,v in ipairs ( doxygenBlock ) do
		
		if v.tag == "@name" then
			
			curDef.funcName = v.text
		
		elseif v.tag == "@text" then
			
			curDef.funcText = v.text
		
		elseif v.tag == "@in" then

			parseParam ( v )
			table.insert ( curDef.inParams, v )
			
		elseif v.tag == "@out" then

			parseParam ( v )
			if v.paramName == nil then v.paramName = "" end			
			table.insert ( curDef.outParams, v )
		
		elseif v.tag == "@opt" then

			parseParam ( v )
			table.insert ( curDef.optParams, v )
			
		elseif v.tag == "@overload" then
			
			if foundOverload then
				local oldFuncName = curDef.funcName
				pushDef ()
				curDef.funcName = oldFuncName
			end

			curDef.overloadText = v.text			
			foundOverload = true
		end
	end
	
	doxy = "\t@brief"

	for i = 1, #defs do
		dumpDef ( defs [ i ], i )
	end
	
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
io.output ( "output.mm" )

----------------------------------------------------------------
-- do the parsing
----------------------------------------------------------------
luadoxParser = MOAIParser.new ()
luadoxParser:setCallbacks ( luadox.onStartRule, luadox.onEndRule, luadox.onTerminal )
luadoxParser:loadRules ( 'lua-parsers/luadox/luadox.cgt' )

predoxParser = MOAIParser.new ()
predoxParser:setCallbacks ( predox.onStartRule, predox.onEndRule, predox.onTerminal )
predoxParser:loadRules ( 'lua-parsers/predox/predox.cgt' )

predoxParser:loadFile ( 'input.mm' )
predoxParser:traverse ()

io.close ()
