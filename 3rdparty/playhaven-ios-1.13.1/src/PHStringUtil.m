/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Copyright 2013 Medium Entertainment, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 NSStringUtil.m
 playhaven-sdk-ios

 Created by Kurtiss Hare on 2/12/10.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PHStringUtil.h"
#import <CommonCrypto/CommonDigest.h>

typedef struct {
    unichar character;
    NSString *entity;
} HTMLEntityPair;

static HTMLEntityPair gEntityTable[] = {
    // C0 Controls and Basic Latin
    { 34, @"&quot;" },
    { 38, @"&amp;" },
    { 39, @"&apos;" },
    { 60, @"&lt;" },
    { 62, @"&gt;" },

    // Latin Extended-A
    { 338, @"&OElig;" },
    { 339, @"&oelig;" },
    { 352, @"&Scaron;" },
    { 353, @"&scaron;" },
    { 376, @"&Yuml;" },

    // Spacing Modifier Letters
    { 710, @"&circ;" },
    { 732, @"&tilde;" },

    // General Punctuation
    { 8194, @"&ensp;" },
    { 8195, @"&emsp;" },
    { 8201, @"&thinsp;" },
    { 8204, @"&zwnj;" },
    { 8205, @"&zwj;" },
    { 8206, @"&lrm;" },
    { 8207, @"&rlm;" },
    { 8211, @"&ndash;" },
    { 8212, @"&mdash;" },
    { 8216, @"&lsquo;" },
    { 8217, @"&rsquo;" },
    { 8218, @"&sbquo;" },
    { 8220, @"&ldquo;" },
    { 8221, @"&rdquo;" },
    { 8222, @"&bdquo;"  },
    { 8224, @"&dagger;" },
    { 8225, @"&Dagger;" },
    { 8240, @"&permil;" },
    { 8249, @"&lsaquo;" },
    { 8250, @"&rsaquo;" },
    { 8364, @"&euro;" },
};

static const char *gBase64MappingWebSafe = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

static int CompareEntityPairs(const void *voidCharacter, const void *voidEntityTable)
{
    const unichar        *character   = (const unichar *) voidCharacter;
    const HTMLEntityPair *entityTable = (const HTMLEntityPair *) voidEntityTable;
    int result;

    if (*character < entityTable->character) {
        result = -1;
    } else if (*character > entityTable->character) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

@implementation PHStringUtil

// NOTE: Left intact for backwards-quirkiness.
// Any service-facing input should continue to use this method,
// so as not to encourage backwards-incompatible service behavior.
+ (NSString *)stringWithQueryQuirky:(NSDictionary *)params
{
    NSArray         *keys  = [params allKeys];
    NSMutableString *query = [NSMutableString string];

    NSUInteger i, n = [keys count];
    for (i = 0; i < n; i++) {
        NSString *key = [keys objectAtIndex:i];
        id item = [params objectForKey:key];

        if (![item isKindOfClass:[NSString class]])
        {
            if ([item respondsToSelector:@selector(stringValue)]) {
                item = [item stringValue];
            }
            else {
                item = (NSString *) item;
            }
        }

        [query appendFormat:@"%@=%@", key, [PHStringUtil stringByUrlEncodingString:item]];

        if (i < n - 1) [query appendString:@"&"];
    }

    return query;
}

+ (NSString *)stringWithQuery:(NSDictionary *)params
{
    NSArray         *keys  = [params allKeys];
    NSMutableString *query = [NSMutableString string];

    NSUInteger i, n = [keys count];
    for (i = 0; i < n; i++) {
        NSString *key = [keys objectAtIndex:i];
        NSString *stringValue = nil;

        id value = [params objectForKey:key];

        if (value != [NSNull null]) {
            if ([value respondsToSelector:@selector(stringValue)]) {
                stringValue = [value stringValue];
            }
            else {
                stringValue = (NSString *) value;
            }

            stringValue = [PHStringUtil stringByUrlEncodingString:stringValue];
        }
        else {
            stringValue = @"";
        }

        [query appendFormat:@"%@=%@", key, stringValue];

        if (i < n - 1) [query appendString:@"&"];
    }

    return query;
}

+ (NSString *)stringByHtmlEscapingString:(NSString *)input
{
    NSUInteger inputLength = [input length];

    if (!inputLength) {
        return input;
    }

    NSMutableString *result      = [NSMutableString string];
    NSMutableData   *outputData  = [NSMutableData dataWithCapacity:sizeof(unichar) * inputLength];
    const unichar   *inputBuffer = CFStringGetCharactersPtr((CFStringRef) input);

    if (!inputBuffer) {
        NSMutableData *inputData = [NSMutableData dataWithLength:inputLength * sizeof(UniChar)];

        if (inputData) {
            [input getCharacters:[inputData mutableBytes]];
            inputBuffer = [inputData bytes];
        }
    }

    if (!inputBuffer || !outputData) {
        return nil;
    }

    unichar *outputBuffer = (unichar *)[outputData mutableBytes];
    NSUInteger outputBufferLength = 0;

    for (NSUInteger i = 0; i < inputLength; ++i) {
        HTMLEntityPair *pair = bsearch(&inputBuffer[i], gEntityTable,
                                       sizeof(gEntityTable) / sizeof(HTMLEntityPair),
                                       sizeof(HTMLEntityPair), CompareEntityPairs);

        if (pair || inputBuffer[i] > 127) {
            if (outputBufferLength) {
                CFStringAppendCharacters((CFMutableStringRef)result,
                                         outputBuffer,
                                         outputBufferLength);
                outputBufferLength = 0;
            }

            if (pair) {
                [result appendString:pair->entity];
            } else {
                [result appendFormat:@"&#%d;", inputBuffer[i]];
            }
        } else {
            outputBuffer[outputBufferLength] = inputBuffer[i];
            outputBufferLength += 1;
        }
    }

    if (outputBufferLength) {
        CFStringAppendCharacters((CFMutableStringRef)result,
                                 outputBuffer,
                                 outputBufferLength);
    }

    return result;
}

+ (NSString *)stringByUrlEncodingString:(NSString *)input
{
    CFStringRef value = CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault,
                                                                (CFStringRef) input,
                                                                NULL,
                                                                (CFStringRef) @"!*'();:@&=+$,/?%#[]",
                                                                kCFStringEncodingUTF8);

    NSString *result = [NSString stringWithString:(NSString *)value];
    CFRelease(value);

    return result;
}

