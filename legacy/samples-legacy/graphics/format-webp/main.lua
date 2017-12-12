MOAISim.openWindow ( "test", 640, 480 )
local viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, -480 )

local layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAIRenderMgr.setRenderTable ({ layer })

local deck = MOAIGfxQuad2D.new ()
deck:setRect ( -320, -240, 320, 240 )
deck:setUVRect ( 0, 0, 1, 1 )

local prop = MOAIProp2D.new ()
prop:setDeck ( deck )
layer:insertProp ( prop )

MOAIGfxDevice.setClearColor ( 0.9, 0.8, 1.0 )

function main()
	local image = MOAIImage.new()
	
	local loadTimes = {}
	
	for fileNumber, fileName in ipairs { "dice", "butterfly" } do
		for _, pot in ipairs { false, true } do
			for _, quantize in ipairs { false, true } do
				for _, premultiply in ipairs { false, true } do
					local transforms = (pot and MOAIImage.POW_TWO or 0)
						+ (quantize and MOAIImage.QUANTIZE or 0)
						+ (premultiply and MOAIImage.PREMULTIPLY_ALPHA or 0)
					for _, extension in pairs { fileNumber == 1 and "png" or "jpg", "webp" } do
						local description = string.format("%s pot=%s quantize=%s premultiply=%s input=%s",
							fileName, pot, quantize, premultiply, extension)
						print(description)
						
						-- Set texture
						local inputFileName = string.format("%s.%s", fileName, extension)
						deck:setTexture(inputFileName, transforms)
						
						-- Test performance
						local start = os.clock()
						local image = MOAIImage.new()
						local iterations = 10
						for i = 1, iterations do
							image:load(inputFileName, transforms)
						end
						local duration = (os.clock() - start) / iterations
						loadTimes[description] = duration
						
						-- Save snapshot
						local filePath = string.format("output/%s.png", description)
						local continue = false
						local frameBuffer = MOAIGfxDevice.getFrameBuffer()
						frameBuffer:grabNextFrame(image, function() image:writePNG(filePath); continue = true; end)
						repeat coroutine.yield() until continue
					end
				end
			end
		end
	end
	
	for description, duration in pairs(loadTimes) do
		print(string.format("%ims to load %s", duration * 1000, description))
	end
end

MOAICoroutine.new():run(main)