//
//  CKServerChangeToken.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CloudKit/CKDatabaseOperation.h>

NS_CLASS_AVAILABLE(10_10, 8_0)
@interface CKServerChangeToken : NSObject <NSCopying, NSSecureCoding>
- (instancetype)init NS_UNAVAILABLE;
@end

