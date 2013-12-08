//
//  SoundManager.m
//  DropChopper
//
//  Created by Michael Malin on 2/25/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//
// blah
#import "SoundManager.h"


@implementation SoundManager

@synthesize shouldPlayMusicDefaultsKey;
@synthesize delegate;

- (id)init
{
    if ((self = [super init])) {
		currentlyShouldPlay = YES; //default is to play
		//*
		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(defaultsChanged:)
													 name:NSUserDefaultsDidChangeNotification
												   object:nil];//*/
	}
	return self;
}

- (void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	
	[shouldPlayMusicDefaultsKey release];
		
	[super dealloc];
}

#pragma mark music

- (void)loadMusicTrackWithFile:(NSString *)fileName repeats:(int)repeats
{
	if(shouldPlayMusicDefaultsKey){
		if([[NSUserDefaults standardUserDefaults] objectForKey:shouldPlayMusicDefaultsKey]){
			currentlyShouldPlay = [[[NSUserDefaults standardUserDefaults] objectForKey:shouldPlayMusicDefaultsKey] boolValue];
		}
	}
		
	if (!fileName) {
		return;
	}
	
	if ([fileName isEqual:currentMusicFile] && musicPlayer.playing) {
		return;
	}
	
	if (musicPlayer) {
		if (musicPlayer.playing) {
			[musicPlayer stop];
		}
		
		[musicPlayer release];
		musicPlayer = nil;
	}
	
	[currentMusicFile release];
	currentMusicFile = nil;
		
	//create a new instance of the music player
	NSError *error;
	musicPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:[NSURL fileURLWithPath:fileName] error:&error];
	if (error || !musicPlayer) {
		if ([delegate respondsToSelector:@selector(soundManager:failedToLoadMusicWithFile:withError:)]) {
			[delegate soundManager:self failedToLoadMusicWithFile:fileName withError:error];
		}
		return;
	}
	
	//update the current music file to reflect the currently playing track 
	currentMusicFile = [fileName copy];
	
	musicPlayer.delegate = self;
	
	// -1 for infinite loop
	musicPlayer.numberOfLoops = repeats;
}

#pragma mark defaults changed

- (void)defaultsChanged:(NSNotification *)notification
{	
	BOOL newShouldPlay = currentlyShouldPlay;
	if(shouldPlayMusicDefaultsKey){
		if([[NSUserDefaults standardUserDefaults] objectForKey:shouldPlayMusicDefaultsKey]){
			newShouldPlay = [[[NSUserDefaults standardUserDefaults] objectForKey:shouldPlayMusicDefaultsKey] boolValue];
		}
	}
	
	if(newShouldPlay != currentlyShouldPlay){
		if(newShouldPlay == YES){
			if(!bgIsPaused && !musicPlayer.playing){
				[musicPlayer play];
			}
		}else {
			[musicPlayer stop];
		}

		currentlyShouldPlay = newShouldPlay;
	}
}

#pragma mark AVAudioPlayerDelegate

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
	NSLog(@"finished playing");
	if ([delegate respondsToSelector:@selector(soundManager:finishedPlayingMusicWithFile:)]) {
		[delegate soundManager:self finishedPlayingMusicWithFile:currentMusicFile];
	}
}

- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error
{
	NSLog(@"decode error: %@", error);
	if ([delegate respondsToSelector:@selector(soundManager:failedToDecodeMusicFile:withError:)]) {
		[delegate soundManager:self failedToDecodeMusicFile:currentMusicFile withError:error];
	}
}

- (void)audioPlayerBeginInterruption:(AVAudioPlayer *)player
{
	NSLog(@"Audio player interrupted");
	if (player.playing) {
		[player stop];
	}
}

- (void)audioPlayerEndInterruption:(AVAudioPlayer *)player
{
	NSLog(@"Audio player interruption finished");
	if ([self shouldPlayMusic]){
		[player play];
	}
}

- (BOOL)shouldPlayMusic
{
	return currentlyShouldPlay;
}

-(void)pauseMusic{
	//NSLog(@"current audio: %@", [[AVAudioSession sharedInstance] category]);
	if([musicPlayer isPlaying]){
		bgIsPaused = YES;
		NSLog(@"music player will stop: %@ %f %@", musicPlayer, [musicPlayer currentTime], [musicPlayer data]);
		[musicPlayer stop];
		NSLog(@"music player has stoped: %@ %f %@", musicPlayer, [musicPlayer currentTime], [musicPlayer data]);

	}
}

-(void)playMusic{
	if([self shouldPlayMusic]){
		NSLog(@"music player will resume %@ %f %@", musicPlayer, [musicPlayer currentTime], [musicPlayer data]);
		NSLog(@"player successfully resumed: %d", [musicPlayer play]);
	}
	bgIsPaused = NO;
}

-(BOOL)currentlyPlayingBGMusic{
	return musicPlayer.playing;
}

-(void)resetMusic{
	if(!currentMusicFile || [self currentlyPlayingBGMusic] || ![self shouldPlayMusic]){
		return;
	}
	[musicPlayer release];
	musicPlayer = nil;
	
	musicPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:[NSURL fileURLWithPath:currentMusicFile] error:NULL];

	if(!musicPlayer){
		return;
	}
	
	musicPlayer.delegate = self;
	
	// -1 for infinite loop
	musicPlayer.numberOfLoops = -1;
	[musicPlayer play];
}

@end
