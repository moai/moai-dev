#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)
	
	include OptionalComponentsDefined.mk

	LOCAL_MODULE 		:= moaiext-android
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= $(DISABLE_ADCOLONY) $(DISABLE_BILLING) $(DISABLE_CHARTBOOST) $(DISABLE_CRITTERCISM) $(DISABLE_FACEBOOK) $(DISABLE_NOTIFICATIONS) $(DISABLE_TAPJOY)
	LOCAL_CFLAGS		+= -include $(MY_MOAI_ROOT)/src/zlcore/zl_replace.h

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)	
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAIAppAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAIDialogAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAIAdColonyAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAIBillingAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAIChartBoostAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAICrittercismAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAIFacebookAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAIMoviePlayerAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAINotificationsAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAITapjoyAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAIImagePickerAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAIKeyboardAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAIFlurryAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAIMobclixAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAISmartFoxAndroid.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/smartfox/Core/BaseEvent.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/smartfox/Core/SFSEvent.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/smartfox/Core/SFSBuddyEvent.cpp


	include $(BUILD_STATIC_LIBRARY)