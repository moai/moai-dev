//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCVideoManager.h"
#import "TJCVideoRequestHandler.h"
#import "TJCVideoView.h"
#import "TapjoyConnectConstants.h"
#import "TJCVideoViewHandler.h"
#import "TJCConstants.h"


static TJCVideoManager *sharedTJCVideoManager_ = nil;

@implementation TJCVideoManager

@synthesize videoView = videoView_, requestHandler = requestHandler_, currentOrientation = currentOrientation_;


+ (TJCVideoManager*)sharedTJCVideoManager
{
	if (!sharedTJCVideoManager_)
	{
		sharedTJCVideoManager_ = [[super alloc] init];
	}
	
	return sharedTJCVideoManager_;
}


- (id)init
{
	self = [super init];
	
	if (self) 
	{
		requestHandler_ = [[TJCVideoRequestHandler alloc] initRequestWithDelegate:self andRequestTag:0];	
	}
	
	return self;
}


- (void)initVideoAdsWithDelegate:(id<TJCVideoAdDelegate>)delegate
{
	downloadIndex_ = 0;
	videoCacheCount_ = TJC_VIDEO_CACHE_COUNT;
	
	// Initiate video request.
	[requestHandler_ requestVideoData];
	
	// Since this can be called more than once to initiate video caching at any point, make sure we do unnecessary allocation.
	// We also don't want to release and reallocate here since it would possibly screw up any currently playing video.
	if (!videoView_)
	{
		videoView_ = [[TJCVideoView alloc] initWithDelegate:delegate];
	}
}


- (NSArray*)getCachedVideoIDs
{
	NSString *cachesDirectory = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) objectAtIndex:0];
	NSString *videoDirectory = [cachesDirectory stringByAppendingFormat:@"/VideoAds"];
	NSArray *dirContents = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:videoDirectory error:nil];

	NSMutableArray *formattedContents = [[[NSMutableArray alloc] init] autorelease];
	
	for (NSString *fileName in dirContents)
	{
		NSString *formattedName = [fileName stringByReplacingOccurrencesOfString:@".mp4" withString:@""];
		[formattedContents addObject:formattedName];
	}
	
	return formattedContents;
}


- (NSMutableDictionary*)getCachedVideoDictonary
{
	return [[NSUserDefaults standardUserDefaults] objectForKey:TJC_CACHED_VIDEO_DICT];
}


- (void)setCachedVideoObjectDict:(NSDictionary*)videoObjectDict withKey:(NSString*)key;
{
	NSMutableDictionary *cachedDict = [[NSMutableDictionary alloc] initWithDictionary:[self getCachedVideoDictonary]];
	
	[cachedDict setObject:videoObjectDict forKey:key];
	[[NSUserDefaults standardUserDefaults] setObject:cachedDict forKey:TJC_CACHED_VIDEO_DICT];
	
	[cachedDict release];
}


- (void)fetchResponseSuccessWithData:(void*)dataObj withRequestTag:(int)aTag
{
	TBXMLElement *videoElement = [TBXML childElementNamed:@"TapjoyVideo" parentElement:dataObj];
	
	// Temporary dictionary for new video objects, which will be compared to locally saved one to make sure we clear out old videos from the cache.
	NSMutableDictionary *videoDict = [[NSMutableDictionary alloc] init];
	
	// Get all video object from xml.
	while (videoElement)
	{
		// This will effectively initiate a request to download the video ad.
		TJCVideoObject *videoObj = [[TJCVideoObject alloc] initWithTBXML:videoElement];
		
		// Store the video object using the offer ID as the key.
		[videoDict setObject:[videoObj getDictionary] forKey:videoObj.offerID];
		
		[videoObj release];
		
		videoElement = [TBXML nextSiblingNamed:@"TapjoyVideo" searchFromElement:videoElement];
	}
	
	// Now clear out any old videos, ones that are not part of the list received.
	for (NSString *videoObjKey in [self getCachedVideoIDs])
	{
		// Get the video object from the cached dictionary.
		NSDictionary *videoObjDict = [[self getCachedVideoDictonary] objectForKey:videoObjKey];
		
		// Create a temporary video object dictionary from the newly received list from the server using the cached offer ID as the key.
		NSString *videoOfferID = [videoObjDict objectForKey:TJC_VIDEO_OBJ_OFFER_ID];
		NSMutableDictionary *tmpObjDict = [[NSMutableDictionary alloc] initWithDictionary:[videoDict objectForKey:videoOfferID]];
		
		// If the video exists in the newly received list, it's still valid. Skip to next one.
		if ((tmpObjDict) && ([tmpObjDict count] > 0))
		{
			NSString *cachesDirectory = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) objectAtIndex:0];
			// Set video file path.
			[tmpObjDict setObject:[cachesDirectory stringByAppendingFormat:@"/VideoAds/%@", [tmpObjDict objectForKey:TJC_VIDEO_OBJ_FILENAME]] 
								  forKey:TJC_VIDEO_OBJ_DATA_LOCATION];
			// Save to user defaults to be pulled up later for app restart.
			[self setCachedVideoObjectDict:tmpObjDict withKey:[tmpObjDict objectForKey:TJC_VIDEO_OBJ_OFFER_ID]];
			
			// Remove this object from uncached dictionary.
			[videoDict removeObjectForKey:[videoObjDict objectForKey:TJC_VIDEO_OBJ_OFFER_ID]];
			[tmpObjDict release];
			continue;
		}
		else
		{			
			// Object not in newly received list, delete the cached video file.
			NSError *error;
			[[NSFileManager defaultManager] removeItemAtPath:[videoObjDict objectForKey:TJC_VIDEO_OBJ_DATA_LOCATION] error:&error];
			
			// Now remove the video object from cached video dictionary and re-save it.
			NSMutableDictionary *cachedVideoDict = [[NSMutableDictionary alloc] initWithDictionary:[self getCachedVideoDictonary]];
			if (videoObjDict)
			{
				NSString *videoOfferID = [videoObjDict objectForKey:TJC_VIDEO_OBJ_OFFER_ID];
				if (videoOfferID)
				{
					// Clean up here by removing the video object since it's stale.
					[cachedVideoDict removeObjectForKey:videoOfferID];
				}
			}

			// Re-save the cached video dictionary.
			[[NSUserDefaults standardUserDefaults] setObject:cachedVideoDict forKey:TJC_CACHED_VIDEO_DICT];
			[cachedVideoDict release];
		}
		[tmpObjDict release];
	}
	
	// Save uncached dictionary.
	[[NSUserDefaults standardUserDefaults] setObject:videoDict forKey:TJC_UNCACHED_VIDEO_DICT];
	
	// Check if caching is necessary at this point.
	if ([[self getCachedVideoDictonary] count] < videoCacheCount_)
	{
		// Save uncached list.
		if (unCachedVideoObjects_)
		{
			[unCachedVideoObjects_ release];
		}
		unCachedVideoObjects_ = [[videoDict allValues] retain];
		
		downloadIndex_ = 0;
		[self beginVideoCaching];
	}
	
	[videoDict release];
}


