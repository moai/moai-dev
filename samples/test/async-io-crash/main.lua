print ( "main.lua" )

local state = {}

for i = 1, 100 do
	state [ i ] = "blarg"
end

local dataBuffer = MOAIDataBuffer.new ()
local saveAction

local thread = MOAIThread.new ()
thread:run (

	function ()
		
		print ( "thread" )
		
		function newFont ( size )
			local font = MOAIFont.new ()
			font:loadFromTTF ( "Dwarves.ttf", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,?!$:;'/\"+-=%", size )
			return font			
		end
		
		while true do
			
			coroutine.yield ()
			
			local font16 = newFont ( 16 )

			local textbox = MOAITextBox.new ()
			textbox:setAlignment ( MOAITextBox.LEFT_JUSTIFY )
			textbox:setYFlip ( true )
			textbox:setRect ( -128, -128, 128, 128 )
			textbox:setFont ( font16 )
			textbox:setTextSize ( 16 )
			textbox:setString ( "blarg" )
			
			 if not saveAction then
								
				dataBuffer:setString ( MOAISerializer.serializeToString ( state ))
				saveAction = dataBuffer:saveAsync ( "state.lua" )
				
				saveAction:setCallback ( 
				
					function () 
						saveAction = nil 
					end 
				)
			end
		end
	end
)

MOAISim.openWindow ( "blarg", 200, 200 )