+ (NSString *)stringByUrlDecodingString:(NSString *)input
{
    return [input stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
}

+ (NSString *)uuid
{
    CFUUIDRef   uuid    = CFUUIDCreate(kCFAllocatorDefault);
    CFStringRef uuidRef = CFUUIDCreateString(kCFAllocatorDefault, uuid);
    CFRelease(uuid);

    NSString *result = [NSString stringWithString:(NSString *)uuidRef];
    CFRelease(uuidRef);

    return [self b64DigestForString:result];
}

+ (NSData *)dataDigestForString:(NSString *)input
{
    const char *cstr = [input cStringUsingEncoding:NSUTF8StringEncoding];
    NSData     *data = [NSData dataWithBytes:cstr length:[input length]];

    uint8_t digest[CC_SHA1_DIGEST_LENGTH];
    CC_SHA1(data.bytes, data.length, digest);

    return [NSData dataWithBytes:digest length:CC_SHA1_DIGEST_LENGTH];
}

+ (NSString *)base64EncodedStringForData:(NSData *)data
{
    NSUInteger     b64EncodedLength = (data.length * 8 + 5) / 6;
    NSMutableData *result = [NSMutableData dataWithLength:b64EncodedLength];

    int resultLength = result.length;
    int dataLength   = data.length;

    if (data.bytes && dataLength && result.bytes && resultLength) {
        char *resultChars = (char *)result.bytes;
        const unsigned char *dataChars = data.bytes;

        while (resultLength > 2) {
            resultChars[0] = gBase64MappingWebSafe[dataChars[0] >> 2];
            resultChars[1] = gBase64MappingWebSafe[((dataChars[0] & 0x03) << 4) + (dataChars[1] >> 4)];
            resultChars[2] = gBase64MappingWebSafe[((dataChars[1] & 0x0f) << 2) + (dataChars[2] >> 6)];
            resultChars[3] = gBase64MappingWebSafe[dataChars[2] & 0x3f];

            resultLength -= 4;
            dataLength   -= 3;
            resultChars  += 4;
            dataChars    += 3;
        }

        switch (dataLength) {
            case 2:
                resultChars[0] = gBase64MappingWebSafe[dataChars[0] >> 2];
                resultChars[1] = gBase64MappingWebSafe[((dataChars[0] & 0x03) << 4) + (dataChars[1] >> 4)];
                resultChars[2] = gBase64MappingWebSafe[(dataChars[1] & 0x0f) << 2];
                //resultChars  += 3;
                //resultLength -= 3;
                break;
            case 1:
                resultChars[0] = gBase64MappingWebSafe[dataChars[0] >> 2];
                resultChars[1] = gBase64MappingWebSafe[(dataChars[0] & 0x03) << 4];
                //resultChars  += 2;
                //resultLength -= 2;
                break;
            default:
                break;
        }
    }

    return [[[NSString alloc] initWithData:result encoding:NSUTF8StringEncoding] autorelease];
}

+ (NSString *)hexEncodedStringForData:(NSData *)data
{
    const uint8_t   *bytes  = data.bytes;
    NSMutableString *result = [NSMutableString stringWithCapacity:2 * data.length];

    for (NSUInteger i = 0; i < data.length; i++) {
        [result appendFormat:@"%02x", bytes[i]];
    }

    return result;
}

+ (NSString *)hexDigestForString:(NSString *)input
{
    return [self hexEncodedStringForData:[self dataDigestForString:input]];
}

+ (NSString *)b64DigestForString:(NSString *)input
{
    return [self base64EncodedStringForData:[self dataDigestForString:input]];
}

+ (NSDictionary *)dictionaryWithQueryString:(NSString *)input
{
    NSMutableDictionary *result = [NSMutableDictionary dictionary];

    if (input != nil) {
        NSScanner      *scanner    = [[NSScanner alloc] initWithString:input];
        NSCharacterSet *delimiters = [NSCharacterSet characterSetWithCharactersInString:@"&;"];

        while (![scanner isAtEnd]) {
            NSString *pair;
            [scanner scanUpToCharactersFromSet:delimiters intoString:&pair];
            [scanner scanCharactersFromSet:delimiters intoString:nil];

            NSArray *keyAndValue = [pair componentsSeparatedByString:@"="];
            if ([keyAndValue count] == 2) {
                NSString *key   = [PHStringUtil stringByUrlDecodingString:[keyAndValue objectAtIndex:0]];
                NSString *value = [PHStringUtil stringByUrlDecodingString:[keyAndValue objectAtIndex:1]];

                [result setValue:value forKey:key];
            }
        }

        [scanner release];
    }

    return result;
}
@end
