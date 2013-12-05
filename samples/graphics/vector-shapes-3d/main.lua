--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

MOAISim.openWindow ( "test", 640, 480 )

gFrameBuffer = MOAIGfxDevice.getFrameBuffer ()
gFrameBuffer:setClearDepth ( true )
gFrameBuffer:setClearColor ( 1, 1, 1, 1 )

gViewport = MOAIViewport.new ()
gViewport:setSize ( 640, 480 )
gViewport:setScale ( 640, 480 )

gWorldLayer = MOAILayer2D.new ()
gWorldLayer:setViewport ( gViewport )
MOAISim.pushRenderPass ( gWorldLayer )

gCameraPivot = MOAITransform.new ()
gCameraSpin = MOAITransform.new ()

gCamera = MOAICamera.new ()
gCamera:setLoc ( 0, 0, gCamera:getFocalLength ( 640 ))
gWorldLayer:setCamera ( gCamera )

gCameraPivot:setAttrLink ( MOAIProp2D.INHERIT_TRANSFORM, gCameraSpin, MOAIProp2D.TRANSFORM_TRAIT )
gCamera:setAttrLink ( MOAIProp2D.INHERIT_TRANSFORM, gCameraPivot, MOAIProp2D.TRANSFORM_TRAIT )

gCameraPivot:setRot ( 80, 0, 0 )
--gCameraSpin:setRot ( 0, 0, -160 )
gCameraSpin:moveRot ( 0, 0, -360, 5 )

drawing = MOAIVectorDrawing.new ()

	drawing:setCircleResolution ( 32 )

	drawing:setFillStyle ( MOAIVectorDrawing.FILL_SOLID )
	drawing:setFillColor ( 0.6, 0.75, 1.0, 1.0 )

	drawing:setLightVec ( -1, -1 )
	
	drawing:setLightColor ( 1, 1, 1, 1 )
	drawing:setLightCurve ( MOAIEaseType.LINEAR ) -- SHARP_EASE_OUT
	
	drawing:setShadowColor ( 0, 0, 0, 0.5 )
	drawing:setShadowCurve ( MOAIEaseType.LINEAR )
	
	drawing:setExtrude ( 100 )
	--drawing:pushRect ( -50, -50, 50, 50 )
	drawing:pushEllipse ( 0, 0, 100 )
	drawing:setExtrude ( 0 )
	
	drawing:setStrokeStyle ( MOAIVectorDrawing.STROKE_EXTERIOR )
	drawing:setStrokeColor ( 0.45, 0.5, 1, 1  )
	drawing:setStrokeWidth ( 50 )
	drawing:setJoinStyle ( MOAIVectorDrawing.JOIN_ROUND )
	drawing:setCapStyle ( MOAIVectorDrawing.CAP_POINTY )
	drawing:setMiterLimit ( 10 )
	
	--drawing:pushRect ( -50, -50, 50, 50 )
	
	--drawing:pushEllipse ( 0, 0, 100 )
	
	--drawing:pushPath ( -50, -50, 50, 50 )
	--drawing:pushPath ( 50, -50, 50, 50, -50, 50, -50, -50 )
	--drawing:pushPath ( 50, -50, -50, -50, -50, 50, 50, 50 )
	--drawing:pushPolygon ( 50, -50, -50, -50, -50, 50, 50, 50 )
	--drawing:pushPolygon ( 50, -50, 0, 50, -50, -50 )
	
	--[[
	drawing:pushCombo ()
		drawing:pushPolygon ( -50, -150, -150, -150, -150, -50, -50, -50 )
		drawing:pushPolygon ( 50, -50, -50, -50, -50, 50, 50, 50 )
		drawing:pushPolygon ( 150, 50, 50, 50, 50, 150, 150, 150 )
	drawing:finish ()
	]]--
	
	--[[
	drawing:pushCombo ()
		drawing:pushPolygon ( 50, -50, -50, -50, -50, 50, 50, 50 )
		drawing:pushPolygon ( -50, 100, 50, 100, 0, 0 )
		drawing:pushPolygon ( 50, -100, -50, -100, 0, 0 )
		drawing:pushPolygon ( 0, 0, 100, -50, 100, 50 )
		drawing:pushPolygon ( 0, 0, -100, 50, -100, -50 )
		drawing:pushPolygon ( 25, -25, -25, -25, -25, 25, 25, 25 )
	drawing:finish ()
	]]--
	
	--[[
	drawing:pushPolygon ()
		drawing:pushVertex ( 175, 175 )
		drawing:pushVertex ( 175, -175 )
		drawing:pushVertex ( -175, -175 )
		drawing:pushVertex ( -175, 175 )
		drawing:pushVertex ( 75, 175 )
		drawing:pushVertex ( 75, -75 )
		drawing:pushVertex ( -75, -75 )
		drawing:pushVertex ( -75, 75 )
		drawing:pushVertex ( -25, 75 )
		drawing:pushVertex ( -25, -25 )
		drawing:pushVertex ( 25, -25 )
		drawing:pushVertex ( 25, 125 )
		drawing:pushVertex ( -125, 125 )
		drawing:pushVertex ( -125, -125 )
		drawing:pushVertex ( 125, -125 )
		drawing:pushVertex ( 125, 175 )
	drawing:finish ()
	]]--

drawing:finish ()

local deck = MOAIVectorDrawingDeck.new ()
deck:reserve ( 1 )
deck:setDrawing ( 1, drawing )

local prop = MOAIProp.new ()
prop:setDeck ( deck )
prop:setCullMode ( MOAIProp.CULL_BACK )
prop:setDepthTest ( MOAIProp.DEPTH_TEST_LESS_EQUAL )

gWorldLayer:insertProp ( prop )
