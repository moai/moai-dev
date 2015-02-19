WIN_WIDTH = 320
WIN_HEIGHT = 480

--==============================================================
-- staging
--==============================================================

----------------------------------------------------------------
stage = function ()
	print ( 'staging' )
end

--==============================================================
-- testing
--==============================================================

----------------------------------------------------------------
test = function ()

	coroutine.yield ()

	----------------------------------------------------------------
	MOAITestMgr.test ( 'Test A', function ()

		MOAITestMgr.comment ( 'comment 1' )
		MOAITestMgr.comment ( 'comment 2' )
		MOAITestMgr.comment ( 'comment 3' )

		MOAITestMgr.comment ( 'yield 1' )
		coroutine.yield ()

		MOAITestMgr.comment ( 'yield 2' )
		coroutine.yield ()

		MOAITestMgr.comment ( 'yield 3' )
		coroutine.yield ()
		
		MOAITestMgr.comment ( 'DONE' )

	end )

	----------------------------------------------------------------
	MOAITestMgr.test ( 'Test B', function ()

		local viewport = MOAIViewport.new ()
		viewport:setSize ( 320, 480 )
		viewport:setScale ( 320, 480 )

		local layer = MOAILayer2D.new ()
		layer:setViewport ( viewport )
		MOAISim.pushRenderPass ( layer )

		local gfxQuad = MOAIGfxQuad2D.new ()
		gfxQuad:setTexture ( "moai.png" )
		gfxQuad:setRect ( -64, -64, 64, 64 )

		local prop = MOAIProp2D.new ()
		prop:setDeck ( gfxQuad )
		layer:insertProp ( prop )
		 
	    local action
	 
	    action = prop:moveRot ( -360, 3 )
		while action:isBusy () do
	    	coroutine.yield ()
	    end

	end )

end
