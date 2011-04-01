serializer = MOAISerializer.new ()

test = {
	foo = 0,
	bar = 1,
	baz = 92,
}

transform = MOAITransform.new ()

serializer:serialize ( test )
serializer:serialize ( transform )
testStr = serializer:exportToString ()

print ( testStr )
