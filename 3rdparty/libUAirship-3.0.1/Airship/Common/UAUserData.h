
#import <Foundation/Foundation.h>

/**
 * Model object for holding user data.
 */
@interface UAUserData : NSObject

/**
 * Autoreleased UAUserData class factory method.
 *
 * @param username The associated user name.
 * @param password The associated user password.
 * @param url The associated user url, as a string.
 */
+ (id)dataWithUsername:(NSString *)username password:(NSString *)password url:(NSString *)url;

/**
 * UAUserData initializer.
 *
 * @param username The associated user name.
 * @param password The associated user password.
 * @param url The associated user url, as a string.
 */
- (id)initWithUsername:(NSString *)username password:(NSString *)password url:(NSString *)url;

/**
 * The user name.
 */
@property(nonatomic, readonly, copy) NSString *username;
/**
 * The password.
 */
@property(nonatomic, readonly, copy) NSString *password;
/**
 * The url as a string.
 */
@property(nonatomic, readonly, copy) NSString *url;

@end
