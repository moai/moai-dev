----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function printAttributes ( attributes, indent )

	if attributes == nil then return end
	indent = indent or ""

	for key, value in pairs ( attributes ) do
		print ( indent .. key .. " = " .. value )
	end

end

function printElement ( element, indent )

	for key, value in ipairs ( element ) do
		
		print ( indent .. key .. ":" )
	
		if type ( value ) == "table" then
			printNode ( value, indent .. "    " )
		else
			print ( indent .. "    " .. value )
		end
	end
end

function printChildren ( children, indent )

	if children == nil then return end
	indent = indent or ""

	for key, value in pairs ( children ) do
		print ( indent .. key .. ":" )
		printElement ( value, indent .. "  " )
	end
end

function printNode ( node, indent )

	indent = indent or ""
	
	if node.type then
		print ( indent .. "type: " .. node.type )
	end

	if node.attributes then
		print ( indent .. "attributes" )
		printAttributes ( node.attributes, indent .. "  " )
	end
	
	if node.children then  
		print ( indent .. "children" )
		printChildren ( node.children, indent .. "  " )
	end
	
	if node.value then  
		print ( indent .. "value: " .. node.value )
	end
end

xml = MOAIXmlParser.parseFile ( "instructions.xml" )
printNode ( xml )

print ()
xml = MOAIXmlParser.parseFile ( "menu.xml" )
printNode ( xml )

print ()
xml = MOAIXmlParser.parseString ( "<?xml version='1.0' encoding='ISO-8859-1'?><foo><bar>bar</bar><baz>baz</baz><baz>bum</baz><baz>boo</baz></foo>" )
printNode ( xml )
