----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- this sample script and 'correct result' image were kindly submitted by
-- Dr. M. Shaban

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'أابتثجحخدذرزسشصضطظعغفقكلمنهويءئى'
text = 'بسم الله الرحمن الرحيم' -- 'In the name of God, the compassionate and merciful'

font = MOAIFont.new ()
font:loadFromTTF ( 'times.ttf', charcodes, 10, 163 )

textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( font:getScale ())
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )

-- set up rtl
-- this is a work in progress
-- right now Moai textbox does not take into account any kind of language
-- specific shaping, ligatures, etc. or attempt to use 'smart font' technology;
-- the current implementation is simply a naive reversal of characters, which is
-- inadequate for languages requiring 'complex text layout' (CTL)

-- the most likely fix is to integrate a CTL package. research points to
-- HarfBuzz-ng as the most likely candidate

textbox:setAlignment ( MOAITextBox.RIGHT_JUSTIFY )
textbox:setRightToLeft ( true )

layer:insertProp ( textbox )

