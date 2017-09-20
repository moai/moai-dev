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
		--tess:pushPoly ( hr, -hr, -hr, -hr, -hr, hr, hr, hr )
	tess:finish ()

	local region = MOAIRegion.new ()
	tess:tesselate ( region )

	return region
end

region = makeRegion ( 0, 0, 100 )

strokeRegion = MOAIRegion.new ()
strokeRegion:stroke ( region, 50, 55 )

region:boolean ( strokeRegion, region, MOAIRegion.BOOLEAN_OR )

local vtxFormat = MOAIVertexFormatMgr.getFormat ( MOAIVertexFormatMgr.XYZC )

local vtxBuffer = MOAIVertexBuffer.new ()
local idxBuffer = MOAIIndexBuffer.new ()

local totalElements = region:getTriangles ( vtxBuffer, idxBuffer, 2, vtxFormat );

local mesh = MOAIMesh.new ()
mesh:setVertexBuffer ( vtxBuffer, vtxFormat )
mesh:setIndexBuffer ( idxBuffer )
mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))
mesh:setTotalElements ( totalElements )
mesh:setBounds ( vtxBuffer:computeBounds ( vtxFormat ))

local prop = MOAIProp.new ()
prop:setDeck ( mesh )
layer:insertProp ( prop )

prop:setColor ( 1, 0, 0, 1 )

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
