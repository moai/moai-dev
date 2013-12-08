//
//  FolderWatcher.m
//

#import "FolderWatcher-mac.h"
#import "GlutHost.h"

#import <Foundation/Foundation.h>
#import <stdio.h>
#import <libkern/OSAtomic.h>
#import <pthread.h>
#import <CoreServices/CoreServices.h>

/*-------------------------------------------------------------------
 * XCode 3 / LLVM <3.0 compatibility macros
 --------------------------------------------------------------------*/
#ifdef __clang__
  #ifdef __has_feature(objc_arc)
    #define ARC_POOL_START @autoreleasepool {
  #else
    #define ARC_POOL_START NSAutoreleasePool *pool = [NSAutoreleasePool new];
  #endif
#else
  #define ARC_POOL_START NSAutoreleasePool *pool = [NSAutoreleasePool new];
#endif

#ifdef __clang__
  #ifdef __has_feature(objc_arc)
    #define ARC_POOL_END }
  #else
    #define ARC_POOL_END [pool drain];
  #endif
#else
  #define ARC_POOL_END [pool drain];
#endif

#ifndef UNUSED
#define UNUSED(x)	((void)(x))	/* to avoid warnings */
#endif

/*-------------------------------------------------------------------
 FolderWatch API implementation
 --------------------------------------------------------------------*/

// Which directories are we interested in?
static NSMutableArray* pathsToWatch;

// Which files have been changed so far?
static OSSpinLock changedFilesLock;
static NSMutableArray* changedFiles;

// How do we figure out if files have been changed?
static NSDate* startupTimestamp;
static NSMutableDictionary* pathModificationDates;


static pthread_t watcherThread;
static FSEventStreamRef eventStream;
static CFRunLoopRef runLoop;
static uint64_t lastEventID = kFSEventStreamEventIdSinceNow;
static bool threadStateReadyToBeStopped = false;


static void updateLastModificationDate(NSString* path)
{
	[pathModificationDates setObject:[NSDate date] forKey:path];	
}

static NSDate* lastModificationDate(NSString* path)
{
	NSDate* lastModTime = [pathModificationDates valueForKey:path];
	if(nil != lastModTime)
		return lastModTime;
	else
		return startupTimestamp;
}

static void addModifiedFiles(NSString* baseDir)
{
	NSFileManager* fm = [[NSFileManager alloc] init];
	NSArray* contents = [fm contentsOfDirectoryAtPath:baseDir error:nil];
	NSString* fullPath = nil;
	
	for(NSString* node in contents) {
        fullPath = [baseDir stringByAppendingPathComponent: node];
		
		NSDictionary *fileAttributes = [fm attributesOfItemAtPath:fullPath error:NULL];
		NSDate *fileModDate = [fileAttributes objectForKey:NSFileModificationDate];
		if([fileModDate compare: lastModificationDate(baseDir)] == NSOrderedDescending) {
			[changedFiles addObject: fullPath];
		}
	}
	
	updateLastModificationDate(baseDir);
}

static void fseventsCallback(ConstFSEventStreamRef streamRef,
					  void* userData,
					  size_t numEvents,
					  void* eventPaths,
					  const FSEventStreamEventFlags eventFlags[],
					  const FSEventStreamEventId eventIds[])
{
	UNUSED ( streamRef );
	UNUSED ( userData );
	UNUSED ( eventFlags );
	
	// Some files we are interested in have been changed
	// Record them for processing later
	
	OSSpinLockLock(&changedFilesLock);
	
	for(uint i = 0; i < numEvents; i++)
	{
		addModifiedFiles([(NSArray*)eventPaths objectAtIndex:i]);
	}
	
	lastEventID = eventIds[numEvents-1];		
	
	OSSpinLockUnlock(&changedFilesLock);	
}

static void FWUtilStartEventStream()
{
	// The lower the number, the faster an file modification event will be triggered
	// after it is observed
	NSTimeInterval latency = 0.5;
	
	eventStream = FSEventStreamCreate(NULL,
									  &fseventsCallback,
									  NULL,
									  (CFArrayRef) pathsToWatch,
									  lastEventID,
									  (CFAbsoluteTime) latency,
									  kFSEventStreamCreateFlagUseCFTypes 
									  );
	
	runLoop = CFRunLoopGetCurrent();
	
	FSEventStreamScheduleWithRunLoop(eventStream, runLoop, kCFRunLoopDefaultMode);
	FSEventStreamStart(eventStream);
}