- (void)fetchResponseError:(TJCResponseError)errorType errorDescription:(id)errorDescObj requestTag:(int)aTag
{
	[TJCLog logWithLevel: LOG_NONFATAL_ERROR
					  format: @"%s: %d; %s; fetch repsonse error", __FILE__, __LINE__, __PRETTY_FUNCTION__];
}


- (void)beginVideoCaching
{
    [TJCLog logWithLevel:LOG_DEBUG format:@"TJCVideoManager: Video caching begun with download index:%d", downloadIndex_];
    
	if (connection_)
	{
		[connection_ release];
	}
	if (videoData_)
	{
		[videoData_ release];
	}
	
	// Pull video object to get video URL to be downloaded.
	if ([unCachedVideoObjects_ count] > 0)
	{
		NSDictionary *videoObjDict = [unCachedVideoObjects_ objectAtIndex:downloadIndex_];
		
		NSURL *url = [NSURL URLWithString:[videoObjDict objectForKey:TJC_VIDEO_OBJ_VIDEO_URL]];
		NSURLRequest *request = [NSURLRequest requestWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:30.0];
		connection_ = [[NSURLConnection alloc] initWithRequest:request delegate:self];
	}
}


- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	[TJCLog logWithLevel: LOG_NONFATAL_ERROR
					  format: @"%s: %d; %s; video download failure: %@", __FILE__, __LINE__, __PRETTY_FUNCTION__, [error localizedFailureReason]];
}


- (void)connection:(NSURLConnection*)theConnection didReceiveData:(NSData*)incrementalData 
{
	if (!videoData_) 
	{
		videoData_ = [[NSMutableData alloc] init];
	}
	[videoData_ appendData:incrementalData];
}


- (void)connectionDidFinishLoading:(NSURLConnection*)theConnection 
{
	NSMutableDictionary *videoObjDict = [NSMutableDictionary dictionaryWithDictionary:[unCachedVideoObjects_ objectAtIndex:downloadIndex_]];
	
	[connection_ release];
	connection_ = nil;
	
	NSString *docsDirectory = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) objectAtIndex:0];
	
	// Set video file path.
	NSString *pathDir = [docsDirectory stringByAppendingFormat:@"/VideoAds"];
	[videoObjDict setObject:[docsDirectory stringByAppendingFormat:@"/VideoAds/%@", [videoObjDict objectForKey:TJC_VIDEO_OBJ_FILENAME]] 
						  forKey:TJC_VIDEO_OBJ_DATA_LOCATION];
	
	NSError *error;
	if (![[NSFileManager defaultManager] createDirectoryAtPath:pathDir withIntermediateDirectories:YES attributes:nil error:&error])
	{
		[TJCLog logWithLevel: LOG_NONFATAL_ERROR
						  format: @"%s: %d; %s; cannot write to file: %@", __FILE__, __LINE__, __PRETTY_FUNCTION__, [error localizedFailureReason]];
		return;
	}
	
	// Save video file to local path.
	[videoData_ writeToFile:[videoObjDict objectForKey:TJC_VIDEO_OBJ_DATA_LOCATION] atomically:YES];
	
	// Save to user defaults to be pulled up later for app restart.
	[self setCachedVideoObjectDict:videoObjDict withKey:[videoObjDict objectForKey:TJC_VIDEO_OBJ_OFFER_ID]];
	
	[videoData_ release];
	videoData_ = nil;
	
	// Increment index and see if we need to cache next video.
	downloadIndex_++;
	if ((downloadIndex_ < [unCachedVideoObjects_ count]) && ([[self getCachedVideoDictonary] count] < videoCacheCount_))
	{
		// Download next video.
		[self beginVideoCaching];
	}
}


- (void)setVideoCacheCount:(int)count
{
	if (count > 0)
	{
		videoCacheCount_ = count;
	}
}


- (void)updateViewWithOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	if ((interfaceOrientation == UIInterfaceOrientationLandscapeLeft) || (interfaceOrientation == UIInterfaceOrientationLandscapeRight))
	{
		[videoView_ refreshViewWithInterfaceOrientation:interfaceOrientation];
	}
}


- (void)dealloc
{
	[unCachedVideoObjects_ release];
	[connection_ release];
	[videoData_ release];
	[requestHandler_ release];
	[videoView_ release];
	[super dealloc];
}


@end
