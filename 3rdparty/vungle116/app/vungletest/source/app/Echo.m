// Echo.m -- print to terminal
// by allen brunson  february 14 2012

#import "AppDelegate.h"
#import "Echo.h"
#import "MainViewController.h"

static BOOL sLogEnabled = TRUE;

#pragma mark internal functions

// output one line to stdout and/or the target/action pair

static void buffOut(NSString* text)
{
    MainViewController*  cntl = AppDelegate.appDelegate.mainViewController;
    
    VG_ASSERT(VGIsString(text));
    
    if (sLogEnabled)
    {
        puts(text.UTF8String);
        fflush(stdout);
    }
    
    [cntl performSelectorOnMainThread:@selector(vungleLogLine:)
     withObject:text waitUntilDone:FALSE];
}

static NSString* dateTimeString(void)
{
    NSDate*           date = [[NSDate alloc] init];
    NSDateFormatter*  dfmt = [[NSDateFormatter alloc] init];
    NSString*         text = nil;
    
    dfmt.dateFormat = @"yyyy/MM/dd HH:mm:ss";
    text = [dfmt stringFromDate:date];
    
    VG_SAFE_RELEASE(dfmt);
    VG_SAFE_RELEASE(date);
    
    return text;
}

// input a string that may contain newlines, break it up into single lines, and
// output date/time and text, one at a time, to buffOut()

static void stringOut(NSString* text)
{
    NSString*  date = dateTimeString();
    NSString*  tbuf = nil;
    
    for (NSString* line in [NSString lines:text])
    {
        VG_ASSERT(VGIsString(line));
        tbuf = [[NSString alloc] initWithFormat:@"%@  %@", date, line];
        buffOut(tbuf);
        VG_SAFE_RELEASE(tbuf);
    }    
    
    VG_SAFE_RELEASE(tbuf);
}

#pragma mark public functions

void nlog(NSString* nfmt, ...)
{
    va_list    args;
    NSString*  text = nil;
    
    va_start(args, nfmt);
    text = [[NSString alloc] initWithFormat:nfmt arguments:args];
    va_end(args);
    
    stringOut(text);
    
    VG_SAFE_RELEASE(text);
}

void nprn(NSString* text)
{
    stringOut(text);
}
