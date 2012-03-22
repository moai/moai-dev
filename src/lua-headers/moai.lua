--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

local function initTransform2DInterface ( interface, super )

	function interface.addRot ( self, rDelta )
		super.addRot ( self, 0, 0, rDelta )
	end
	
	function interface.getLoc ( self )
		local x, y, z = super.getLoc ( self )
		return x, y
	end
	
	function interface.getRot ( self )
		local x, y, z = super.getRot ( self )
		return z
	end

	function interface.move ( self, xDelta, yDelta, rDelta, xSclDelta, ySclDelta, length, mode )
		return super.move ( self, xDelta, yDelta, 0, 0, 0, rDelta, xSclDelta, ySclDelta, 0, length, mode )
	end

	function interface.moveLoc ( self, xDelta, yDelta, length, mode )
		return super.moveLoc ( self, xDelta, yDelta, 0, length, mode )
	end

	function interface.movePiv ( self, xDelta, yDelta, length, mode )
		return super.movePiv ( self, xDelta, yDelta, 0, length, mode )
	end
	
	function interface.moveRot ( self, rDelta, length, mode )
		return super.moveRot ( self, 0, 0, rDelta, length, mode )
	end
	
	function interface.moveScl ( self, xSclDelta, ySclDelta, length, mode )
		return super.moveScl ( self, xSclDelta, ySclDelta, 0, length, mode )
	end
	
	function interface.seek ( self, xGoal, yGoal, rGoal, xSclGoal, ySclGoal, length, mode )
		return super.seek ( self, xGoal, yGoal, 0, 0, 0, rGoal, xSclGoal, ySclGoal, 1, length, mode )
	end

	function interface.seekLoc ( self, xGoal, yGoal, length, mode )
		return super.seekLoc ( self, xGoal, yGoal, 0, length, mode )
	end

	function interface.seekPiv ( self, xGoal, yGoal, length, mode )
		return super.seekPiv ( self, xGoal, yGoal, 0, length, mode )
	end
	
	function interface.seekRot ( self, rGoal, length, mode )
		return super.seekRot ( self, 0, 0, rGoal, length, mode )
	end
	
	function interface.seekScl ( self, xSclGoal, ySclGoal, length, mode )
		return super.seekScl ( self, xSclGoal, ySclGoal, 1, length, mode )
	end
	
	function interface.setLoc ( self, x, y )
		super.setLoc ( self, x, y, 0 )
	end
	
	function interface.setRot ( self, rot )
		super.setRot ( self, 0, 0, rot )
	end
	
end

----------------------------------------------------------------
-- moai2D.lua - version 1.0 Beta
----------------------------------------------------------------

MOAIThread = MOAICoroutine
MOAILayerBridge2D = MOAILayerBridge

MOAICamera2D = MOAICamera.extend (

	-- extend the instance interface
	function ( interface, super )
		initTransform2DInterface ( interface, super )
	end,

	-- extend the class
	function ( class, super )

		local new = class.new

		function class.new ()
			local self = new ()
			self:setOrtho ( true )
			self:setNearPlane ( 1 )
			self:setFarPlane ( -1 )
			return self
		end
	end
)

if MOAIFreeTypeFontReader then

	-- extend the instance interface
	MOAIFont = MOAIFont.extend (
	
		function ( interface, super )
		end,
		
		-- extend the class
		function ( class, super )
		
			function class.new ()
				local self = super.new ()
				self:setCache ( MOAIGlyphCache.new ())
				self:setReader ( MOAIFreeTypeFontReader.new ())
				
				function self.loadFromTTF ( self, filename, charcodes, points, dpi )
					self:load ( filename )
					self:preloadGlyphs ( charcodes, points, dpi )
				end
				
				return self
			end
		end
	)
end

MOAIHttpTask = MOAIHttpTaskCurl or MOAIHttpNaCl

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

MOAIProp2D = MOAIProp.extend (

	-- extend the instance interface
	function ( interface, super )
		initTransform2DInterface ( interface, super )
		
		function interface.setFrame ( self, xMin, yMin, xMax, yMax )
			super.setFrame ( self, xMin, yMin, 0, xMax, yMax, 0 )
		end
		
		function interface.getRect ( self )			
			print ( self )
			local xMin, yMin, zMin, xMax, yMax, zMax = super.getBounds ( self )
			return xMin, yMin, xMax, yMax
		end
	end,
	
	-- extend the class
	function ( class, super )
	end
)

MOAIRenderMgr = MOAIRenderMgr.extend (
	
	-- extend the class
	function ( class, super )
	
		function class.affirmRenderTable ()

			local renderTable = class.getRenderTable ()
			if not renderTable then
				renderTable = {}
				class.setRenderTable ( renderTable )
			end
			
			return renderTable
		end

		function class.clearRenderStack ()
			
			--local renderTable = class.affirmRenderTable ()
			--renderTable [ 1 ] = nil
			class.setRenderTable ( nil )
		end
		
		function class.popRenderPass ()

			local renderTable = class.affirmRenderTable ()
			table.remove ( renderTable )
		end
		
		function class.pushRenderPass ( pass )

			local renderTable = class.affirmRenderTable ()
			table.insert ( renderTable, pass )
		end
		
		function class.removeRenderPass ( pass )

			local renderTable = class.affirmRenderTable ()
			for i, cursor in ipairs ( renderTable ) do
				if cursor == pass then
					table.remove ( renderTable, i )
					break
				end
			end
		end
	end
)

MOAISim = MOAISim.extend (
	
	-- extend the class
	function ( class, super )

		function class.clearRenderStack ()
			MOAIRenderMgr.clearRenderStack ()
		end
		
		function class.popRenderPass ()
			MOAIRenderMgr.popRenderPass ()
		end
		
		function class.pushRenderPass ( pass )
			MOAIRenderMgr.pushRenderPass ( pass )
		end
		
		function class.removeRenderPass ( pass )
			MOAIRenderMgr.removeRenderPass ( pass )
		end
	end
)

MOAITextBox = MOAITextBox.extend (

	-- extend the instance interface
	function ( interface, super )
		
		function interface.affirmStyle ( self )
			local style = self:getStyle ()
			if not style then
				style = MOAITextStyle.new ()
				self:setStyle ( style )
			end
			return style
		end
		
		function interface.setFont ( self, font )
			local style = self:affirmStyle ()
			style:setFont ( font )
		end
		
		function interface.setTextSize ( self, points, dpi )
			local style = self:affirmStyle ()
			style:setSize ( points, dpi )
		end
	end,
	
	-- extend the class
	function ( class, super )
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
