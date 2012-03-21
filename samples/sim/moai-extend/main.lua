----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIXSim = MOAISim.extend (

	function ( class, super )

		print ( "EXTEND SINGLETON" )

		function class.openWindow ( name, width, height )
			print ( name, width, height )
			super.openWindow ( name, width, height )
		end
	end
)

MOAIXProp2D = MOAIProp2D.extend (

	function ( interface, super )
		
		print ( "EXTEND FACTORY" )
		
		function interface:moveRot ( degrees, delay )
			print ( degrees, delay )
			super.moveRot ( self, degrees, delay )
		end
	end,
	
	function ( class, super )
		
		print ( "EXTEND CLASS" )
		
		function class:test ()
			print ( "TEST" )
		end
	end
)

MOAIXSim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

MOAIXProp2D.test ()
prop = MOAIXProp2D.new ()

prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )
