----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function printTable ( t )
	for k, v in pairs ( t ) do
		print ( k, v )
	end
	print ()
end

local frameBuffer = MOAIGfxDevice.getFrameBuffer ()
--local frameBuffer = MOAIFrameBuffer.new ()
print ( "SET CLEAR COLOR" )
print ( frameBuffer )
print ( frameBuffer.setClearColor )

local refTable = getmetatable ( frameBuffer )
local memberTable = getmetatable ( refTable )
local interfaceTable = getmetatable ( memberTable )

print ( refTable )
print ( memberTable )
print ( interfaceTable )

--[[
print ( MOAITextBox )
print ( MOAITextBox.new )
print ( 'interface ', MOAITextBox.getInterfaceTable ())
print ()

--============================================================--
-- MOAITextBox
--============================================================--
MOAITextBox.extend (

	'MOAITextBox',
	
	----------------------------------------------------------------
	function ( interface, class, superInterface, superClass )
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		-- extend the interface
		function interface.affirmStyle ( self )
			print ( 'AFFIRM STYLE!' )
			local style = self:getStyle ()
			print ( style )
			if not style then
				style = MOAITextStyle.new ()
				print ( style )
				self:setStyle ( style )
				print ( 'OK' )
			end
			return style
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setFont ( self, font )
			print ( 'SET FONT!' )
			print ( self.affirmStyle )
			local style = self:affirmStyle ()
			style:setFont ( font )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setTextSize ( self, points, dpi )
			local style = self:affirmStyle ()
			style:setSize ( points, dpi )
		end
		
		print ( 'interface ', interface )
	end
)

print ( MOAITextBox )
print ( MOAITextBox.new )
print ( 'interface ', MOAITextBox.getInterfaceTable ())
print ()

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'The quick <c:f70>brown<c> fox jumps over the <c:7f3>lazy<c> dog.'

textbox = MOAITextBox.new ()
font = MOAIFont.new ()

print ( 'TEXBOX' )
print ( textbox )
local refTable = getmetatable ( textbox )
local memberTable = getmetatable ( refTable )
local interfaceTable = getmetatable ( memberTable )

print ( 'refTable ', refTable )
print ( 'memberTable ', memberTable )
print ( 'interfaceTable ', interfaceTable )
print ()

--printTable ( interfaceTable )

--font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 12, 163 )
--textbox:setString ( text )

print ( textbox.setFont )
print ( textbox.setTextSize )
print ( textbox.setRect )
print ( textbox.setYFlip )

textbox:setFont ( font )
print ( 'OK' )
--textbox:setTextSize ( 12, 163 )
--textbox:setRect ( -150, -230, 150, 230 )
--textbox:setYFlip ( true )
--layer:insertProp ( textbox )
]]--
