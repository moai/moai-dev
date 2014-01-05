// VGDownload.h -- vungle download tracking
// by allen brunson  february 16 2012


/******************************************************************************/
/*                                                                            */
/***  functions                                                             ***/
/*                                                                            */
/******************************************************************************/

// call this at app startup

void VGReportDownload(NSString* appID);


/******************************************************************************/
/*                                                                            */
/***  VGDownload class                                                      ***/
/*                                                                            */
/******************************************************************************/

@interface VGDownload: NSObject
{
}

// construction and destruction

-(void)dealloc;
-(id)init;

// instance methods

-(void)reportAppInstall:(NSString*)URL;

@end


/******************************************************************************/
/*                                                                            */
/***  VGDownload module                                                     ***/
/*                                                                            */
/******************************************************************************

overview
--------

This module can be added to your iOS application to track app downloads. To use
it, drag these four files into your project:

  VGDownload.m
  VGDownload.h
  OpenUDID.m
  OpenUDID.h
  
into your project, and call the function VGReportDownload() from your
application delegate's startup method, which likely goes by this name:

  -(BOOL)application:(UIApplication*)application
   didFinishLaunchingWithOptions:(NSDictionary*)launchOptions

For more information, see: http://api.vungle.com/api/downloads
  
  
threading concerns
------------------  

this module gathers information in the main thread, then performs its http
request in a background thread. this satisfies apple's threading rule that
says all ui classes, such as UIDevice, must be used only in the main thread.


compatibility
-------------

this module stores its installed state as a BOOL in the NSUserDefaults prefs
database. the vungle pub ad sdk saves this information in exactly the same way.
so it's safe for a program to switch from using this module to that library, or
vice versa.

*/
