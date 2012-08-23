----------------------------------------------------------------
--
--  Android App External Storage Moai Sample main.lua
--  This sample prints external storage information.
--  It does not display anything on the device.
--
--  Written in 2012 by Brendan A R Sechter <bsechter@sennue.com>
--
--  To the extent possible under law, the author(s) have
--  dedicated all copyright and related and neighboring rights
--  to this software to the public domain worldwide. This
--  software is distributed without any warranty.
--
--  You should have received a copy of the CC0 Public Domain
--  Dedication along with this software. If not, see
--  <http://creativecommons.org/publicdomain/zero/1.0/>.
--
----------------------------------------------------------------

-- Global Settings
programName  = "Android App External Storage Moai Sample"
screenWidth  = MOAIEnvironment.horizontalResolution or 1024
screenHeight = MOAIEnvironment.verticalResolution   or 9 / 16 * result.screenWidth

-- Simulator Window
MOAISim.openWindow( programName, screenWidth, screenHeight )

-- Viewport
viewport = MOAIViewport.new ( )
unitsX  = 1024
unitsY  = unitsX * screenHeight / screenWidth
viewport:setSize  ( screenWidth, screenHeight )
viewport:setScale ( unitsX,      unitsY       )

-- External Storage Information
if "Android" == MOAIEnvironment.osBrand then
  print ( "-----------------------",    "----------------------------------------------" )
  print ( "External Storage State:",    MOAIAppAndroid.getExternalStorageState ( ) )
  print ( "External Storage Root: ",    MOAIAppAndroid.getExternalStorageDirectory ( ) )
  print ( "-----------------------",    "----------------------------------------------" )
  print ( "External Alarms:       ",    MOAIAppAndroid.getExternalStoragePublicDirectory ( MOAIAppAndroid.DIRECTORY_ALARMS ) )
  print ( "External DCIM:         ",    MOAIAppAndroid.getExternalStoragePublicDirectory ( MOAIAppAndroid.DIRECTORY_DCIM ) )
  print ( "External Downloads:    ",    MOAIAppAndroid.getExternalStoragePublicDirectory ( MOAIAppAndroid.DIRECTORY_DOWNLOADS ) )
  print ( "External Movies:       ",    MOAIAppAndroid.getExternalStoragePublicDirectory ( MOAIAppAndroid.DIRECTORY_MOVIES ) )
  print ( "External Music:        ",    MOAIAppAndroid.getExternalStoragePublicDirectory ( MOAIAppAndroid.DIRECTORY_MUSIC ) )
  print ( "External Notification: ",    MOAIAppAndroid.getExternalStoragePublicDirectory ( MOAIAppAndroid.DIRECTORY_NOTIFICATIONS ) )
  print ( "External Pictures:     ",    MOAIAppAndroid.getExternalStoragePublicDirectory ( MOAIAppAndroid.DIRECTORY_PICTURES ) )
  print ( "External Podcasts:     ",    MOAIAppAndroid.getExternalStoragePublicDirectory ( MOAIAppAndroid.DIRECTORY_PODCASTS ) )
  print ( "External Ringtones:    ",    MOAIAppAndroid.getExternalStoragePublicDirectory ( MOAIAppAndroid.DIRECTORY_RINGTONES ) )
  print ( "-----------------------",    "----------------------------------------------" )
  print ( "MEDIA_BAD_REMOVAL      ",    MOAIAppAndroid.MEDIA_BAD_REMOVAL )
  print ( "MEDIA_CHECKING         ",    MOAIAppAndroid.MEDIA_CHECKING )
  print ( "MEDIA_MOUNTED          ",    MOAIAppAndroid.MEDIA_MOUNTED )
  print ( "MEDIA_MOUNTED_READ_ONLY",    MOAIAppAndroid.MEDIA_MOUNTED_READ_ONLY )
  print ( "MEDIA_NOFS             ",    MOAIAppAndroid.MEDIA_NOFS )
  print ( "MEDIA_REMOVED          ",    MOAIAppAndroid.MEDIA_REMOVED )
  print ( "MEDIA_SHARED           ",    MOAIAppAndroid.MEDIA_SHARED )
  print ( "MEDIA_UNMOUNTABLE      ",    MOAIAppAndroid.MEDIA_UNMOUNTABLE )
  print ( "MEDIA_UNMOUNTED        ",    MOAIAppAndroid.MEDIA_UNMOUNTED )
  print ( "-----------------------",    "----------------------------------------------" )
  print ( "DIRECTORY_ALARMS       ",    MOAIAppAndroid.DIRECTORY_ALARMS )
  print ( "DIRECTORY_DCIM         ",    MOAIAppAndroid.DIRECTORY_DCIM )
  print ( "DIRECTORY_DOWNLOADS    ",    MOAIAppAndroid.DIRECTORY_DOWNLOADS )
  print ( "DIRECTORY_MOVIES       ",    MOAIAppAndroid.DIRECTORY_MOVIES )
  print ( "DIRECTORY_MUSIC        ",    MOAIAppAndroid.DIRECTORY_MUSIC )
  print ( "DIRECTORY_NOTIFICATIONS",    MOAIAppAndroid.DIRECTORY_NOTIFICATIONS )
  print ( "DIRECTORY_PICTURES     ",    MOAIAppAndroid.DIRECTORY_PICTURES )
  print ( "DIRECTORY_PODCASTS     ",    MOAIAppAndroid.DIRECTORY_PODCASTS )
  print ( "DIRECTORY_RINGTONES    ",    MOAIAppAndroid.DIRECTORY_RINGTONES )
  print ( "-----------------------",    "----------------------------------------------" )
else
  print ( "This sample only works on Adroid devices." )
end
