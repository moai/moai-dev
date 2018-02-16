----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

charcodes = ' φωψÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ'
text = 'φωψ ÀÁÂÃÄÅ ÆÇ ÈÉÊË ÌÍÎÏ ÐÑ ÒÓÔÕÖ ×Ø ÙÚÛÜ ÝÞß àáâãäå æç èéêë ìíîï ðñ òóôõö ÷ø ùúûü ýþÿ'

font = MOAIFont.new ()
font:loadFromTTF ( 'times.ttf', charcodes, 12, 163 )

textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( 12, 163 )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
textbox:setPartition ( layer )

