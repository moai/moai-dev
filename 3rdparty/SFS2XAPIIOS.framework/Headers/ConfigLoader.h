//
//  ConfigLoader.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "IConfigLoader.h"
#import "XMLParser.h"

@interface ConfigLoader : NSObject {

}

/*
 By default, loads a file called sfs-config.xml located in the same
 folder as the application
 */
-(void)loadConfig:(NSString *)filePath delegate:(id<IConfigLoader>)delegate;


@end
