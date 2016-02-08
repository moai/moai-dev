--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

MOAISim.openWindow ( "test", 640, 480 )
MOAIGfxMgr.setClearColor ( 1, 1, 1, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

makeRegion = function ( x, y, r )

	local hr = r / 2

	local tess = MOAIVectorTesselator.new ()

	tess:pushCombo ()
		tess:pushTranslate ( x, y )
		tess:pushPoly ( r, -r, -r, -r, -r, r, r, r )
	tess:finish ()

	local region = MOAIRegion.new ()
	tess:tesselate ( region )

	return region
end

mainRegion = makeRegion ( 0, 0, 100 )

strokeRegion = MOAIRegion.new ()
strokeRegion:stroke ( mainRegion, 50, 55 )

region = MOAIRegion.new ()
region:boolean ( mainRegion, strokeRegion, MOAIRegion.BOOLEAN_OR )

local vtxStream = MOAIMemStream.new ()
local idxStream = MOAIMemStream.new ()

local vtxFormat = MOAIVertexFormatMgr.getFormat ( MOAIVertexFormatMgr.XYZC )

region:getTriangles ( vtxStream, idxStream, vtxFormat )

vtxStream:seek ( 0 )
idxStream:seek ( 0 )

MOAIGeometryWriter.snapCoords ( vtxFormat, vtxStream, 0.1 )
MOAIGeometryWriter.applyColor ( vtxFormat, vtxStream, 0, 0, 0, 1, MOAIGeometryWriter.COLOR_OVERWRITE )
MOAIGeometryWriter.applyColor ( vtxFormat, vtxStream, 1, 0, 0, 1, mainRegion, 0, MOAIGeometryWriter.COLOR_OVERWRITE )
MOAIGeometryWriter.pruneVertices ( vtxFormat, vtxStream, idxStream )

local mesh = MOAIGeometryWriter.getMesh ( vtxFormat, vtxStream, idxStream )
mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))

local prop = MOAIProp.new ()
prop:setDeck ( mesh )
layer:insertProp ( prop )

--prop:setColor ( 1, 0, 0, 1 )

function clickCallback ( down )
	
	if down then
		
		local x, y = MOAIInputMgr.device.pointer:getLoc ()
		x, y = prop:worldToModel ( layer:wndToWorld ( x, y ))
		if region:pointInside ( x, y ) then
			prop:addRot ( 0, 0, 5 )
		end
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )
