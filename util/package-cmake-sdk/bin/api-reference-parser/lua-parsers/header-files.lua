----------------------------------------------------------------
function dumpTable ( list, label )

	-- create list before so we can check for tag text
	local hasTagText = false
	local listText = ""
	
	for i,v in ipairs ( list ) do
		
		listText = listText .. "\t\t<tr><td>" .. name .. "." .. v.tagName .. "</td>"
		
		if v.tagText ~= "" then
			listText = listText .. "<td>" .. v.tagText .. "</td>"		
			hasTagText = true
		end
		
		listText = listText .. "</tr>\n"
	end	

	-- dump header
	doxy = doxy .. "<td vertical-align=\"top\">\n"
	doxy = doxy .. "\t\t<table border=\"1\" cellpadding=\"4\">\n"
	doxy = doxy .. "\t\t<tr><td align=\"center\"><b>" .. label .. "</b></td>"
	
	if hasTagText then
		doxy = doxy .. "<td align=\"center\"><b>Description</b></td>"
	end
	
	doxy = doxy .. "</tr>\n"

	-- dump list
	doxy = doxy .. listText
	
	-- dump footer
	doxy = doxy .. "\t\t</table>\n"
	doxy = doxy .. "\t</td>"

end

----------------------------------------------------------------
function parseTag ( v )

	local func = string.gmatch ( v.text, "[^%s]+" )
	v.tagName = func ()
	v.tagText = ""
	
	local s = func ()
	
	while s do
		v.tagText = v.tagText .. " " .. s
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
	
	if not startIdx then
		return ""
	end
	
	local endIdx = 1 + ( #str - string.find ( string.reverse ( str ), nonWhiteSpacePattern ))
	return string.sub ( str, startIdx, endIdx )
end

----------------------------------------------------------------
doxygenBlock = {}

function handleDoxygenBlock ()

	name = ""
	text = ""
	constants = {}
	attributes = {}
	flags = {}
	
	for i,v in ipairs ( doxygenBlock ) do
		
		if v.tag == "@name" then
			name = v.text
		elseif v.tag == "@text" then
			text = v.text
		elseif v.tag == "@attr" then
			parseTag ( v )
			table.insert ( attributes, v )
		elseif v.tag == "@const" then
			parseTag ( v )
			table.insert ( constants, v )
		elseif v.tag == "@flag" then
			parseTag ( v )
			table.insert ( flags, v )
		end
	end
	
	doxy = "\t@brief " .. text .. "\n"

	local hasConstants = #constants > 0
	local hasAttributes = #attributes > 0
	local hasFlags = #flags > 0
	
	if hasConstants or hasAttributes or hasFlags then
		
		doxy = doxy .. "\t@htmlonly\n"
		doxy = doxy .. "\t<table cellpadding=\"8\"><tr>\n"
		
		-- output constants
		if hasConstants then
			dumpTable ( constants, "Constants" )
		end

		-- output attributes
		if hasAttributes then
			dumpTable ( attributes, "Attributes" )
		end

		-- output flags
		if hasFlags then
			dumpTable ( flags, "Flags" )
		end
		
		doxy = doxy .. "</tr></table>\n"
		doxy = doxy .. "\t@endhtmlonly\n"
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
		if ( string.find ( text, "@") ) then
			io.write ( string.gsub( text , "@", "") )
		else 
			io.write ( text )
		end
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
io.output ( "output.h" )

----------------------------------------------------------------
-- do the parsing
----------------------------------------------------------------
luadoxParser = MOAIParser.new ()
luadoxParser:setCallbacks ( luadox.onStartRule, luadox.onEndRule, luadox.onTerminal )
luadoxParser:loadRules ( 'lua-parsers/luadox/luadox.cgt' )

predoxParser = MOAIParser.new ()
predoxParser:setCallbacks ( predox.onStartRule, predox.onEndRule, predox.onTerminal )
predoxParser:loadRules ( 'lua-parsers/predox/predox.cgt' )

predoxParser:loadFile ( 'input.h' )
predoxParser:traverse ()

io.close ()
