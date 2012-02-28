#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	# DO NOT CHECK THIS FILE INTO VERSION CONTROL
	# This file is local and should be added to 
	# the ignore list of your version control system.

#----------------------------------------------------------------#
# path to Android SDK folder
# On Windows, you MUST use forward slashes as directory separators.
# For example: C:/android-sdk
#----------------------------------------------------------------#

	android_sdk_root="/Android/android-sdk-macosx"
	
#----------------------------------------------------------------#
# debug & release settings
# you must define key store data in order to build for release
#----------------------------------------------------------------#

	debug=false
	key_store=../../../../../SVN-Repositories/zipline-dev-trunk/signing/android/zipline.test.keystore
	key_alias=android
	key_store_password=DoMoreFaster
	key_alias_password=DoMoreFaster