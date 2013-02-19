--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

--============================================================--
-- initTransform2DInterface
--============================================================--
local function initTransform2DInterface ( interface, superInterface )

	----------------------------------------------------------------
	function interface.addLoc ( self, xDelta, yDelta )
		superInterface.addLoc ( self, xDelta, yDelta, 0 )
	end

	----------------------------------------------------------------
	function interface.addPiv ( self, xDelta, yDelta )
		superInterface.addPiv ( self, xDelta, yDelta, 0 )
	end
	
	----------------------------------------------------------------
	function interface.addRot ( self, rDelta )
		superInterface.addRot ( self, 0, 0, rDelta )
	end
	
	----------------------------------------------------------------
	function interface.addScl ( self, xSclDelta, ySclDelta )
		superInterface.addScl ( self, xSclDelta, ySclDelta, 0 )
	end
	
	----------------------------------------------------------------
	function interface.getLoc ( self )
		local x, y = superInterface.getLoc ( self )
		return x, y
	end
	
	----------------------------------------------------------------
	function interface.getPiv ( self )
		local x, y = superInterface.getPiv ( self )
		return x, y
	end
	
	----------------------------------------------------------------
	function interface.getRot ( self )
		local x, y, z = superInterface.getRot ( self )
		return z
	end

	----------------------------------------------------------------
	function interface.getScl ( self )
		local x, y = superInterface.getScl ( self )
		return x, y
	end
	
	----------------------------------------------------------------
	function interface.modelToWorld ( self, x, y )
		local x, y = superInterface.modelToWorld ( self, x, y, 0 )
		return x, y
	end
	
	----------------------------------------------------------------
	function interface.move ( self, xDelta, yDelta, rDelta, xSclDelta, ySclDelta, length, mode )
		return superInterface.move ( self, xDelta, yDelta, 0, 0, 0, rDelta, xSclDelta, ySclDelta, 0, length, mode )
	end

	----------------------------------------------------------------
	function interface.moveLoc ( self, xDelta, yDelta, length, mode )
		return superInterface.moveLoc ( self, xDelta, yDelta, 0, length, mode )
	end

	----------------------------------------------------------------
	function interface.movePiv ( self, xDelta, yDelta, length, mode )
		return superInterface.movePiv ( self, xDelta, yDelta, 0, length, mode )
	end
	
	----------------------------------------------------------------
	function interface.moveRot ( self, rDelta, length, mode )
		return superInterface.moveRot ( self, 0, 0, rDelta, length, mode )
	end
	
	----------------------------------------------------------------
	function interface.moveScl ( self, xSclDelta, ySclDelta, length, mode )
		return superInterface.moveScl ( self, xSclDelta, ySclDelta, 0, length, mode )
	end
	
	----------------------------------------------------------------
	function interface.seek ( self, xGoal, yGoal, rGoal, xSclGoal, ySclGoal, length, mode )
		return superInterface.seek ( self, xGoal, yGoal, 0, 0, 0, rGoal, xSclGoal, ySclGoal, 1, length, mode )
	end

	----------------------------------------------------------------
	function interface.seekLoc ( self, xGoal, yGoal, length, mode )
		return superInterface.seekLoc ( self, xGoal, yGoal, 0, length, mode )
	end

	----------------------------------------------------------------
	function interface.seekPiv ( self, xGoal, yGoal, length, mode )
		return superInterface.seekPiv ( self, xGoal, yGoal, 0, length, mode )
	end
	
	----------------------------------------------------------------
	function interface.seekRot ( self, rGoal, length, mode )
		return superInterface.seekRot ( self, 0, 0, rGoal, length, mode )
	end
	
	----------------------------------------------------------------
	function interface.seekScl ( self, xSclGoal, ySclGoal, length, mode )
		return superInterface.seekScl ( self, xSclGoal, ySclGoal, 1, length, mode )
	end
	
	----------------------------------------------------------------
	function interface.setLoc ( self, x, y )
		superInterface.setLoc ( self, x, y, 0 )
	end
	
	----------------------------------------------------------------
	function interface.setPiv ( self, x, y )
		superInterface.setPiv ( self, x, y, 0 )
	end
	
	----------------------------------------------------------------
	function interface.setRot ( self, rot )
		superInterface.setRot ( self, 0, 0, rot )
	end
	
	----------------------------------------------------------------
	function interface.setScl ( self, x, y )
		superInterface.setScl ( self, x, y, 1 )
	end
	
	----------------------------------------------------------------
	function interface.worldToModel ( self, x, y )
		local x, y = superInterface.worldToModel ( self, x, y, 0 )
		return x, y
	end
	
