----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.extend (

	'MOAIXSim',

	function ( class, superClass )

		print ( "EXTEND SINGLETON" )

		function class.openWindow ( name, width, height )
			print ( name, width, height )
			superClass.openWindow ( name, width, height )
		end
	end
)

MOAIProp2D.extend (

	'MOAIXProp2D',
	
	function ( interface, class, superInterface, superClass )
		
		print ( "EXTEND FACTORY" )
		
		function interface:moveRot ( degrees, delay )
			print ( degrees, delay )
			superInterface.moveRot ( self, degrees, delay )
		end
		
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
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

MOAIXProp2D.test ()
prop = MOAIXProp2D.new ()

prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )
