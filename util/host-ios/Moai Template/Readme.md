##iOS App Template

###Configure your lua path

    TLDR; Remove hello-moai folder ref and replace with a ref to your lua folder then edit bootstrap.lua with new folder reference

 * Open the project `MoaiTemplate.xcodeproj`
 * On the left there will be a red item called `hello-moai`, this is a dead folder reference to a moai lua sample
 * Right click this item and `Delete`
 * Right click on `MoaiTemplate` at the top
 * Click Add Files To Target MoaiTemplate
 * Select your lua source folder
 * You should see your lua source folder in blue in the project navigator on the left
 * Edit `bootstrap.lua`
 * Change the calls in there to point to your lua folder you just added (the working directory is relative to the project so there should be no `/`s just the name of the folder)

###Customize the App
 
    TLDR; Change the bundle identifier and rename the project

  * Click on `MoaiTemplate` at the top of the project navigator
  * In the right section on the `General` page, change the bundle Identifier
  * Click on `MoaiTemplate` at the top of the project navigator again
  * Press enter to rename
	* Agree to the refactoring
	
	
###Building

  * Run the build.sh file __OR__ open project in xcode and hit the play button
	
	