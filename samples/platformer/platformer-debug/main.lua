----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 480, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 480, 480 )
viewport:setScale ( 480, 480 )

function makePlatformerProp ( hrad, vrad, floorAngle, ceilingAngle )
	local prop = MOAIPlatformerBody2D.new ()
	layer:insertProp ( prop )
	prop:setEllipse ( hrad, vrad )
	prop:setFloorAngle ( floorAngle )
	prop:setCeilingAngle ( ceilingAngle )
	return prop
end

function makeSurfaceDeck ( size )
	local surfaceDeck = MOAISurfaceDeck2D.new ()
	surfaceDeck:reserveSurfaceLists ( 1 )
	surfaceDeck:reserveSurfaces ( 1, size )
	return surfaceDeck
end

function makeTerrainProp ( surfaceDeck )
	local prop = MOAIProp.new ()
	prop:setDeck ( surfaceDeck )
	layer:insertProp ( prop )
	return prop
end

function printStatus ( body )
	coroutine.yield ()
	local steps, completed = body:getStatus ()
	print ( steps, completed )
end

function pushStandardMoves ( queueStack, platformer )

	pushQueue ( queueStack,
		function ()
			platformer:setMove ( -32, 0 )
			printStatus ( platformer )
		end,
		function ()
			platformer:setMove ( 64, 0 )
			printStatus ( platformer )
		end,
		function ()
			platformer:setMove ( 0, -64 )
			printStatus ( platformer )
		end,
		nil
	)
end

function pushQueue ( queueStack, ... )
	table.insert ( queueStack, arg )
end

