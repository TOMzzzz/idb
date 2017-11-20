/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <Foundation/Foundation.h>

#import <FBControlCore/FBEventReporter.h>
#import <FBControlCore/FBJSONConversion.h>
#import <FBControlCore/FBFuture.h>
#import <FBControlCore/FBTerminationHandle.h>

NS_ASSUME_NONNULL_BEGIN

@protocol FBFileConsumer;
@protocol FBEventReporter;
@protocol FBiOSTarget;
@protocol FBiOSTargetFutureDelegate;
@protocol FBiOSTargetContinuation;

/**
 An extensible string enum representing an Action Type.
 */
typedef NSString *FBiOSTargetFutureType NS_EXTENSIBLE_STRING_ENUM;

/**
 The Action Type for an Application Launch.
 */
extern FBiOSTargetFutureType const FBiOSTargetFutureTypeApplicationLaunch;

/**
 The Action Type for an Agent Launch.
 */
extern FBiOSTargetFutureType const FBiOSTargetFutureTypeAgentLaunch;

/**
 The Action Type for a Test Launch.
 */
extern FBiOSTargetFutureType const FBiOSTargetFutureTypeTestLaunch;

/**
 The Result of invoking an FBiOSTargetFuture.
 Represents the execution state of the underlying operation.
 */
@protocol FBiOSTargetContinuation <FBTerminationHandle>

/**
 A Optional Future that resolves when the operation started from the FBiOSTargetFuture has completed.
 For any FBiOSTargetFuture that performs ongoing work, this will be non-nil.
 For any FBiOSTargetFuture that has finished it's work when resolved, this will be nil.
 */
@property (nonatomic, strong, nullable, readonly) FBFuture<NSNull *> *completed;

@end

/**
 Re-Names an existing continuation.
 Useful when a lower-level continuation should be hoisted to a higher-level naming.

 @param continuation the continuation to wrap
 @param handleType the handle to apply.
 @return a new Termination Awaitable.
 */
extern id<FBiOSTargetContinuation> FBiOSTargetContinuationRenamed(id<FBiOSTargetContinuation> continuation, FBTerminationHandleType handleType);

/**
 Makes a continuation that has nothing left to do.

 @param handleType the handle of the continuation
 @return a new Termination Awaitable.
 */
extern id<FBiOSTargetContinuation> FBiOSTargetContinuationDone(FBTerminationHandleType handleType);

/**
 A protocol that can be bridged to FBiOSTargetFutureDelegate
 */
@protocol FBiOSTargetFuture <NSObject, FBJSONSerializable, FBJSONDeserializable>

/**
 The Action Type of the Reciever.
 */
@property (nonatomic, copy, readonly) FBiOSTargetFutureType actionType;

/**
 Starts the action represented by the reciever.

 @param target the target to run against.
 @param consumer the consumer to report binary data to.
 @param reporter the reporter to report structured data to.
 @return a Future wrapping the action type.
 */
- (FBFuture<FBiOSTargetFutureType> *)runWithTarget:(id<FBiOSTarget>)target consumer:(id<FBFileConsumer>)consumer reporter:(id<FBEventReporter>)reporter;

@end

/**
 A base class for convenient FBiOSTargetFuture implementations.
 Most useful when there is an empty payload.
 */
@interface FBiOSTargetFutureSimple : NSObject <FBJSONSerializable, FBJSONDeserializable, NSCopying>

@end

NS_ASSUME_NONNULL_END
