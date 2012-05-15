#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	LOOP_HOME=/Users/adam/Documents/LOOP/loop-2.3-beta/lua

	pwd=`pwd`
	LUASOCKET_HOME=$pwd/../luasocket-2.0.2/src

	# copy settings
	cp $LUASOCKET_HOME/socket.lua socket.lua
	cp $LUASOCKET_HOME/mime.lua mime.lua
	cp $LUASOCKET_HOME/ltn12.lua ltn12.lua
	
	mkdir socket
	
	cp $LUASOCKET_HOME/ftp.lua socket/ftp.lua
	cp $LUASOCKET_HOME/http.lua socket/http.lua
	cp $LUASOCKET_HOME/smtp.lua socket/smtp.lua
	cp $LUASOCKET_HOME/tp.lua socket/tp.lua
	cp $LUASOCKET_HOME/url.lua socket/url.lua

	cd $LOOP_HOME
	lua precompiler.lua -d $pwd -o luasocketscripts -l "$pwd/?.lua" $pwd/ltn12.lua \
															$pwd/mime.lua \
															$pwd/socket.lua \
															$pwd/socket/ftp.lua \
															$pwd/socket/http.lua \
															$pwd/socket/smtp.lua \
															$pwd/socket/tp.lua \
															$pwd/socket/url.lua

	lua $LOOP_HOME/preloader.lua -d $pwd -o fullluasocket -I $LUASOCKET_HOME -I $pwd luasocket.h mime.h luasocketscripts.h 
	
	cd $pwd
	
	rm *.lua
	rm -r socket