##Android Host (Android Studio Gradle)

###Requirements
  * Oracle JDK 1.7+
  * set `JAVA_HOME` to path of the jdk
  * Download and Install [Android Studio and SDK Manager](https://developer.android.com/sdk/installing/studio.html)
  * Download and Install Android NDK r9d
     * [Windows](http://dl.google.com/android/ndk/android-ndk-r9d-windows-x86.zip)
     * [Mac](http://dl.google.com/android/ndk/android-ndk-r9d-darwin-x86_64.tar.bz2)
     * [Linux](http://dl.google.com/android/ndk/android-ndk-r9d-linux-x86_64.tar.bz2)
     
###Setup your lua path
  * open `gradle.properties` in the root of the project
  * edit the moaiLuaRoot setting to point to your lua sources (you can use paths relative to the `gradle.properties` file, avoid drive letters or escape the `:` with `\`)
  * edit moaiModules to include the plugins you want (currently for this proof of concept only moai-android-twitter is available)  
  
   
###Customise your application 
  * You can edit your application config from android studio or just from a file browser
  * At the very least you will need to change the applicationId in the `./app/build.gradle` file (this is what identifies your app on the play store and must be unique and constant per app)
    * open `./app/build.gradle`
      * Change applicationId, (and version etc if required) under `defaultConfig`
      
      
  * To change what your app is called you should edit the `app_name` in the `./app/src/main/res/values/strings.xml` file
    * open `./app/src/main/res/values/strings.xml`
       * Change the fields you want changed (`app_name` at least).
       
     
###Compile with android studio
  * Before compiling gradle/android studio need to know where your ndk and sdk are, edit the `local.properties` in the root of the project and ensure `sdk.dir` and `ndk.dir` are set correctly 
  
  * Run android studio and `Import Project...`, then launch with the play button __OR__ run `build.sh` (build.bat on windows)
  
  