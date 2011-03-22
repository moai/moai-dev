local function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end 

printf ( "hello, moai!\n" )

MOAIDebugLines.setStyle ( MOAIDebugLines.COLLISION_PRIMS, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.OVERLAPS, 2, 1, 0, 0 )

scene = MOAILayer2D.new ()
scene:showDebugLines ()

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 0 )
scene:setViewport ( viewport )

camera = MOAICamera2D.new ()
scene:setCamera ( camera )

partition = MOAIPartition.new ()
scene:setPartition ( partition );

function onOverlap1 ( code, begin, x, y )
	if begin then
		printf ( "overlap 1: %d %d %d\n", code, x, y )
	else
		printf ( "overlap 1: END\n" )
	end
end

quad1 = MOAIQuad.new ()
quad1:setQuad (
	-32, 64,
	32, 64,
	64, -64,
	-64, -64
)
quad1:setCallback ( onOverlap1 )
partition:insertPrim ( quad1 )

function onOverlap2 ( code, begin, x, y )
	if begin then
		printf ( "overlap 2: %d %d %d\n", code, x, y )
	else
		printf ( "overlap 2: END\n" )
	end
end

quad2 = MOAIQuad.new ()
quad2:setRect ( -64, -64, 64, 64 )
quad2:setLoc ( 96, 96 )
quad2:setCallback ( onOverlap2 )
quad2:setCacheOverlaps ( false )
partition:insertPrim ( quad2 )

--quad1:setRot ( 100 )
--quad2:setRot ( 10 )

--quad1:moveLoc ( 0, -64, 1.5 )

quad1:moveRot ( 360, 12 )
quad2:moveRot ( 360, 12 )

MOAISim.pushRenderPass ( scene )
MOAISim.openWindow ( "cathead", 320, 480 )
