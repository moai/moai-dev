  ApplicationId = "com.getmoai.moaitemplate" --be sure to change this!
  AppName = "Moai Template"
  CompanyName = ""
  LuaSrc = "src"
  HostSettings = {
    ["android-gradle"] = {
       SdkDir = "",
       NdkDir = "",
       --valid values are moai-android-adcolony, moai-android-amazon-billing, moai-android-chartboost, moai-android-crittercism,
       --           moai-android-facebook, moai-android-flurry, moai-android-fortumo, moai-android-google-billing,
       --           moai-android-google-play-services, moai-android-google-push, moai-android-tapjoy,
       --           moai-android-twitter, moai-android-vungle
       Modules = {
          "moai-android-twitter"
       },
       -- Facebook app ID for use with moai-android-facebook
       FBAppId = 0,
       -- google app id for use with Google Play Services
       GMSAppId = 0,
       -- scheme name , url scheme name for launching app
       SchemeName ="moaischeme"
       
       --icons, valid keys are: hdpi,mdpi,xhdpi,xxhdpi, paths can be relative to this config file
       --Icons = {
       --    hdpi = "hostconfig/android/icons/hdpi/icon.png',
      --     mdpi = "hostconfig/android/icons/mdpi/icon.png ,
      --     xxhdpi = "" --leave blank to remove the default moai icon for this resolution
       -- } 
    },
    
    ["windows"] = {
        ----you can configure a custom icon. path is relative to this config file
        --Icon = "hostconfig/windows/dp.ico" 
        
        ----The executable file description 
        --ExeDescription = "Another Great Moai Game"
        
    },
    
    ["html"] = {
        --you can configure a background image for the player, a splash or title screen like those seen on youtube
        --Background = "hostconfig/html/background.png"
        
        --you can also set the ram in MB to be allocated
        --Ram = 256
    },
    
    ["osx"] = {
      --configure custom icon with path relative to this config file
      --Icon = "hostconfig/osx/app.png"
    }
    
  }
