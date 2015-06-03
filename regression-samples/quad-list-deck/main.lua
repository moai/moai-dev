----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- just for readability
local RECT_XMIN		= 1
local RECT_YMIN		= 2
local RECT_XMAX		= 3
local RECT_YMAX		= 4

local basename = 'r/worry_bear_run_left'
--basename = 'r/running_julius'
json = MOAIJsonParser.decode ( MOAIFileSystem.loadFile ( basename .. '.json' ))
assert ( json )

local CHUNKSIZE = json.chunkSize

local ANCHOR_X = json.anchor.x
local ANCHOR_Y = json.anchor.y

local uvRects		= {}
local screenRects	= {}
local materialIDs	= {}
local sprites		= {}

for i, tile in ipairs ( json.tiles ) do
	local cropRect = tile.cropRect
	local padRect = tile.paddingRect

	local cropRectWidth = cropRect [ RECT_XMAX ] - cropRect [ RECT_XMIN ]
	local cropRectHeight = cropRect [ RECT_YMAX ] - cropRect [ RECT_YMIN ]

	local uvRect = {
		[ RECT_XMIN ]		= cropRect [ RECT_XMIN ] / CHUNKSIZE,
		[ RECT_YMIN ]		= cropRect [ RECT_YMAX ] / CHUNKSIZE,
		[ RECT_XMAX ]		= cropRect [ RECT_XMAX ] / CHUNKSIZE,
		[ RECT_YMAX ]		= cropRect [ RECT_YMIN ] / CHUNKSIZE,
	}

	local screenRect = {
		[ RECT_XMIN ]		= padRect [ RECT_XMIN ] - ANCHOR_X,
		[ RECT_YMIN ]		= padRect [ RECT_YMIN ] - ANCHOR_Y,
		[ RECT_XMAX ]		= padRect [ RECT_XMIN ] - ANCHOR_X + cropRectWidth,
		[ RECT_YMAX ]		= padRect [ RECT_YMIN ] - ANCHOR_Y + cropRectHeight,
	}

	local material1 = math.floor ( uvRect [ RECT_XMIN ])
	local material2 = math.floor ( uvRect [ RECT_XMAX ])

	--print ( 'crop rect', i, cropRect [ RECT_XMIN ], cropRect [ RECT_YMIN ], cropRect [ RECT_XMAX ], cropRect [ RECT_YMAX ])
	--print ( ' - uv', i, uvRect [ RECT_XMIN ], uvRect [ RECT_YMIN ], uvRect [ RECT_XMAX ], uvRect [ RECT_YMAX ])

	if material1 == material2 then

		-- the rect does not straddle a material boundary, so
		-- just offset it by its material ID (in uv space) and record it

		table.insert ( sprites, {
			base	= #materialIDs + 1,
			size	= 1,
		})

		table.insert ( uvRects, {
			[ RECT_XMIN ]		= uvRect [ RECT_XMIN ] - material1,
			[ RECT_YMIN ]		= uvRect [ RECT_YMIN ],
			[ RECT_XMAX ]		= uvRect [ RECT_XMAX ] - material1,
			[ RECT_YMAX ]		= uvRect [ RECT_YMAX ],
		})

		table.insert ( screenRects, screenRect )
		table.insert ( materialIDs, material1 + 1 )

		local screenRect2 = screenRect
		
		local uvRect2 = {
			[ RECT_XMIN ]		= uvRect [ RECT_XMIN ] - material1,
			[ RECT_YMIN ]		= uvRect [ RECT_YMIN ],
			[ RECT_XMAX ]		= uvRect [ RECT_XMAX ] - material1,
			[ RECT_YMAX ]		= uvRect [ RECT_YMAX ],
		}
		print ( ' - uv', i, uvRect2 [ RECT_XMIN ], uvRect2 [ RECT_YMIN ], uvRect2 [ RECT_XMAX ], uvRect2 [ RECT_YMAX ])
		print ( ' NEW - screenRect', i, screenRect2 [ RECT_XMIN ], screenRect2 [ RECT_YMIN ], screenRect2 [ RECT_XMAX ], screenRect2 [ RECT_YMAX ])
		print("Material", material1 + 1)

	else

		-- the rect does straddle a material boundary, so split it in two

		-- sorry, we're not handling super-big sprites at the moment
		assert ( material2 == ( material1 + 1 ))

		table.insert ( sprites, {
			base	= #materialIDs + 1,
			size	= 2,
		})

		local split = (( material2 - uvRect [ RECT_XMIN ]) / ( uvRect [ RECT_XMAX ] - uvRect [ RECT_XMIN ])) * cropRectWidth

		table.insert ( uvRects, {
			[ RECT_XMIN ]		= uvRect [ RECT_XMIN ] - material1,
			[ RECT_YMIN ]		= uvRect [ RECT_YMIN ],
			[ RECT_XMAX ]		= 1,
			[ RECT_YMAX ]		= uvRect [ RECT_YMAX ],
		})

		local uvRect2 = {
			[ RECT_XMIN ]		= uvRect [ RECT_XMIN ] - material1,
			[ RECT_YMIN ]		= uvRect [ RECT_YMIN ],
			[ RECT_XMAX ]		= 1,
			[ RECT_YMAX ]		= uvRect [ RECT_YMAX ],
		}
		print ( ' - uv', i, uvRect2 [ RECT_XMIN ], uvRect2 [ RECT_YMIN ], uvRect2 [ RECT_XMAX ], uvRect2 [ RECT_YMAX ])

		table.insert ( uvRects, {
			[ RECT_XMIN ]		= 0,
			[ RECT_YMIN ]		= uvRect [ RECT_YMIN ],
			[ RECT_XMAX ]		= uvRect [ RECT_XMAX ] - material2,
			[ RECT_YMAX ]		= uvRect [ RECT_YMAX ],
		})
		uvRect2 = {
			[ RECT_XMIN ]		= 0,
			[ RECT_YMIN ]		= uvRect [ RECT_YMIN ],
			[ RECT_XMAX ]		= uvRect [ RECT_XMAX ] - material2,
			[ RECT_YMAX ]		= uvRect [ RECT_YMAX ],
		}
		print ( ' - uv', i, uvRect2 [ RECT_XMIN ], uvRect2 [ RECT_YMIN ], uvRect2 [ RECT_XMAX ], uvRect2 [ RECT_YMAX ])
		table.insert ( screenRects, {
			[ RECT_XMIN ]		= screenRect [ RECT_XMIN ],
			[ RECT_YMIN ]		= screenRect [ RECT_YMIN ],
			[ RECT_XMAX ]		= screenRect [ RECT_XMIN ] + split,
			[ RECT_YMAX ]		= screenRect [ RECT_YMAX ],
		})
		local screenRect2 = {
			[ RECT_XMIN ]		= screenRect [ RECT_XMIN ],
			[ RECT_YMIN ]		= screenRect [ RECT_YMIN ],
			[ RECT_XMAX ]		= screenRect [ RECT_XMIN ] + split,
			[ RECT_YMAX ]		= screenRect [ RECT_YMAX ],
		}
		print ( ' NEW - screenRect', i, screenRect2 [ RECT_XMIN ], screenRect2 [ RECT_YMIN ], screenRect2 [ RECT_XMAX ], screenRect2 [ RECT_YMAX ])

		table.insert ( screenRects, {
			[ RECT_XMIN ]		= screenRect [ RECT_XMAX ] - ( cropRectWidth - split ),
			[ RECT_YMIN ]		= screenRect [ RECT_YMIN ],
			[ RECT_XMAX ]		= screenRect [ RECT_XMAX ],
			[ RECT_YMAX ]		= screenRect [ RECT_YMAX ],
		})

		screenRect2 = {
			[ RECT_XMIN ]		= screenRect [ RECT_XMAX ] - ( cropRectWidth - split ),
			[ RECT_YMIN ]		= screenRect [ RECT_YMIN ],
			[ RECT_XMAX ]		= screenRect [ RECT_XMAX ],
			[ RECT_YMAX ]		= screenRect [ RECT_YMAX ],
		}
		print ( ' NEW - screenRect', i, screenRect2 [ RECT_XMIN ], screenRect2 [ RECT_YMIN ], screenRect2 [ RECT_XMAX ], screenRect2 [ RECT_YMAX ])

		table.insert ( materialIDs, material1 + 1 )
		table.insert ( materialIDs, material2 + 1 )

		print("Material", material1 + 1)
		print("Material", material2 + 1)
	end
