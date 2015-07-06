--==============================================================
-- args
--==============================================================

SRC_PATH		= MOAI_SDK_HOME .. 'src/'
DST_PATH		= 'src-doxy-lua/'
HTML_PATH		= 'html/'
TEMPLATE_PATH	= 'template-lua/'
OUTPUT_DIR		= INVOKE_DIR .. 'lua-docs-html/'

print ( 'SRC_PATH', SRC_PATH )

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
	
	if param then

		if escape == 'o' or escape == 'out' then
			if not util.isAbsPath ( param ) then
				param = INVOKE_DIR .. param
			end 
			OUTPUT_DIR = MOAIFileSystem.getAbsoluteDirectoryPath ( param )
		end
	end
end

print ( 'OUTPUT_DIR', OUTPUT_DIR )

--==============================================================
-- util
--==============================================================

parseFile					= nil
parseParam					= nil
parseTag					= nil
prepareDoxyfile				= nil
removeFormatting			= nil
writeDefGroup				= nil
writeDoxygenBlock			= nil
writeDoxygenClassBlock		= nil
writeDoxygenFuncBlock		= nil
writeHtmlTable				= nil

----------------------------------------------------------------
parseFile = function ( src, dst )

	print ( 'parsing source file:', src )

	MOAIFileSystem.affirmPath ( util.getFolderFromPath ( dst ))

	local module = string.match ( string.sub ( src, #SRC_PATH + 1 ), '(moai%-[^/]+)/' )
	
	if module then
		
		-- trim off the Moai bit
		local name = string.sub ( module, 6 ) -- without the 'moai' bit

		local platformName = string.find ( name, '^ios' ) and 'iOS' or string.find ( name, '^android' ) and 'Android'
		local platform = platformName and string.lower ( platformName )

		if platform then
			writeDefGroup ( string.format ( '%s%s-module.h', DST_PATH, platform ), platform, platformName )
		end
		
		if name == platform then
			module = platform
		else
			name = platform and string.sub ( name, #platform + 2 ) or 'moai ' .. name
			name = string.lower ( string.gsub ( name, '%-', ' ' ))
			name = string.gsub ( name, '([%l])([%w]*)', function ( f, s ) return string.upper ( f ) .. s end )
			
			local moduleHeader = string.format ( '%s%s/%s-module.h', DST_PATH, module, module )
			writeDefGroup ( moduleHeader, module, name, platform )
		end
	end
	
	local str = util.loadFileAsString ( src )

	str = string.gsub ( str, 'private:', 'public:' )
	str = string.gsub ( str, '%)[%s]*=[%s]*0;', '%);' )

	local out = io.open ( dst, 'w' )
	
	for s, i in util.tokens ( str, '/%*%*.-%*/' ) do
		if i == 1 then
			writeDoxygenBlock ( out, s, module )
		else
			out:write ( s )
		end
	end

	out:close ()
end

----------------------------------------------------------------
parseParam = function ( v )

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

	v.paramName = v.paramName and v.paramName or ''
	v.paramType = v.paramType and v.paramType or ''
end

----------------------------------------------------------------
parseTag = function ( v )

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
prepareDoxyfile = function ( filename, tmpFilename )

	MOAIFileSystem.copy ( filename, tmpFilename )

	local str1 = MOAIFileSystem.checkFileExists ( SRC_PATH..'config/moai_version_major.h' ) and
		util.loadFileAsString ( SRC_PATH..'config/moai_version_major.h' ) or
		util.loadFileAsString ( SRC_PATH..'config-default/moai_version_major.h' )

	local str2 = util.loadFileAsString ( SRC_PATH..'config/moai_version_minor.h' ) and
		util.loadFileAsString ( SRC_PATH..'config/moai_version_minor.h' ) or
		util.loadFileAsString ( SRC_PATH..'config-default/moai_version_minor.h' )

	if str1 and str2 then

		major		= string.match ( string.sub ( str1, string.find ( str1, 'MOAI_SDK_VERSION_MAJOR %d' )),"%d+" )
		minor		= string.match ( string.sub ( str2, string.find ( str2, 'MOAI_SDK_VERSION_MINOR %d' )),"%d+" )
		revision	= string.match ( string.sub ( str2, string.find ( str2, 'MOAI_SDK_VERSION_REVISION %d' )),"%d+" )

		util.replaceInFile (  tmpFilename, {
			[ '@@VERSION@@' ] = string.format ( 'v%i.%i.%i', major, minor, revision ),
		})
	end
end

----------------------------------------------------------------
removeFormatting = function ( str )

	str = util.trim ( str )

	local ret = ''
	local func = string.gmatch ( str, '[^\n\r\f]' )
	local s = func ()
	
	if not s then 
		ret = str
	else
		while s do
			ret = ret .. s
			s = func ()
		end
	end
	
	local ret2 = ''
	for i = 1, #ret do
	
		s = string.sub ( ret, i, i )
		if ( s == '\t' ) then
			ret2 = ret2 .. ' '
		else
			ret2 = ret2 .. s
		end
	end
		
	return ret2
end

----------------------------------------------------------------
writeDefGroup = function ( filename, module, name, parent )
	
	if not MOAIFileSystem.checkFileExists ( filename ) then

		local out = io.open ( filename, 'w' )
		out:write ( string.format ( '/** @defgroup %s %s\n', module, name ))

		if parent then
			out:write ( string.format ( '* @ingroup %s\n', parent ))
		end

		out:write ( '*  @{@}\n' )
		out:write ( '*/\n' )
		out:close ()
	end
end

----------------------------------------------------------------
writeDoxygenBlock = function ( out, str, module )

	local block = str

	str = util.trim ( str, '/%*%*[%s]*', '[%s]*%*/' )

	local doxygenBlock = {}
	local isLua = false
	local isFunc = false
	local verbatim = false

	for s, i in util.tokens ( str, '@[^%s]*' ) do
		if i == 1 then
			
			local tag = util.trim ( s )
			table.insert ( doxygenBlock, { tag = tag, text = '' })
			
			if tag == '@defgroup' then
				verbatim = true
				break
			end
			
			if tag == '@lua' then
				isLua = true
			end
			
			if tag == '@in' or tag == '@out' then
				isFunc = true
			end
		else
			local entry = doxygenBlock [ #doxygenBlock ]
			if entry then entry.text = removeFormatting ( s ) end
		end
	end
	
	if verbatim then
		
		out:write ( block )
		
	elseif isLua then
		
		out:write ( '/**' )
		
		if isFunc then
			writeDoxygenFuncBlock ( out, doxygenBlock )
		else
			writeDoxygenClassBlock ( out, doxygenBlock, module )
		end
		
		out:write ( '*/' )
	end
end

----------------------------------------------------------------
writeDoxygenClassBlock = function ( out, doxygenBlock, module )
	
	out:write ( string.format ( '\n\t@ingroup %s\n', module ))
	
	name = ''
	text = ''
	constants = {}
	attributes = {}
	flags = {}
	
	for i,v in ipairs ( doxygenBlock ) do
		
		if v.tag == '@lua' then
			name = v.text
		elseif v.tag == '@text' then
			text = v.text
		elseif v.tag == '@attr' then
			parseTag ( v )
			table.insert ( attributes, v )
		elseif v.tag == '@const' then
			parseTag ( v )
			table.insert ( constants, v )
		elseif v.tag == '@flag' then
			parseTag ( v )
			table.insert ( flags, v )
		end
	end
	
	out:write ( '\t@brief ' .. text .. '\n' )

	local hasConstants = #constants > 0
	local hasAttributes = #attributes > 0
	local hasFlags = #flags > 0
	
	if hasConstants or hasAttributes or hasFlags then
		
		out:write ( '\n\t@htmlonly\n' )
		out:write ( '\t<table cellpadding=\"8\"><tr>\n' )
		
		-- output constants
		if hasConstants then
			writeHtmlTable ( out, constants, 'Constants' )
		end

		-- output attributes
		if hasAttributes then
			writeHtmlTable ( out, attributes, 'Attributes' )
		end

		-- output flags
		if hasFlags then
			writeHtmlTable ( out, flags, 'Flags' )
		end
		
		out:write ( '\t</tr></table>\n' )
		out:write ( '\t@endhtmlonly\n' )
	end
end

----------------------------------------------------------------
writeDoxygenFuncBlock = function ( out, doxygenBlock )
	
	local curDef = nil
	local defs = {}
	local foundOverload = false
	
	local pushDef = function ()
		curDef = {}
		curDef.funcName = ''
		curDef.funcText = ''
		curDef.inParams = {}
		curDef.optParams = {}
		curDef.outParams = {}
		table.insert ( defs, curDef )
	end
	
	pushDef ()
	
	for i, v in ipairs ( doxygenBlock ) do
		
		if v.tag == '@lua' then
			
			curDef.funcName = v.text
		
		elseif v.tag == '@text' then
			
			curDef.funcText = v.text
		
		elseif v.tag == '@in' then

			parseParam ( v )
			table.insert ( curDef.inParams, v )
			
		elseif v.tag == '@out' then

			parseParam ( v )		
			table.insert ( curDef.outParams, v )
		
		elseif v.tag == '@opt' then

			parseParam ( v )
			table.insert ( curDef.optParams, v )
			
		elseif v.tag == '@overload' then
			
			if foundOverload then
				local oldFuncName = curDef.funcName
				pushDef ()
				curDef.funcName = oldFuncName
			end

			curDef.overloadText = v.text			
			foundOverload = true
		end
	end
	
	out:write ( '\t@brief' )

	for i, def in ipairs ( defs ) do
		
		if i == 1 then
			out:write ( '\t' .. def.funcText .. '\n\n' )
		end

		local argList = ''
		for i, v in ipairs ( def.inParams ) do
			if i ~= 1 then argList = argList .. ', ' end

			if not v.paramType then v.paramType = '' end
			if not v.paramName then v.paramName = '' end
		
			argList = argList .. v.paramType .. ' ' .. v.paramName
		end

		if #def.optParams > 0 then
		
			if #def.inParams == 0 then
				argList = argList .. ' [ ' 
			else
				argList = argList .. ' [, '
			end
		
			for i, v in ipairs ( def.optParams ) do
				if i ~= 1 then argList = argList .. ', ' end
				argList = argList .. v.paramType .. ' ' .. v.paramName
			end
		
			argList = argList .. ' ]'
		end
	
		local argText = ' ( ' .. argList .. ' )'
	
		if argList == '' then
			argText = ' ()'
		end
	
		out:write (  '\t<hr><tt>function ' .. def.funcName .. argText .. '</tt>\n\n' )

		for i,v in ipairs ( def.inParams ) do
			out:write ( '\t@param ' .. v.paramType .. ' ' .. v.paramName .. ' ' .. v.paramText .. '\n' )
		end

		for i,v in ipairs ( def.optParams ) do
			out:write ( '\t@param ' .. v.paramType .. ' ' .. v.paramName .. ' <em>Optional.</em> ' .. v.paramText .. '\n' )
		end

		local returnList = ''
		for i,v in ipairs ( def.outParams ) do
			if i ~= 1 then returnList = returnList .. ', ' end
		
			if v.paramName == '' then
				returnList = returnList .. v.paramType
			else
				returnList = returnList .. v.paramType .. ' ' .. v.paramName
			end
		end
	
		out:write ( '\t@return ' .. returnList .. '\n' )
	
		if foundOverload then
			if def.overloadText then
				out:write ( '\t@note ' .. def.overloadText .. '\n\n' )
			end
		end
	end
end

----------------------------------------------------------------
writeHtmlTable = function ( out, list, label )

	-- create list before so we can check for tag text
	local hasTagText = false
	local listText = ''
	
	for i,v in ipairs ( list ) do
		
		listText = listText .. '\t\t\t<tr><td>' .. name .. '.' .. v.tagName .. '</td>'
		
		if v.tagText ~= '' then
			listText = listText .. '<td>' .. v.tagText .. '</td>'		
			hasTagText = true
		end
		
		listText = listText .. '</tr>\n'
	end	

	-- dump header
	out:write ( '\t\t<td vertical-align=\"top\">\n' )
	out:write ( '\t\t\t<table border=\"1\" cellpadding=\"4\">\n' )
	out:write ( '\t\t\t<tr><td align=\"center\"><b>' .. label .. '</b></td>' )
	
	if hasTagText then
		out:write ( '<td align=\"center\"><b>Description</b></td>' )
	end
	
	out:write ( '</tr>\n' )

	-- dump list
	out:write ( listText )
	
	-- dump footer
	out:write ( '\t\t\t</table>\n' )
	out:write ( '\t\t</td>\n' )
end

--==============================================================
-- here's where the magic happens
--==============================================================

MOAIFileSystem.deleteDirectory ( DST_PATH, true )
MOAIFileSystem.deleteDirectory ( HTML_PATH, true )

for filename in util.iterateFiles ( SRC_PATH, { '.c$', '.cpp$', '.h$', '.m$', '.mm$' }) do
	local src = SRC_PATH .. filename
	local dst = DST_PATH .. filename
	parseFile ( src, dst )
end

local DOXYFILE = 'doxyfile'
local DOXYFILE_TMP = 'doxyfile-tmp'

prepareDoxyfile ( DOXYFILE, DOXYFILE_TMP )
os.execute ( 'doxygen ' .. DOXYFILE_TMP )
MOAIFileSystem.deleteFile ( DOXYFILE_TMP )

local concat = function ( ... )
	return table.concat ( arg )
end

REPLACE_IN_HTML = {
	[ 'Static .- Member Functions' ]				= 'Function List',
	[ 'Member Function Documentation' ]				= 'Function Documentation',
	[ 'SUPPRESS_EMPTY_FILE_WARNING' ]				= '',
	[ 'static int[^<]*' ]							= '',
	[ '(>)_(.-[^<]*<)' ]							= concat,
	[ '%(lua_State %*L%)' ]							= '',
	[ 'int MOAI.-::_(.-</td>)' ]					= concat,
	[ '(>)lua_State[%s]*%*.-(<)' ]					= concat,
	[ '<em>L</em>' ]								= '',
	[ '<td class=\'paramtype\'></td>' ]				= '',
	[ '<td class=\'paramname\'></td>' ]				= '',
	[ '<td>%(</td>' ]								= '',
	[ '<td>%)</td>' ]								= '',
	[ '<span class="mlabel">public</span>' ]		= '',
	[ '<span class="mlabel">private</span>' ]		= '',
	[ '<span class="mlabel">protected</span>' ]		= '',
	[ '<span class="mlabel">static</span>' ]		= '',
	[ '<img src="closed.png" alt="%-"/>&#160;' ]	= '<img src="open.png" alt="-"/>&#160;',
	[ '(>)int _(.-[^<]*<)' ]						= concat,
}

REPLACE_IN_JS = {
	[ '(")_([%a%d]*")' ]							= concat,
}

for filename in util.iterateFiles ( HTML_PATH, { '.html$' }) do
	util.replaceInFile ( HTML_PATH .. filename, REPLACE_IN_HTML )
end

for filename in util.iterateFiles ( HTML_PATH, { '.js$' }) do
	util.replaceInFile ( HTML_PATH .. filename, REPLACE_IN_JS )
end

-- for testing

--MOAIFileSystem.copy ( 'html-lua/html/class_m_o_a_i_prop.html', 'test.html' )
--util.replaceInFile ( 'test.html', REPLACE_IN_HTML )

--MOAIFileSystem.copy ( 'html-lua/html/class_m_o_a_i_action.js', 'test.js' )
--util.replaceInFile ( 'test.js', REPLACE_IN_JS )

for filename in util.iterateFiles ( TEMPLATE_PATH ) do
	local src = TEMPLATE_PATH .. filename
	local dst = HTML_PATH .. 'html/' .. filename
	MOAIFileSystem.copy ( src, dst )
end

MOAIFileSystem.copy ( HTML_PATH, OUTPUT_DIR )

MOAIFileSystem.deleteDirectory ( DST_PATH, true )
MOAIFileSystem.deleteDirectory ( HTML_PATH, true )
