----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'MOAI DSL example' )

instance = MOAIExampleInstanceDSL.new ()

stream = MOAIRetainedDSLStream.new ()
stream
    :_ ( MOAIExampleSharedDSL )
    :foo ()
    :bar ()
    :baz ()
    :_ ( instance )
    :foo ()
    :bar ()
    :baz ()

print ( 'RUN 1' )
stream:executeBytecode ()
print ()

print ( 'RUN 2' )
stream:executeBytecode ()
print ()

print ( 'RUN 3' )
stream:executeBytecode ()
print ()

print ( 'IMMEDIATE' )
stream = MOAIImmediateDSLStream.new ()
stream
    :_ ( MOAIExampleSharedDSL )
    :foo ()
    :bar ()
    :baz ()
    :_ ( instance )
    :foo ()
    :bar ()
    :baz ()
