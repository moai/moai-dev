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
	
	drawing:setLineStyle ( MOAIVectorDrawing.LINE_STROKE )
	drawing:setLineColor ( 0.45, 0.5, 1, 1  )
	
	drawing:setJoinStyle ( MOAIVectorDrawing.JOIN_MITER )
	drawing:setCapStyle ( MOAIVectorDrawing.CAP_POINTY )
	drawing:setMiterLimit ( 10 )
	
	drawing:setLineWidth ( 50 )
	drawing:setStrokeStyle ( MOAIVectorDrawing.STROKE_EXTERIOR )

	drawing:pushRect ( -50, -50, 50, 50 )
	
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

layer:insertProp ( prop )
