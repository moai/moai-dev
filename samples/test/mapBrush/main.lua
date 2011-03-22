print ( "hello, moai!" )

apiKey = "1d586f1b57ea51238741390130b237d2"

function getTileURL ( level, xTile, yTile )
	return string.format ( "http://tile.cloudmade.com/%s/1/256/%d/%d/%d.png", apiKey, level, xTile, yTile )
end

scene = MOAILayer2D.new ()
MOAISim.pushRenderPass ( scene )

viewport = MOAIViewport.new ()
viewport:setSize ( 480, 480 )
viewport:setScale ( 1, 0 )
scene:setViewport ( viewport )

camera = MOAICamera2D.new ()
scene:setCamera ( camera )

partition = MOAIPartition.new ()
scene:setPartition ( partition );

map = MOAISlippyMap.new ()
map:setLoadingTiles ( "map_spinner.png", "map_grid.png")
map:setFrame ( -0.5, -0.5, 0.5, 0.5 )
map:setTileURLCallback ( getTileURL )

partition:insertPrim ( map )

map:moveRot ( 360, 15 )
map:moveScl ( 32, 32, 15 )
map:moveLoc ( 10, -4, 15 )

MOAISim.openWindow ( "map", 480, 480 )