// Stops the event stream
// This will also cause for the thread to stop
static void FWUtilStopEventStream()
{
	// If a thread isn't even running, to watch the event stream
	// Neither the event stream nor the thread needs to be stopped
	if(!watcherThread)
		return;
	
	// The watcher thread is running...
	// It's possible that the watch thread haven't started the event stream
	// or scheduled with the run loop yet.
	// If it's not ready to be stopped, wait until it is.
	while(!threadStateReadyToBeStopped)
		pthread_yield_np();
	
	FSEventStreamStop(eventStream);
	FSEventStreamUnscheduleFromRunLoop(eventStream, runLoop, kCFRunLoopDefaultMode);
	CFRunLoopWakeUp(runLoop);
	
	// Unscheduling from the run loop should cause the thread to exit
	// Wait for this to happen so we only ever have one watcher thread
	int retVal = pthread_join(watcherThread, NULL);
	assert(0 == retVal);
	
	watcherThread = 0;
	eventStream = 0;
	runLoop = 0;
	threadStateReadyToBeStopped = false;
}

static void* FWWatchThreadMain(void*)
{
	ARC_POOL_START
		FWUtilStartEventStream();
		threadStateReadyToBeStopped = true;
		CFRunLoopRun();
	ARC_POOL_END
	
	return 0;
}

static void FWLazyInit()
{
	if(!pathsToWatch)
		pathsToWatch = [[NSMutableArray alloc] init];
	if(!changedFiles)
		changedFiles = [[NSMutableArray alloc] init];
	if(!startupTimestamp)
		startupTimestamp = [[NSDate date] retain];
	if(!pathModificationDates)
		pathModificationDates = [[NSMutableDictionary alloc] init];
}

static void FWStartWatcherThread()
{
	int retVal = pthread_create(&watcherThread, NULL, FWWatchThreadMain, NULL);
	assert(0 == retVal);
}

void FWStart(const char* path)
{
	if(!path)
		return;
	
	ARC_POOL_START	
		FWLazyInit();

		NSString* newPath = [NSString stringWithUTF8String: path];

		// Don't do anything if the incoming path is not a valid path
		NSFileManager* fm = [[[NSFileManager alloc] init] autorelease];
		if(![fm fileExistsAtPath: newPath])
			return;

		// Don't do anything if the path is already being watched
		if([pathsToWatch containsObject: newPath])
			return;
		
		[pathsToWatch addObject: newPath];
		
		FWUtilStopEventStream();	// Also causes thread to stop running
		FWStartWatcherThread();
	ARC_POOL_END
}

void FWStop(const char* path)
{
	if(!path)
		return;
	
	ARC_POOL_START
		FWLazyInit();
		
		NSString* pathToRemove = [NSString stringWithUTF8String:path];
		[pathsToWatch removeObject: pathToRemove];
		
		FWUtilStopEventStream();
		FWStartWatcherThread();
	ARC_POOL_END
}

void FWStopAll(void)
{
	ARC_POOL_START
		FWLazyInit();
		
		FWUtilStopEventStream();
		
		[pathsToWatch removeAllObjects];
	ARC_POOL_END
}

char* FWEnumChangedFile(void)
{
	ARC_POOL_START
		FWLazyInit();
		
		// Is there anything in the changed files array?
		// If not, we're done.
		if([changedFiles count] == 0)
			return nil;
		
		// Otherwise... return a single changed file to the caller
		
		OSSpinLockLock(&changedFilesLock);
		
		char* path = strdup([[changedFiles objectAtIndex: 0] UTF8String]);
		[changedFiles removeObjectAtIndex: 0];
		
		OSSpinLockUnlock(&changedFilesLock);
		
		return path;
	ARC_POOL_END
}

/*-------------------------------------------------------------------
	Convenience Functions
 --------------------------------------------------------------------*/

#import <aku/AKU.h>

void FWWatchFolder(const char* filename)
{
	if(!filename)
		return;
	
	NSFileManager* fm = [[NSFileManager alloc] init];
	BOOL isDirectory = NO;
	BOOL fileExists = NO;
	
	// Retrieve the absolute path of the specified filename
	NSString* filePath = [NSString stringWithUTF8String: filename];
	if(![filePath isAbsolutePath])
	{
		filePath = [[fm currentDirectoryPath] stringByAppendingPathComponent: filePath];
		filePath = [filePath stringByStandardizingPath];
	}

	// Check if the file exists
	// If not, just give up
	fileExists = [fm fileExistsAtPath: filePath isDirectory: &isDirectory];
	if(!fileExists)
		return;

	// If we're already looking at a directory, then just start watching it
	// Otherwise, figure out the path of the containing directory and watch that
	NSString* baseDir;
	if(isDirectory)
		baseDir = filePath;
	else
		baseDir = [filePath stringByDeletingLastPathComponent];

	printf("Watching %s\n", [baseDir UTF8String]);
	FWStart([baseDir UTF8String]);
	
	[fm release];
}


void FWReloadChangedLuaFiles(void)
{
	char* filename;
	while((filename = FWEnumChangedFile()) != NULL)
	{
		NSString* fileExtension = [[NSString stringWithUTF8String: filename] pathExtension];
		if([fileExtension caseInsensitiveCompare: @"lua"] == NSOrderedSame)
		{
			GlutRefreshContext();
			AKURunScript(filename);
			printf("%s reloaded.\n", filename);
		}
		free(filename);
	}
}

