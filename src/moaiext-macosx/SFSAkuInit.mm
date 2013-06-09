//
//  SFSAkuInit.m
//  MoaiSample
//
//  Created by Brendan Ragan on 21/03/13.
//
//

#import "SFSAkuInit.h"
#import "moaiext-macosx.h"
#include <sys/types.h>
#include <sys/sysctl.h>

@implementation SFSAkuInit

+(void)MoaiTypesInit {
	loadMoaiLib_NSArray ();
	loadMoaiLib_NSData ();
//	loadMoaiLib_NSDate ();
	loadMoaiLib_NSDictionary ();
	loadMoaiLib_NSError ();
	loadMoaiLib_NSNumber ();
	loadMoaiLib_NSObject ();
	loadMoaiLib_NSString ();
}

+(NSString*)SetMoaiEnvironment:(cc8*)key withDefault:(NSString*)defaultValue orBundleKey:(NSString*)bundleKey {
	MOAIEnvironment& environment = MOAIEnvironment::Get ();
	NSString *temp = [[[ NSBundle mainBundle ] infoDictionary ] objectForKey:bundleKey];
	if (temp == nil)
		temp = defaultValue;
	environment.SetValue(key, [temp UTF8String]);
	return temp;
}

+(void)MoaiEnvironmentInit {
	MOAIEnvironment& environment = MOAIEnvironment::Get ();
	size_t len = 0;
    sysctlbyname("hw.model", NULL, &len, NULL, 0);
	char *model;
    if (len) {
		model = (char*)malloc(len*sizeof(char));
        sysctlbyname("hw.model", model, &len, NULL, 0);
	} else {
		model = "Unknown";
	}
	
	[SFSAkuInit SetMoaiEnvironment:MOAI_ENV_appDisplayName withDefault:@"Moai Debug" orBundleKey:@"CFBundleDisplayName"];
	NSString *bundleID = [SFSAkuInit SetMoaiEnvironment:MOAI_ENV_appID withDefault:@"moai-test-debug" orBundleKey:@"CFBundleIdentifier"];
	[SFSAkuInit SetMoaiEnvironment:MOAI_ENV_appVersion withDefault:@"UNKNOWN VERSION" orBundleKey:@"CFBundleShortVersionString"];
	[SFSAkuInit SetMoaiEnvironment:MOAI_ENV_buildNumber withDefault:@"UNKNOWN BUILD" orBundleKey:@"CFBundleVersion"];
	
	NSFileManager *fileManager = [NSFileManager defaultManager];
	
	NSString *cacheDir = [NSString stringWithFormat:@"%@/.%s/cache/", NSHomeDirectory(), [bundleID UTF8String]];

	if(![fileManager fileExistsAtPath:cacheDir])
		[fileManager createDirectoryAtPath:cacheDir withIntermediateDirectories:YES attributes:nil error:NULL];
	
	environment.SetValue ( MOAI_ENV_cacheDirectory,		[cacheDir UTF8String]);
	environment.SetValue ( MOAI_ENV_countryCode,		[[[ NSLocale currentLocale ] objectForKey: NSLocaleCountryCode ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_cpuabi,				"x86");
	environment.SetValue ( MOAI_ENV_devBrand,			"Apple");
	environment.SetValue ( MOAI_ENV_devUserName,		[[NSHost currentHost].localizedName UTF8String ]);
//	environment.SetValue ( MOAI_ENV_devName,			[[ UIDevice currentDevice ].localizedModel UTF8String ]);
	environment.SetValue ( MOAI_ENV_devManufacturer,	"Apple");
	environment.SetValue ( MOAI_ENV_devModel,			model );
	environment.SetValue ( MOAI_ENV_devPlatform,		"MacOSX");
//	environment.SetValue ( MOAI_ENV_devProduct,			[[ UIDevice currentDevice ].model UTF8String ]);
	NSString *docsDir = [NSString stringWithFormat:@"%@/.%@/documents/", NSHomeDirectory(), bundleID];
	environment.SetValue ( MOAI_ENV_documentDirectory,	[docsDir UTF8String]);

	if(![fileManager fileExistsAtPath:docsDir])
		[fileManager createDirectoryAtPath:docsDir withIntermediateDirectories:YES attributes:nil error:NULL];
	
//	environment.SetValue ( MOAI_ENV_iosRetinaDisplay,	[[ UIScreen mainScreen ] scale ] == 2.0 );
	environment.SetValue ( MOAI_ENV_languageCode,		[[[ NSLocale currentLocale ] objectForKey: NSLocaleLanguageCode ] UTF8String ]);
//	environment.SetValue ( MOAI_ENV_osBrand,			"iOS" ); // THIS IS SET ELSEWERE
	environment.SetValue ( MOAI_ENV_osVersion,			[[NSProcessInfo processInfo].operatingSystemVersionString UTF8String ]);
	environment.SetValue ( MOAI_ENV_resourceDirectory,	[[[ NSBundle mainBundle ] resourcePath ] UTF8String ]);
	//TODO: dpi can be based on model.
//	environment.SetValue ( MOAI_ENV_screenDpi,			100);
//	environment.SetValue ( MOAI_ENV_horizontalResolution, [[ UIScreen mainScreen ] bounds ].size.width * [[ UIScreen mainScreen ] scale ] );
//	environment.SetValue ( MOAI_ENV_verticalResolution, [[ UIScreen mainScreen ] bounds ].size.height * [[ UIScreen mainScreen ] scale ] );
//	environment.SetValue ( MOAI_ENV_udid,				[[SFSAkuInit MacOSXUUID] UTF8String]);
//	environment.SetValue ( MOAI_ENV_openUdid,			[[ MOAIOpenUDID value] UTF8String ]);
}

@end
