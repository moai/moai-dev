
function justWords(str, maxNb)

	delim = "%s"
    if string.find(str, delim) == nil then
        return { str }
    end
    if maxNb == nil or maxNb < 1 then
        maxNb = 0    -- No limit
    end
    local result = {}
    local pat = "(.-)" .. delim .. "()"
    local nb = 0
    local lastPos
    for part, pos in string.gfind(str, pat) do
        nb = nb + 1
        result[nb] = part
        lastPos = pos
        if nb == maxNb then break end
    end

    if nb ~= maxNb then
        result[nb + 1] = string.sub(str, lastPos)
    end
    return result
end

function colladaMatchInArray ( elementArray, _id )
	local found = nil
	local id = _id:sub(2)
	for v=1, #elementArray, 1 do 
		if id == elementArray[v].attributes["id"] then
			found = elementArray[v]
		end
	end
	return found
end

--load model
--In ( collada file ) -> Out ( MOAIMesh )
function colladaToMesh( colladafile, textureFile )

	--load vertex data
	local xml = MOAIXmlParser.parseFile ( colladafile )
	local xmlMesh = xml.children["library_geometries"][1].children["geometry" ][1].children["mesh"][1]

	--TODO Add support for more triangle arrays?
	local xmlTriangles = xmlMesh.children["triangles"][1]
	
	local xmlVerticesArray = xmlMesh.children["vertices"]
	local xmlSourcesArray = xmlMesh.children["source"]
	
	local vertexData = {}
	vertexData.positionData = {}
	vertexData.normalData = {}
	vertexData.colorData = {}
	vertexData.texCoordData = {}

	--get vertex data
	for i=1,#xmlTriangles.children["input"],1 do
	
		local xmlVertexFormatInput = xmlTriangles.children["input"][i]
		
		local semantic = xmlVertexFormatInput.attributes["semantic"]
		local sourceId = xmlVertexFormatInput.attributes["source"]
		
		local offset = tonumber ( xmlVertexFormatInput.attributes["offset"] )
		
		if semantic == "VERTEX" then
		
			local xmlVerticesElement = colladaMatchInArray ( xmlVerticesArray, sourceId )
			
			--TODO support more elements in vertices array (get rid of the 1)
			xmlVertexFormatInput = xmlVerticesElement.children["input"][1]
		
			semantic = xmlVertexFormatInput.attributes["semantic"]
			sourceId = xmlVertexFormatInput.attributes["source"]
		end
		
		--get source element
		local xmlSourceElement = colladaMatchInArray ( xmlSourcesArray, sourceId )
		
		--determine info about data
		local xmlDataArrayInfo = xmlSourceElement.children["technique_common"][1].children["accessor"][1]
		
		local numVerts = tonumber ( xmlDataArrayInfo.attributes["count"] )
		local numStride = tonumber ( xmlDataArrayInfo.attributes["stride"] )
		local arraySourceId = xmlDataArrayInfo.attributes["source"]
		local xmlFloatArray = colladaMatchInArray ( xmlSourceElement.children["float_array"], arraySourceId )
		
		local floatArray = justWords ( xmlFloatArray.value )
		
		local finalData
		
		if semantic == "NORMAL" then
			finalData = vertexData.normalData
		end
		
		if semantic == "POSITION" then
			finalData = vertexData.positionData
		end
		
		if semantic == "TEXCOORD" then
			finalData = vertexData.texCoordData
		end
		
		for dataIndex=1,numVerts,1 do
			finalData[dataIndex] = {}
			for elementIndex=1,numStride,1 do
				finalData[dataIndex][elementIndex] = tonumber ( floatArray[ (dataIndex-1)*numStride + elementIndex ] )
			end
		end
			
	end
	
	--get indices
	local numTriangles = tonumber ( xmlTriangles.attributes["count"] )
	local triangleText = xmlTriangles.children["p"][1].value
	local triangleIndices = justWords(triangleText)

	--TODO deindexing would happen here
	
	local vertexFormat = MOAIVertexFormat.new ()

	vertexFormat:declareCoord ( 1, MOAIVertexFormat.GL_FLOAT, 3 )
	vertexFormat:declareUV ( 2, MOAIVertexFormat.GL_FLOAT, 2 )
	vertexFormat:declareColor ( 3, MOAIVertexFormat.GL_UNSIGNED_BYTE )

	local vbo = MOAIVertexBuffer.new ()
	vbo:setFormat ( vertexFormat )
	vbo:reserveVerts ( #vertexData.positionData )
	
	--verts
	--print ( "Writing " .. #vertexData.positionData .. " vertices")
	for i=1,#vertexData.positionData,1 do
		if vertexData.positionData[i] ~= nil then
			--print ( vertexData.positionData[i][1] .. " " .. vertexData.positionData[i][2] .. " " .. vertexData.positionData[i][3] )
			vbo:writeFloat ( vertexData.positionData[i][1], vertexData.positionData[i][2], vertexData.positionData[i][3] )
		else
			--print ( "WARNING cannot read vertex data" )
			vbo:writeFloat ( 0, 0, 0 )
		end
		
		if vertexData.texCoordData[i] ~= nil then
			vbo:writeFloat ( vertexData.texCoordData[i][1], vertexData.texCoordData[i][2] )
		else
			vbo:writeFloat ( 0, 0 )
		end
		
		vbo:writeColor32 ( 1, 1, 1 )
	end
	
	vbo:bless ()

	local ibo = MOAIIndexBuffer.new ()
	ibo:reserve ( #triangleIndices )

	-- indices
	--print ( "Writing " .. #triangleIndices .. " indices")
	for i=1,#triangleIndices,1 do
		--print ( i .. " " .. tonumber(triangleIndices[i]) + 1 )
		ibo:setIndex ( i, tonumber(triangleIndices[i]) + 1 )
	end

	local mesh = MOAIMesh.new ()
	mesh:setTexture ( textureFile )
	mesh:setVertexBuffer ( vbo )
	mesh:setIndexBuffer ( ibo )
	mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
	
	return mesh
end

--debug functions
function printNode ( node, indent )

	indent = indent or ""
	
	if node.type then
		print ( indent .. "type: " .. node.type )
	end
	
	if node.children then  
		print ( indent .. "children" )
		printChildren ( node.children, indent .. "  " )
	end
	
end

function printElement ( element, indent )

	for key, value in ipairs ( element ) do
		
		print ( indent .. key .. ":" )
	
		if type ( value ) == "table" then
			printNode ( value, indent .. "    " )
		else
			print ( indent .. "    " .. value )
		end
	end
end

function printChildren ( children, indent )

	if children == nil then return end
	indent = indent or ""

	for key, value in pairs ( children ) do
		print ( indent .. "KEY:"..key .. ":" )
		printElement ( value, indent .. "  " )
	end
end

function printTableKeys ( children )

	if children == nil then return end

	for key, value in pairs ( children ) do
		print ( "KEY:"..key )--.. " VALUE:" .. value )
	end
end
