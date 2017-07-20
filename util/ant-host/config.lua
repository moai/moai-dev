ANDROID_PLATFORM_TARGET				= "android-17"
OS_BRAND 							= "Android"

MODULES = {

	adcolony 		= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/adcolony-2.0.6',
		src				= MOAI_SDK_HOME .. 'src/moai-android-adcolony',
	},

	amazon_billing	= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/amazon-billing-1.0.3',
		src				= MOAI_SDK_HOME .. 'src/moai-android-amazon-billing',
	},
	
	chartboost 		= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/chartboost-5.1.1',
		src				= MOAI_SDK_HOME .. 'src/moai-android-chartboost',
	},

	contrib 		= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/contrib',
	},
	
	crittercism 	= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/crittercism-4.4.0',
		src				= MOAI_SDK_HOME .. 'src/moai-android-crittercism',
	},
	
	deltadna 		= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/deltadna-v3.3.6',
		src				= MOAI_SDK_HOME .. 'src/moai-android-deltadna',
	},

	facebook 		= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/facebook-v4.4.1',
		src				= MOAI_SDK_HOME .. 'src/moai-android-facebook',
	},
	
	flurry 			= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/flurry-3.3.4',
		src				= MOAI_SDK_HOME .. 'src/moai-android-flurry',
	},
	
	google_billing	= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/google-billing-v3',
		src				= MOAI_SDK_HOME .. 'src/moai-android-google-billing',
	},
	
	google_play 	= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/google-play-services',
		src				= MOAI_SDK_HOME .. 'src/moai-android-google-play-services',
	},
	
	google_push 	= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/google-push',
		src				= MOAI_SDK_HOME .. 'src/moai-android-google-push',
	},
	
	moai 			= {
		src				= MOAI_SDK_HOME .. 'src/moai-android',
		bin				= MOAI_SDK_HOME .. 'ant/libmoai/libs/armeabi-v7a/libmoai.so',
	},

	tapjoy 			= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/tapjoy-9.1.5',
		src				= MOAI_SDK_HOME .. 'src/moai-android-tapjoy',
	},
	
	vungle			= {
		lib				= MOAI_SDK_HOME .. '3rdparty-android/vungle-1.3.11',
		src				= MOAI_SDK_HOME .. 'src/moai-android-vungle',
	},
}
