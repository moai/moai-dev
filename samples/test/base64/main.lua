data = MOAIDataBuffer.deflate ( 'A dreary young bank clerk named Fennis wished to foster an aura of menace', 9 )
data = MOAIDataBuffer.base64Encode ( data )
print ( data )

data = MOAIDataBuffer.base64Decode ( data )
data = MOAIDataBuffer.inflate ( data )
print ( data )

