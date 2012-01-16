--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

local function initTransform2DInterface ( interface, super )

	function interface.addRot ( self, rDelta )
		super.addRot ( self, 0, 0, rDelta )
	end

	function interface.getRot ( self )
		local x, y, z = super.getRot ( self )
		return z
	end

	function interface.move ( self, xDelta, yDelta, rDelta, xSclDelta, ySclDelta, length, mode )
		super.move ( self, xDelta, yDelta, 0, 0, 0, rDelta, xSclDelta, ySclDelta, 0, length, mode )
	end

	function interface.moveLoc ( self, xDelta, yDelta, length, mode )
		super.moveLoc ( self, xDelta, yDelta, 0, length, mode )
	end

	function interface.movePiv ( self, xDelta, yDelta, length, mode )
		super.movePiv ( self, xDelta, yDelta, 0, length, mode )
	end
	
	function interface.moveRot ( self, rDelta, length, mode )
		super.moveRot ( self, 0, 0, rDelta, length, mode )
	end
	
	function interface.moveScl ( self, xSclDelta, ySclDelta, length, mode )
		super.moveScl ( self, xSclDelta, ySclDelta, 0, length, mode )
	end
	
	function interface.seek ( self, xGoal, yGoal, rGoal, xSclGoal, ySclGoal, length, mode )
		super.seek ( self, xGoal, yGoal, 0, 0, 0, rGoal, xSclGoal, ySclGoal, 1, length, mode )
	end

	function interface.seekLoc ( self, xGoal, yGoal, length, mode )
		super.seekLoc ( self, xGoal, yGoal, 0, length, mode )
	end

	function interface.seekPiv ( self, xGoal, yGoal, length, mode )
		super.seekPiv ( self, xGoal, yGoal, 0, length, mode )
	end
	
	function interface.seekRot ( self, rGoal, length, mode )
		super.seekRot ( self, 0, 0, rGoal, length, mode )
	end
	
	function interface.seekScl ( self, xSclGoal, ySclGoal, length, mode )
		super.seekScl ( self, xSclGoal, ySclGoal, 0, length, mode )
	end
	
	function interface.setRot ( self, rot )
		super.setRot ( self, 0, 0, rot )
	end
	
end

----------------------------------------------------------------
-- moai2D.lua - version 1.0 Beta
----------------------------------------------------------------

MOAIThread = MOAICoroutine

MOAILayer2D = MOAILayer.extend (

	-- extend the instance interface
	function ( interface, super )
	end,
	
	-- extend the class
	function ( class, super )

		function class.new ()
			local self = super.new ()
			self:setPartitionCull2D ( true )
			return self
		end
	end
)

MOAITransform2D = MOAITransform.extend (

	-- extend the instance interface
	function ( interface, super )
		initTransform2DInterface ( interface, super )
	end,
	
	-- extend the class
	function ( class, super )
	end
)

MOAIProp2D = MOAIProp.extend (

	-- extend the instance interface
	function ( interface, super )
		initTransform2DInterface ( interface, super )
	end,
	
	-- extend the class
	function ( class, super )
	end
)
