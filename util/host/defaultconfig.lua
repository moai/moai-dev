  ApplicationId = "com.getmoai.moaitemplate" --be sure to change this!
  AppName = "Moai Template"
  CompanyName = ""
  LuaSrc = "src"

  android_gradle = {
     SdkDir = "",
     NdkDir = "",
     --valid values are moai-android-adcolony, moai-android-amazon-billing, moai-android-chartboost, moai-android-crittercism,
     --           moai-android-facebook, moai-android-flurry, moai-android-fortumo, moai-android-google-billing,
     --           moai-android-google-play-services, moai-android-google-push, moai-android-tapjoy,
     --           moai-android-twitter, moai-android-vungle
     Modules = {
        "moai-android-twitter"
     }
  }
