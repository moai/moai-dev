print ( "hello, moai!" )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )

texture = MOAITexture.new ()
texture:load ( "cathead.png" )
texture:setRect ( -64, -64, 64, 64 )

sprite = MOAISprite2D.new ()
sprite:setGfxSource ( texture )
sprite:moveRot ( 360, 1.5 )

layer:insertPrim ( sprite )
MOAISim.pushRenderPass ( layer )
MOAISim.openWindow ( "cathead", 320, 480 )

function onUpdate ( self )
	print ( self.name )
end

nodesly = MOAIScriptNode.new ()
nodesly.name = "Nodesly"
nodesly:setCallback ( onUpdate )

nodebert = MOAIScriptNode.new ()
nodebert.name = "Nodebert"
nodebert:setDependency ( nodesly )
nodebert:setCallback ( onUpdate )

function main ()
	while true do
		nodesly:scheduleUpdate ()
		coroutine.yield ()
	end
end

thread = MOAIThread.new ()
thread:run ( main )
