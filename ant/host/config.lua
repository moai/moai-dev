
PROJECT_NAME				= 'MoaiSample'
PACKAGE_NAME				= 'com.ziplinegames.moaisample'
PLATFORM_NAME				= 'android'

MODULES = {

	adcolony = {
		src = MOAI_SDK_HOME .. 'src/moai-android-adcolony',
		lib = MOAI_SDK_HOME .. '3rdparty-android/adcolony-2.0.6',
	},

	chartboost = {
		src = MOAI_SDK_HOME .. 'src/moai-android-chartboost',
		lib = MOAI_SDK_HOME .. '3rdparty-android/chartboost-3.4.0',
	},

	contrib	= {
		lib = MOAI_SDK_HOME .. '3rdparty-android/contrib',
	},

	crittercism = {
		src = MOAI_SDK_HOME .. 'src/moai-android-crittercism',
		lib = MOAI_SDK_HOME .. '3rdparty-android/crittercism-4.4.0',
	},

	facebook = {
		src = MOAI_SDK_HOME .. 'src/moai-android-facebook',
		lib = MOAI_SDK_HOME .. '3rdparty-android/facebook-v3',
	},

	flurry = {
		src = MOAI_SDK_HOME .. 'src/moai-android-flurry',
		lib = MOAI_SDK_HOME .. '3rdparty-android/flurry-3.3.4',
	},

	google_billing = {
		src = MOAI_SDK_HOME .. 'src/moai-android-google-billing',
		lib = MOAI_SDK_HOME .. '3rdparty-android/google-billing-v3',
	},

	google_play = {
		src = MOAI_SDK_HOME .. 'src/moai-android-google-play-services',
		lib = MOAI_SDK_HOME .. '3rdparty-android/google-play-services',
	},

	google_push = {
		src = MOAI_SDK_HOME .. 'src/moai-android-google-push',
		lib = MOAI_SDK_HOME .. '3rdparty-android/google-push',
	},

	moai = {
		src = MOAI_SDK_HOME .. 'src/moai-android',
	},

	tapjoy = {
		src = MOAI_SDK_HOME .. 'src/moai-android-tapjoy',
		lib = MOAI_SDK_HOME .. '3rdparty-android/tapjoy-9.1.5',
	},

	vungle = {
		src = MOAI_SDK_HOME .. 'src/moai-android-vungle',
		lib = MOAI_SDK_HOME .. '3rdparty-android/vungle-1.3.11',
	},
}
