--MOAI
serializer = ... or MOAISerializer.new ()

local function init ( objects )

	--MOAIGrid
	serializer:initInstance ( objects [ 0x024D8F98 ], {
		[ "mCellWidth" ] = 2,
		[ "mData" ] = "Y2BgaGAYxaN4FA8JnAok0oC4nAEM6GZvLJCIA+J8OtubDCRSBsC/0UAiho7+dQESyNgN",
		[ "mXOff" ] = 0,
		[ "mHeight" ] = 12,
		[ "mWidth" ] = 30,
		[ "mTileHeight" ] = 2,
		[ "mTileWidth" ] = 2,
		[ "mCellHeight" ] = 2,
		[ "mYOff" ] = 0,
	})

end

--Declaring Objects
local objects = {

	--Declaring Instances
	[ 0x024D8F98 ] = serializer:register ( MOAIGrid.new (), 0x024D8F98 ),

}

init ( objects )

--Returning Tables
return objects [ 0x024D8F98 ]
