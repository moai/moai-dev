//
//  SFSCodecError.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

@interface SFSCodecException : NSException {
	
}

+(id)exceptionWithReason:(NSString *)reason;

@end
