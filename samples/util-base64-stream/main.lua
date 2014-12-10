----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'HELLO' )

stream = MOAIMemStream.new ()
stream:open ()

data = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis id massa vel leo blandit pharetra. Aenean a nisl mi. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Nam quis magna sit amet diam fermentum consequat. Donec dapibus pharetra diam vel convallis. Pellentesque quis tellus mauris. Sed eget risus tortor, in cursus nisi. Sed ultrices nulla non nunc ullamcorper id venenatis urna ultrices. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Nam sodales tellus et diam imperdiet pharetra sagittis odio tempus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc mollis adipiscing nibh ut malesuada. Proin rutrum volutpat est sed feugiat. Suspendisse at imperdiet justo. Pellentesque ullamcorper risus venenatis tellus elementum mattis. Quisque adipiscing feugiat orci vitae egestas.'
len = #data

print ( data )

writer = MOAIStreamWriter.new ()
writer:openBase64 ( stream )
writer:write ( data, len )
writer:close ()

stream:seek ( 0 )

reader = MOAIStreamReader.new ()
reader:openBase64 ( stream )
data = reader:read ( len )
reader:close ()

print ()
print ( data )