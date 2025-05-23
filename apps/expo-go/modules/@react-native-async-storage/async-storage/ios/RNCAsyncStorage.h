/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#import <Foundation/Foundation.h>

#import <React/RCTBridgeModule.h>
#import <React/RCTInvalidating.h>

#ifdef RCT_NEW_ARCH_ENABLED
#import <rnasyncstorage/rnasyncstorage.h>
#endif

#import "RNCAsyncStorageDelegate.h"

/**
 * A simple, asynchronous, persistent, key-value storage system designed as a
 * backend to the AsyncStorage JS module, which is modeled after LocalStorage.
 *
 * Current implementation stores small values in serialized dictionary and
 * larger values in separate files. Since we use a serial file queue
 * `RKFileQueue`, reading/writing from multiple threads should be perceived as
 * being atomic, unless someone bypasses the `RNCAsyncStorage` API.
 *
 * Keys and values must always be strings or an error is returned.
 */

NS_ASSUME_NONNULL_BEGIN

@interface RNCAsyncStorage : NSObject <
#ifdef RCT_NEW_ARCH_ENABLED
                                 NativeAsyncStorageModuleSpec
#else
                                 RCTBridgeModule
#endif
                                 ,
                                 RCTInvalidating>

@property (nonatomic, weak, nullable) id<RNCAsyncStorageDelegate> delegate;

@property (nonatomic, assign) BOOL clearOnInvalidate;

@property (nonatomic, readonly, getter=isValid) BOOL valid;

// NOTE(nikki): Added to allow scoped per Expo app
- (instancetype)initWithStorageDirectory:(NSString *)storageDirectory;

// Clear the RNCAsyncStorage data from native code
- (void)clearAllData;

// Grab data from the cache. ResponseBlock result array will have an error at position 0, and an
// array of arrays at position 1.
- (void)multiGet:(NSArray<NSString *> *)keys callback:(RCTResponseSenderBlock)callback;

// Add multiple key value pairs to the cache.
- (void)multiSet:(NSArray<NSArray<NSString *> *> *)kvPairs
        callback:(RCTResponseSenderBlock)callback;

// Interface for natively fetching all the keys from the storage data.
- (void)getAllKeys:(RCTResponseSenderBlock)callback;

@end

NS_ASSUME_NONNULL_END