function queueStackNext ( queueStack )
	if #queueStack > 0 then
		local queue = queueStack [ #queueStack ]
		if #queue > 0 then
			return table.remove ( queue, 1 )
		else
			table.remove ( queueStack )
			return queueStackNext ( queueStack )
		end
	end
end

function reset ()
	MOAIRenderMgr.clearRenderStack ()
	layer = MOAILayer2D.new ()
	layer:setViewport ( viewport )
	MOAIRenderMgr.pushRenderPass ( layer )
end

----------------------------------------------------------------
function testMove ()

	reset ()

	local surfaceDeck = makeSurfaceDeck ( 1 )
	surfaceDeck:setSurface ( 1, 1, 128, 0, -128, 0 )
	makeTerrainProp ( surfaceDeck )
	
	local platformer = makePlatformerProp ( 32, 32, 60, 0 )
	pushStandardMoves ( queueStack, platformer )
end

----------------------------------------------------------------
function testPeak ()

	reset ()

	local surfaceDeck = makeSurfaceDeck ( 2 )
	surfaceDeck:setSurface ( 1, 1, 128, -128, 0, 0 )
	surfaceDeck:setSurface ( 1, 2, 0, 0, -128, -128 )
	
	makeTerrainProp ( surfaceDeck )
	
	local platformer = makePlatformerProp ( 32, 32, 60, 0 )
	pushStandardMoves ( queueStack, platformer )
end

----------------------------------------------------------------
function testValley ()

	reset ()

	local surfaceDeck = makeSurfaceDeck ( 2 )
	surfaceDeck:setSurface ( 1, 1, 128, 128, 0, 0 )
	surfaceDeck:setSurface ( 1, 2, 0, 0, -128, 128 )
	
	makeTerrainProp ( surfaceDeck )
	
	local platformer = makePlatformerProp ( 32, 32, 60, 0 )
	pushStandardMoves ( queueStack, platformer )
end

----------------------------------------------------------------
function testRightT ()

	reset ()

	local surfaceDeck = makeSurfaceDeck ( 2 )
	surfaceDeck:setSurface ( 1, 1, 128, -128, -128, 128 )
	surfaceDeck:setSurface ( 1, 2, 0, 0, -128, -128 )
	
	makeTerrainProp ( surfaceDeck )
	
	local platformer = makePlatformerProp ( 32, 32, 60, 0 )
	pushStandardMoves ( queueStack, platformer )
end

----------------------------------------------------------------
function testLeftT ()

	reset ()

	local surfaceDeck = makeSurfaceDeck ( 2 )
	surfaceDeck:setSurface ( 1, 1, 128, 128, -128, -128 )
	surfaceDeck:setSurface ( 1, 2, 128, -128, 0, 0 )
	
	makeTerrainProp ( surfaceDeck )
	
	local platformer = makePlatformerProp ( 32, 32, 60, 0 )
	pushStandardMoves ( queueStack, platformer )
end

----------------------------------------------------------------
function testRightY ()

	reset ()

	local surfaceDeck = makeSurfaceDeck ( 2 )
	surfaceDeck:setSurface ( 1, 1, 128, 128, 0, 0 )
	surfaceDeck:setSurface ( 1, 2, 128, -128, -128, 128 )
	
	makeTerrainProp ( surfaceDeck )
	
	local platformer = makePlatformerProp ( 32, 32, 60, 0 )
	pushStandardMoves ( queueStack, platformer )
end

----------------------------------------------------------------
function testLeftY ()

	reset ()

	local surfaceDeck = makeSurfaceDeck ( 2 )
	surfaceDeck:setSurface ( 1, 1, 128, 128, -128, -128 )
	surfaceDeck:setSurface ( 1, 2, 0, 0, -128, 128 )
	
	makeTerrainProp ( surfaceDeck )
	
	local platformer = makePlatformerProp ( 32, 32, 60, 0 )
	pushStandardMoves ( queueStack, platformer )
end

----------------------------------------------------------------
function testRampShove ()

	reset ()

	local surfaceDeck = makeSurfaceDeck ( 2 )
	--surfaceDeck:setSurface ( 1, 1, 128, -192, -128, 64 )
	surfaceDeck:setSurface ( 1, 1, 128, 64, -128, -192 )
	surfaceDeck:setSurface ( 1, 2, -64, -256, -64, 0 )
	
	makeTerrainProp ( surfaceDeck )
	
	local platformer = makePlatformerProp ( 128, 128, 60, 0 )
	platformer:setLoc ( -32, 8 )
	printStatus ( platformer )

end

----------------------------------------------------------------
function testValleyShoveA ()

	reset ()

	local surfaceDeck = makeSurfaceDeck ( 3 )
	surfaceDeck:setSurface ( 1, 1, 128, 64, 0, -64 )
	surfaceDeck:setSurface ( 1, 2, 0, -64, -128, 64 )
	surfaceDeck:setSurface ( 1, 3, -64, 0, -64, 256 )
	
	makeTerrainProp ( surfaceDeck )
	
	local platformer = makePlatformerProp ( 128, 128, 60, 0 )
	platformer:setLoc ( -32, 0 )
	printStatus ( platformer )

end

----------------------------------------------------------------
function testValleyShoveB ()

	reset ()

	local surfaceDeck = makeSurfaceDeck ( 3 )
	surfaceDeck:setSurface ( 1, 1, 128, 16, 0, 0 )
	surfaceDeck:setSurface ( 1, 2, 0, 0, -128, 0 )
	surfaceDeck:setSurface ( 1, 3, -64, 0, -64, 256 )
	
	makeTerrainProp ( surfaceDeck )
	
	local platformer = makePlatformerProp ( 128, 128, 60, 0 )
	platformer:setLoc ( -32, 0 )
	printStatus ( platformer )

end

----------------------------------------------------------------
function testValleyShoveC ()

	reset ()

	local surfaceDeck = makeSurfaceDeck ( 5 )
	surfaceDeck:setSurface ( 1, 1, 128, 16, 16, 0 )
	surfaceDeck:setSurface ( 1, 2, 16, 0, 0, -16 )
	surfaceDeck:setSurface ( 1, 3, 0, -16, -16, 0 )
	surfaceDeck:setSurface ( 1, 4, -16, 0, -128, 0 )
	surfaceDeck:setSurface ( 1, 5, -64, 0, -64, 256 )
	
	makeTerrainProp ( surfaceDeck )
	
	local platformer = makePlatformerProp ( 128, 128, 60, 0 )
	platformer:setLoc ( -32, 0 )
	printStatus ( platformer )

end

----------------------------------------------------------------
-- queue
queueStack = {}
pushQueue ( queueStack,
	testMove,
	testPeak,
	testValley,
	testRightT,
	testLeftT,
	testRightY,
	testLeftY,
	testRampShove,
	testValleyShoveA,
	testValleyShoveB,
	testValleyShoveC,
	function () print ( 'DONE' ) reset () end
)

doNext = queueStackNext ( queueStack )

function onKeyboardEvent ( key, down )
	if MOAIInputMgr.device.keyboard:keyDown ( ' ' ) then doNext = queueStackNext ( queueStack ) end
end

MOAIInputMgr.device.keyboard:setCallback ( onKeyboardEvent )

function main ()
	while true do
		if doNext then
			doNext ()
			doNext = nil
		end
		coroutine.yield ()
	end
end

local thread = MOAICoroutine.new ()
thread:run ( main )
