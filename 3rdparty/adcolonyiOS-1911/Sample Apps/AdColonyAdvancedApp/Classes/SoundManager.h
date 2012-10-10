//
//  SoundManager.h
//  DropChopper
//
//  Created by Michael Malin on 2/25/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@class SoundManager;

@protocol SoundManagerDelegate <NSObject>

@optional
//music
- (void)soundManager:(SoundManager *)soundManager failedToLoadMusicWithFile:(NSString *)fileName withError:(NSError *)error;
- (void)soundManager:(SoundManager *)soundManager failedToDecodeMusicFile:(NSString *)fileName withError:(NSError *)error;
- (void)soundManager:(SoundManager *)soundManager finishedPlayingMusicWithFile:(NSString *)fileName;

@end


@interface SoundManager : NSObject <AVAudioPlayerDelegate> {
	//the music player object
	AVAudioPlayer *musicPlayer;
	
	//the currently playing track
	NSString *currentMusicFile;
	BOOL bgIsPaused;
	BOOL currentlyShouldPlay;
		
	//defaults keys to lookup
	NSString *shouldPlayMusicDefaultsKey;
	
	id<SoundManagerDelegate> delegate;
}


//*****************************************//
//************ BACKGROUND MUSIC************//
//*****************************************//

//play track
- (void)loadMusicTrackWithFile:(NSString *)fileName repeats:(int)repeats;

//defaults
- (void)defaultsChanged:(NSNotification *)notification;
- (BOOL)shouldPlayMusic;

//properties

-(void)pauseMusic;
-(void)playMusic;
-(void)resetMusic;
-(BOOL)currentlyPlayingBGMusic;

@property (nonatomic, copy) NSString *shouldPlayMusicDefaultsKey;
@property (nonatomic, assign) id<SoundManagerDelegate> delegate;


@end
