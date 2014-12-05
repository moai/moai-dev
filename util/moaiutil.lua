require ( 'util' )
require ( 'http' )

--==============================================================
-- setup
--==============================================================

INVOKE_DIR		= MOAIFileSystem.getAbsoluteDirectoryPath ( arg [ 1 ])
MOAI_SDK_HOME	= MOAIFileSystem.getAbsoluteDirectoryPath ( arg [ 2 ])
MOAI_CMD		= arg [ 3 ]
SCRIPT_DIR		= string.format ( '%sutil/%s/', MOAI_SDK_HOME, MOAI_CMD )

MOAIFileSystem.setWorkingDirectory ( SCRIPT_DIR )

dofile ( 'main.lua' )