end

--============================================================--
-- moai2D.lua - version 1.0 Beta
--============================================================--

--============================================================--
-- MOAIThread
--============================================================--
MOAIThread = MOAICoroutine

--============================================================--
-- MOAILayerBridge2D
--============================================================--
MOAILayerBridge2D = MOAILayerBridge

--============================================================--
-- MOAICamera
--============================================================--
MOAICamera.extend (

	'MOAICamera2D',
	
	----------------------------------------------------------------
	function ( interface, class, superInterface, superClass )
		
		-- extend the interface
		initTransform2DInterface ( interface, superInterface )
		
		interface.getFieldOfView = nil
		interface.getFocalLength = nil
		interface.setFieldOfView = nil
		interface.setOrtho = nil
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setFarPlane ( self, far )
			superInterface.setFarPlane ( self, far or -1 )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setNearPlane ( self, near )
			superInterface.setNearPlane ( self, near or 1 )
		end
		
		-- extend the class
		local new = class.new
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.new ()
			local self = new ()
			superInterface.setOrtho ( self, true )
			superInterface.setNearPlane ( self, 1 )
			superInterface.setFarPlane ( self, -1 )
			return self
		end
	end
)

--============================================================--
-- MOAIFreeTypeFontReader
--============================================================--
if MOAIFreeTypeFontReader then
	MOAIFont.extend (
	
		'MOAIFont',
		
		----------------------------------------------------------------
		function ( interface, class, superInterface, superClass )
			
			-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
			-- extend the interface
			function interface.loadFromTTF ( self, filename, charcodes, points, dpi )
				self:load ( filename )
				self:preloadGlyphs ( charcodes, points, dpi )
			end
		
			-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
			-- extend the class
			local new = class.new
			
			function class.new ()
				local self = new ()
				superInterface.setCache ( self, MOAIGlyphCache.new ())
				superInterface.setReader ( self, MOAIFreeTypeFontReader.new ())
				return self
			end
		end
	)
end

--============================================================--
-- MOAIHttpTask
--============================================================--
MOAIHttpTask = MOAIHttpTaskCurl or MOAIHttpNaCl

--============================================================--
-- MOAILayer
--============================================================--
MOAILayer.extend (

	'MOAILayer2D',
	
	----------------------------------------------------------------
	function ( interface, class, superInterface, superClass )
	
		-- extend the interface
		initTransform2DInterface ( interface, superInterface )
		
		interface.setPartitionCull2D = nil
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setSortScale ( self, x, y, priority )
			superInterface.setSortScale ( self, x, y, 0, priority )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.wndToWorld ( self, x, y )
			local x, y = superInterface.wndToWorld ( self, x, y, 0 )
			return x, y
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.worldToWnd ( self, x, y )
			local x, y = superInterface.worldToWnd ( self, x, y, 0 )
			return x, y
		end
		
		-- extend the class
		class.SORT_Z_ASCENDING = nil
		class.SORT_Z_DESCENDING = nil
	end
)

--============================================================--
-- MOAIProp
--============================================================--
MOAIProp.extend (

	'MOAIProp2D',
	
	----------------------------------------------------------------
	function ( interface, class, superInterface, superClass )
	
		-- extend the interface
		initTransform2DInterface ( interface, superInterface )
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setFrame ( self, xMin, yMin, xMax, yMax )
			superInterface.setBounds ( self, xMin, yMin, 0, xMax, yMax, 0 )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.getRect ( self )			
			print ( self )
			local xMin, yMin, zMin, xMax, yMax, zMax = superInterface.getBounds ( self )
			return xMin, yMin, xMax, yMax
		end
	end
)

