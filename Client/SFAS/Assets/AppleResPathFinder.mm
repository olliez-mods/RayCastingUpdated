#include "AppleResPathFinder.hpp"
#import <Foundation/Foundation.h>
std::string applerp__::resourcePath(void)
{
    //THIS IS BROKEN
    //NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    std::string rpath;
    NSBundle* bundle = [NSBundle mainBundle];

    if (bundle == nil) {
#ifdef DEBUG
        NSLog(@"bundle is nil... thus no resources path can be found.");
#endif
    } else {
        NSString* path = [bundle resourcePath];
        rpath = [path UTF8String] + std::string("/");
    }

    //BROKEN
    //[pool drain];

    return rpath;
}