end

gfxQuadListDeck = MOAIGfxQuadListDeck2D.new ()

-- we're going to init the textures with filenames to enable reloading and
-- prevent caching of image data
gfxQuadListDeck:reserveMaterials ( 2 )

local loadAsBitmap = function ( filename )

	local image = MOAIImage.new ()
	image:load ( filename )
	image:resize ( 128, 128 )
	image:simpleThreshold ( 1, 1, 1, 0.5 )
	image:convert ( MOAIImage.COLOR_FMT_A_1 )
	return image
end


local maxCols = math.ceil(json.width / CHUNKSIZE)
local maxRows = math.ceil(json.height / CHUNKSIZE)

local fileCount = maxCols * maxRows

local extension = ".png"
-- reload the images one by one and generate bitmaps from their alpha
-- ideally, these will be pre-generated and stored alongside the png's
for i=1, fileCount do
	local zeroBasedIdx = i - 1
	local imageFilename = basename .. "." .. zeroBasedIdx .. extension
	-- Make sure the file exists!  If it does not, MOAITexture.load fails silently.
	assert(MOAIFileSystem.checkFileExists(imageFilename), "Texture._loadChunk: missing file '" .. imageFilename .. "'")
	gfxQuadListDeck:setTexture ( i, imageFilename )
	gfxQuadListDeck:setHitMask ( i, loadAsBitmap ( imageFilename ))
	gfxQuadListDeck:setHitMaskThreshold ( i, 0, 0, 0, 1 )
