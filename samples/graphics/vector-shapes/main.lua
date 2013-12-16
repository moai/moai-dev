--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

MOAISim.openWindow ( "test", 640, 480 )
MOAIGfxDevice.setClearColor ( 1, 1, 1, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

drawing = MOAIVectorDrawing.new ()

	drawing:setCircleResolution ( 32 )

	drawing:setFillStyle ( MOAIVectorDrawing.FILL_SOLID )
	drawing:setFillColor ( 0.6, 0.75, 1.0, 1.0 )
	
	drawing:setStrokeStyle ( MOAIVectorDrawing.STROKE_EXTERIOR )
	drawing:setStrokeColor ( 0.45, 0.5, 1, 1  )
	drawing:setStrokeWidth ( 10 )
	drawing:setJoinStyle ( MOAIVectorDrawing.JOIN_ROUND )
	drawing:setCapStyle ( MOAIVectorDrawing.CAP_POINTY )
	drawing:setMiterLimit ( 10 )

	drawing:pushPoly ()
	drawing:pushBezierVertices ( -50, -50, -50, 50, 50, 50, 50, -50 )
	drawing:finish ()
	
	--drawing:pushRect ( -50, -50, 50, 50 )
	
	--drawing:pushScale ( 1, 0.5 )
	drawing:pushTranslate ( 50, 100 )
	--drawing:pushEllipse ( 0, 0, 100 )
	
	--drawing:pushPath ( -50, -50, 50, 50 )
	--drawing:pushPath ( 50, -50, 50, 50, -50, 50, -50, -50 )
	--drawing:pushPath ( 50, -50, -50, -50, -50, 50, 50, 50 )
	--drawing:pushPoly ( 50, -50, -50, -50, -50, 50, 50, 50 )
	--drawing:pushPoly ( 50, -50, 0, 50, -50, -50 )
	
	--[[
	drawing:pushCombo ()
		drawing:pushPoly ( -50, -150, -150, -150, -150, -50, -50, -50 )
		drawing:pushPoly ( 50, -50, -50, -50, -50, 50, 50, 50 )
		drawing:pushPoly ( 150, 50, 50, 50, 50, 150, 150, 150 )
	drawing:finish ()
	]]--
	
	--[[
	drawing:pushCombo ()
		drawing:pushPoly ( 50, -50, -50, -50, -50, 50, 50, 50 )
		drawing:pushPoly ( -50, 100, 50, 100, 0, 0 )
		drawing:pushPoly ( 50, -100, -50, -100, 0, 0 )
		drawing:pushPoly ( 0, 0, 100, -50, 100, 50 )
		drawing:pushPoly ( 0, 0, -100, 50, -100, -50 )
		drawing:pushPoly ( 25, -25, -25, -25, -25, 25, 25, 25 )
	drawing:finish ()
	]]--
	
	--[[
	drawing:pushPoly ()
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

layer:insertProp ( prop )
