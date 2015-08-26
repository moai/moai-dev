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

local vtxStream = MOAIMemStream.new ()
local idxStream = MOAIMemStream.new ()

local vtxFormat = MOAIVertexFormatMgr.getFormat ( MOAIVertexFormatMgr.XYZC )

tess = MOAIVectorTesselator.new ()

tess:setFillStyle ( MOAIVectorTesselator.FILL_SOLID )
tess:setFillColor ( 0.6, 0.75, 1.0, 1.0 )
tess:setCircleResolution ( 4 )

local sizes = {}

local function pushCircle ( x, y, r )

	tess:pushEllipse ( x, y, r )
	local n = tess:tesselate ( vtxStream, idxStream, vtxFormat );
	tess:clearShapes ()

	table.insert ( sizes, n )
end

pushCircle ( -100, -100, 100 )
pushCircle ( 100, -100, 100 )
pushCircle ( 100, 100, 100 )
pushCircle ( -100, 100, 100 )

vtxStream:seek ()
idxStream:seek ()

local mesh = MOAIGeometryWriter.getMesh ( vtxFormat, vtxStream, idxStream )
mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))

local selectionMesh = MOAISelectionMesh.new ()
selectionMesh:setMesh ( mesh )

selectionMesh:reserveSelections ( #sizes )

local base = 1
for i, n in ipairs ( sizes ) do
	local top = base + n
	print ( base, top )
	selectionMesh:addSelection ( i, base, top )
	base = top
end

selectionMesh:printSelection ()

local prop = MOAIProp.new ()
prop:setDeck ( selectionMesh )
layer:insertProp ( prop )

index = 1

function onMouseEvent ( down, delta )

	if down == true then

		if delta > 0 then
			index = ( index % #sizes ) + 1
		elseif delta < 0 then
			index = index - 1
			index = index <= 0 and #sizes or index
		end

		prop:setIndex ( index )
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( function ( down ) onMouseEvent ( down, 1 ) end )
MOAIInputMgr.device.mouseRight:setCallback ( function ( down ) onMouseEvent ( down, -1 ) end )