--============================================================--
-- MOAIRenderMgr
--============================================================--
MOAIRenderMgr.extend (

	'MOAIRenderMgr',
	
	----------------------------------------------------------------
	function ( class, superClass )
	
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		-- extend the class
		function class.affirmRenderTable ()
		
			local renderTable = class.getRenderTable ()
			if not renderTable then
				renderTable = {}
				class.setRenderTable ( renderTable )
			end
			
			return renderTable
		end

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.clearRenderStack ()
			class.setRenderTable ( nil )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.grabNextFrame ()
			local frameBuffer = MOAIGfxDevice.getFrameBuffer ()
			frameBuffer:grabNextFrame ()
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.getRenderTable ()
			local frameBuffer = MOAIGfxDevice.getFrameBuffer ()
			return frameBuffer:getRenderTable ()
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.popRenderPass ()

			local renderTable = class.affirmRenderTable ()
			table.remove ( renderTable )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.pushRenderPass ( pass )

			local renderTable = class.affirmRenderTable ()
			table.insert ( renderTable, pass )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.removeRenderPass ( pass )

			local renderTable = class.affirmRenderTable ()
			for i, cursor in ipairs ( renderTable ) do
				if cursor == pass then
					table.remove ( renderTable, i )
					break
				end
			end
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.setRenderTable ( renderTable )
		
			local frameBuffer = MOAIGfxDevice.getFrameBuffer ()
			frameBuffer:setRenderTable ( renderTable )
		end
	end
)

--============================================================--
-- MOAISim
--============================================================--
MOAISim.extend (

	'MOAISim',
	
	----------------------------------------------------------------
	function ( class, superClass )

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		-- extend the class
		function class.clearRenderStack ()
			MOAIRenderMgr.clearRenderStack ()
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.popRenderPass ()
			MOAIRenderMgr.popRenderPass ()
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.pushRenderPass ( pass )
			MOAIRenderMgr.pushRenderPass ( pass )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.removeRenderPass ( pass )
			MOAIRenderMgr.removeRenderPass ( pass )
		end 
	end
)

--============================================================--
-- MOAITextBox
--============================================================--
MOAITextBox.extend (

	'MOAITextBox',
	
	----------------------------------------------------------------
	function ( interface, class, superInterface, superClass )
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		-- extend the interface
		function interface.affirmStyle ( self )
			local style = self:getStyle ()
			if not style then
				style = MOAITextStyle.new ()
				self:setStyle ( style )
			end
			return style
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setFont ( self, font )
			local style = self:affirmStyle ()
			style:setFont ( font )
		end
		
		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function interface.setTextSize ( self, points, dpi )
			local style = self:affirmStyle ()
			style:setSize ( points, dpi )
		end
	end
)

--============================================================--
-- MOAITransform
--============================================================--
MOAITransform.extend (

	'MOAITransform2D',
	
	----------------------------------------------------------------
	function ( interface, class, superInterface, superClass )
	
		-- extend the interface
		initTransform2DInterface ( interface, superInterface )
	end
)

MOAIApp = MOAIAppAndroid or MOAIAppIOS
MOAIDialog = MOAIDialogAndroid or MOAIDialogIOS
MOAIMoviePlayer = MOAIMoviePlayerAndroid or MOAIMoviePlayerIOS

-- Optional 3rd party extensions
MOAIAdColony = MOAIAdColonyAndroid or MOAIAdColonyIOS
MOAIBilling = MOAIBillingAndroid or MOAIBillingIOS
MOAIChartBoost = MOAIChartBoostAndroid or MOAIChartBoostIOS
MOAICrittercism = MOAICrittercismAndroid or MOAICrittercismIOS
MOAIFacebook = MOAIFacebookAndroid or MOAIFacebookIOS
MOAINotifications = MOAINotificationsAndroid or MOAINotificationsIOS
MOAITapjoy = MOAITapjoyAndroid or MOAITapjoyIOS