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
#import <AppKit/NSScreen.h>
#import <CoreGraphics/CGDisplayConfiguration.h>

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

+(char*)GetCharSYSCTL:(NSString*)name {
	size_t len = 0;
    sysctlbyname([name UTF8String], NULL, &len, NULL, 0);
	char *value;
    if (len) {
		value = (char*)malloc(len * sizeof(char));
        sysctlbyname([name UTF8String], value, &len, NULL, 0);
		return value;
	} else {
		return (char*)"unknown";
	}
}

+(uint64_t)GetIntSYSCTL:(NSString*)name {
	size_t len = sizeof(uint64_t);
	uint64_t value = 0;
    sysctlbyname([name UTF8String], &value, &len, NULL, 0);
	return value;
}

+(void)MoaiEnvironmentInit {
	MOAIEnvironment& environment = MOAIEnvironment::Get ();

	
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
	environment.SetValue ( MOAI_ENV_devModel,			[SFSAkuInit GetCharSYSCTL:@"hw.model"] );
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
	NSSize mainScreenSize = [NSScreen mainScreen].frame.size;
	environment.SetValue ( MOAI_ENV_desktopRes, [[NSString stringWithFormat:@"%fx%f", mainScreenSize.width, mainScreenSize.height] UTF8String]);
//	environment.SetValue ( MOAI_ENV_udid,				[[SFSAkuInit MacOSXUUID] UTF8String]);
//	environment.SetValue ( MOAI_ENV_openUdid,			[[ MOAIOpenUDID value] UTF8String ]);
	
	environment.SetValue ( MOAI_ENV_processorModel, [SFSAkuInit GetCharSYSCTL:@"machdep.cpu.brand_string"]);
	environment.SetValue ( MOAI_ENV_processorFreq, (uint64_t)[SFSAkuInit GetIntSYSCTL:@"hw.cpufrequency"] / 1000000);
	environment.SetValue ( MOAI_ENV_ramAmount, (uint64_t)[SFSAkuInit GetIntSYSCTL:@"hw.memsize"] / 1048576);
	environment.SetValue ( MOAI_ENV_screenCount, [NSScreen screens].count);

	CGSize measure = CGDisplayScreenSize((CGDirectDisplayID)[[[NSScreen mainScreen].deviceDescription objectForKey:@"NSScreenNumber"] pointerValue]);
	CGSize dpi = CGSizeMake(mainScreenSize.width/(measure.width * 0.0393700787), mainScreenSize.height/(measure.height * 0.0393700787));
	if (dpi.width > dpi.height) {
		environment.SetValue ( MOAI_ENV_screenDpi, dpi.width);
	} else {
		environment.SetValue ( MOAI_ENV_screenDpi, dpi.height);
	}
}

@end
