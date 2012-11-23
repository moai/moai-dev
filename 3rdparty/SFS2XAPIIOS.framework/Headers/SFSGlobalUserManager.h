//
//  SFSGlobalUserManager.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "SFSUserManager.h"

@interface SFSGlobalUserManager : SFSUserManager {
@private
	NSMutableDictionary *_roomRefCount;
}


-(id)initWithSfs:(SmartFox2XClient *)sfs;

@end