end

local totalRects	= #materialIDs

gfxQuadListDeck:reserveUVQuads ( totalRects)
gfxQuadListDeck:reserveQuads ( totalRects )
gfxQuadListDeck:reservePairs ( totalRects )

for i = 1, totalRects do

	local uvRect = uvRects [ i ]
	local screenRect = screenRects [ i ]

	print ( 'uv rect', i, uvRect [ RECT_XMIN ], uvRect [ RECT_YMIN ], uvRect [ RECT_XMAX ], uvRect [ RECT_YMAX ])
	print ( 'screen rect', i, " --", screenRect [ RECT_XMIN ], screenRect [ RECT_YMIN ], screenRect [ RECT_XMAX ], screenRect [ RECT_YMAX ])

	gfxQuadListDeck:setUVRect ( i,
		uvRect [ RECT_XMIN ],
		uvRect [ RECT_YMIN ],
		uvRect [ RECT_XMAX ],
		uvRect [ RECT_YMAX ]
	)

	gfxQuadListDeck:setRect ( i,
		screenRect [ RECT_XMIN ],
		screenRect [ RECT_YMIN ],
		screenRect [ RECT_XMAX ],
		screenRect [ RECT_YMAX ]
	)

	gfxQuadListDeck:setPair ( i, i, i, materialIDs [ i ])
end

gfxQuadListDeck:reserveLists ( #sprites )
for i, sprite in ipairs ( sprites ) do
	gfxQuadListDeck:setList ( i, sprite.base, sprite.size )
end

----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuadListDeck )
prop:setHitGranularity ( MOAIProp.HIT_TEST_FINE )
layer:insertProp ( prop )

index = 1

label = MOAITextLabel.new ()

function onMouseEvent ( down, delta )

	if down == true then

		local x, y = MOAIInputMgr.device.pointer:getLoc ()
		x, y = layer:wndToWorld ( x, y )
		-- if prop:inside ( x, y ) then

			if delta > 0 then
				index = ( index % #sprites ) + 1
			elseif delta < 0 then
				index = index - 1
				index = index <= 0 and #sprites or index
			end

			prop:setIndex ( index )

			label:setString ( tostring(index) )

		-- end
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( function ( down ) onMouseEvent ( down, 1 ) end )
MOAIInputMgr.device.mouseRight:setCallback ( function ( down ) onMouseEvent ( down, -1 ) end )

font = MOAIFont.new ()
font:loadFromTTF ( 'r/arial-rounded.TTF' )


label:setString ( '' )
label:setFont ( font )
label:setTextSize ( 32 )
label:setYFlip ( true )
label:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.BASELINE_JUSTIFY )
label:setLoc ( 0, -220 )
layer:insertProp ( label )
