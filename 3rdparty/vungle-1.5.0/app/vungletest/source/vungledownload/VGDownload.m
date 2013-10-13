// VGDownload.m -- vungle download tracking
// by allen brunson  february 16 2012

// for getting mac address
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <string.h>

#import "OpenUDID.h"
#import "VGDownload.h"

// don't change the name of this key! it is shared with the vungle pub ad sdk
static NSString* sKeyAppsInstalled = @"VGAppsInstalled";

static BOOL appInstalledGet(NSString *appID)
{
    NSUserDefaults* ndef = [NSUserDefaults standardUserDefaults];
	NSDictionary *dict = [ndef objectForKey:sKeyAppsInstalled];
    return [[dict objectForKey:appID] isEqualToNumber:[NSNumber numberWithBool:YES]];
}

static void appInstalledSet(BOOL state, NSString *appID)
{
    NSUserDefaults* ndef = [NSUserDefaults standardUserDefaults];
	NSMutableDictionary *dict = [[[ndef objectForKey:sKeyAppsInstalled] mutableCopy] autorelease];
	if (!dict) {
		dict = [NSMutableDictionary dictionary];
	}
    [dict setObject:[NSNumber numberWithBool:YES] forKey:appID];
	[ndef setObject:dict forKey:sKeyAppsInstalled];
    [ndef synchronize];
}

static NSString* formatHexBytes(const uint8_t* bytes, size_t length)
{
    NSMutableString*  text = [NSMutableString string];
    
    for (size_t indx = 0; indx < length; indx++)
    {
        [text appendString:[NSString stringWithFormat:@"%02x", bytes[indx]]];
    }
    
    return [NSString stringWithString:text];
}

NS_INLINE BOOL isKindOf(id object, Class classType)
{
    return object && classType && [object isKindOfClass:classType];
}

// mac address of wifi interface. works even if wifi is switched off.

static NSString* macAddress(void)
{
    const size_t kMacBytes = 6;
    const size_t kMgmtSize = 6;
    
    uint8_t              addr[kMacBytes];
    size_t               blen = 0;
    uint8_t*             buff = NULL;
    struct if_msghdr*    imsg = NULL;
    int                  mgmt[kMgmtSize];
    struct sockaddr_dl*  sock = NULL;
    
    memset(addr, 0, sizeof(addr));
    memset(mgmt, 0, sizeof(mgmt));
    
    mgmt[0] = CTL_NET;
    mgmt[1] = AF_ROUTE;
    mgmt[2] = 0;              
    mgmt[3] = AF_LINK;
    mgmt[4] = NET_RT_IFLIST;
    mgmt[5] = if_nametoindex("en0");
    
    if (mgmt[5] == 0) 
    {
        return @"if_nametoindex failed";
    }
    
    if (sysctl(mgmt, kMgmtSize, NULL, &blen, NULL, 0) < 0)
    {
        return @"sysctl1 failed";
    }
    
    if ((buff = malloc(blen)) == NULL)
    {
        return @"buffer alloc failed";
    }
    
    if (sysctl(mgmt, kMgmtSize, buff, &blen, NULL, 0) < 0)
    {
        free(buff);
        buff = NULL;
        
        return @"sysctl2 failed";
    }    
    
    imsg = (struct if_msghdr*) buff;
    sock = (struct sockaddr_dl*) (imsg + 1);
    memcpy(addr, sock->sdl_data + sock->sdl_nlen, sizeof(addr));
    
    free(buff);
    buff = NULL;
    
    return formatHexBytes(addr, sizeof(addr));
}

static BOOL runRequest(NSString* URL)
{
    NSData*             data = nil;
    NSError*            eval = nil;
    NSURL*              nurl = [NSURL URLWithString:URL];
    NSURLRequest*       nreq = [NSURLRequest requestWithURL:nurl];
    NSHTTPURLResponse*  resp = nil;
    
    data = [NSURLConnection sendSynchronousRequest:nreq 
                                 returningResponse:&resp error:&eval];
    
    if (!isKindOf(data, [NSData class])) return FALSE; 
    if (!isKindOf(resp, [NSHTTPURLResponse class])) return FALSE;
    
    return (eval == nil) && (data.length >= 1) && (resp.statusCode == 200);
}

static NSString* systemIdentifier(void)
{
    NSString* openUDID = [OpenUDID value];
	return openUDID;
}

static NSString* url(NSString* appID)
{
    NSString*  addr = macAddress();
    NSString*  nfmt = nil;
    NSString*  udid = systemIdentifier();
    
    nfmt = @"http://api.vungle.com/api/v1/new?isu=%@&app_id=%@&ma=%@";
    return [NSString stringWithFormat:nfmt, udid, appID, addr];
}

void VGReportDownload(NSString* appID)
{
    VGDownload*  dnld = nil;
    SEL          proc = @selector(reportAppInstall:);
    if (appInstalledGet(appID)) return;
    
    dnld = [[[VGDownload alloc] init] autorelease];
    [dnld performSelectorInBackground:proc withObject:appID];
}

@implementation VGDownload

-(void)dealloc
{
    [super dealloc];
}

-(id)init
{
    self = [super init];
    if (!self) return nil;
    
    return self;
}

-(void)reportAppInstall:(NSString *)appID
{
    NSAutoreleasePool*  pool = [[NSAutoreleasePool alloc] init];
    const BOOL          rval = runRequest(url(appID));
    
    appInstalledSet(rval, appID);
    
    [pool drain];
}

@end

