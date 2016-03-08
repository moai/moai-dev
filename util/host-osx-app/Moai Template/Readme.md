##OSX App Store Template

###Configure your lua path

    TLDR; Remove font-ttf folder ref and replace with a ref to your lua folder then edit bootstrap.lua with new folder reference

 * Open the project `Moai Template.xcodeproj`
 * On the left there will be a red item called `font-ttf`, this is a dead folder reference to the lua sample for fonts
 * Right click this item and `Delete`
 * Right click on `Moai Template` at the top
 * Click Add Files To Target Moai Template
 * Select your lua source folder
 * You should see your lua source folder in blue in the project navigator on the left
 * select the `app` folder, then the `lua` folder and edit `bootstrap.lua`
 * Change the calls in there to point to your lua folder you just added (the working directory is relative to the project so there should be no `/`s just the name of the folder)

###Customize the App
 
    TLDR; Change the bundle identifier and rename the project

  * Click on `Moai Template` at the top of the project navigator
  * In the right section on the `General` page, change the bundle Identifier
  * Click on `Moai Template` at the top of the project navigator again
  * Press enter to rename
	* Agree to the refactoring
	
	
###Building
  * Run the build.sh file __OR__ open project in xcode and hit the play button
	
	