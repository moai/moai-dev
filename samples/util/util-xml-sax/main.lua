----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function handleElement ( element )

	print ( 'name:', element.getName ())
	print ( 'text:', element.getText ())

	local attributes, size = element.getAttributes ()
		if attributes then
		print ( 'attributes:', size )
		for k, v in pairs ( attributes ) do
			print ( '', k, v )
		end
	end
	
	print ()
end

function onDone ( element )
	print ( 'DONE' )
	return false
end

function onElement ( element )
	print ( 'ELEMENT' )
	handleElement ( element )
	return true
end

function onElementBegin ( element )
	print ( 'ELEMENT_BEGIN' )
	handleElement ( element )
	return true
end

function onElementEnd ( element )
	print ( 'ELEMENT_END' )
	handleElement ( element )
	return true
end

function onXmlError ( element )
	print ( 'XML_ERROR' )
	return false
end

eventHandlers = {
	[ MOAIXmlParser.DONE ]				= onDone,
	[ MOAIXmlParser.ELEMENT ]			= onElement,
	[ MOAIXmlParser.ELEMENT_BEGIN ]		= onElementBegin,
	[ MOAIXmlParser.ELEMENT_END ]		= onElementEnd,
	[ MOAIXmlParser.XML_ERROR ]			= onXmlError,
}

stream = MOAIFileStream.new ()
stream:open ( 'instructions.xml' )

for event, element in MOAIXmlParser.events ( stream ) do
	eventHandlers [ event ]( element )
end
