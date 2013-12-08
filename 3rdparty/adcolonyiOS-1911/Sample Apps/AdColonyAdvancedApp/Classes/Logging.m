//
//  Logging.m
//  SampleAdColonyApp
//
//  Created by John Fernandes-Salling on 10/20/10.
//  Copyright 2010 Jirbo, INC. All rights reserved.
//

#import "Logging.h"

void logMessage(NSString* message)
{
	NSDictionary *infoDict = [NSDictionary dictionaryWithObject:message forKey:@"log"];
	[[NSNotificationCenter defaultCenter] postNotificationName:@"newLogStatement" object:infoDict];
}