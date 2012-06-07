----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

dofile ( '../lib/camera.lua' )
dofile ( '../lib/hero.lua' )
dofile ( '../lib/mesh.lua' )

MOAISim.openWindow ( "test", 1024, 640 )

viewport = MOAIViewport.new ()
viewport:setSize ( 1024, 640 )
viewport:setScale ( 1024, 640 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
layer:setSortMode ( MOAILayer.SORT_ISO )
MOAISim.pushRenderPass ( layer )

camera = makeIsometricCamera ()
layer:setCamera ( camera )

texture = MOAITexture.new ()
texture:load ( 'moai.png' )

local props = {}

props [ 1 ] = makeMeshProp ( makeBox ( 96, 96, 128, texture ))
props [ 1 ]:setLoc ( 128, 0 )

props [ 2 ] = makeMeshProp ( makeBox ( 224, 96, 128, texture ))
props [ 2 ]:setLoc ( 32, 128 )

props [ 3 ] = makeMeshProp ( makeBox ( 96, 96, 128, texture ))
props [ 3 ]:setLoc ( 0, 0 )

layer:insertProp ( props [ 1 ])
layer:insertProp ( props [ 2 ])
layer:insertProp ( props [ 3 ])

runHero ( props )
