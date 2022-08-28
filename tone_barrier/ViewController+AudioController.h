//
//  ViewController+AudioController.h
//  tone_barrier
//
//  An Objective-C class extension that declares properties related to
//  audio configuration and playback
//  used by the ViewController class
//
//  Created by Xcode Developer on 8/3/22.
//

#import "ViewController.h"
@import Accelerate;


NS_ASSUME_NONNULL_BEGIN

static AVAudioSourceNodeRenderBlock (^audio_renderer)(void) = ^ AVAudioSourceNodeRenderBlock {
    const Float32 frequency = 440.f;
    const Float32 sampleRate = 48000.f;
    const Float32 amplitude = 0.25f;
    const Float32 M_PI_SQR = 2.f * M_PI;
    
    return ^OSStatus(BOOL * _Nonnull isSilence, const AudioTimeStamp * _Nonnull timestamp, AVAudioFrameCount frameCount, AudioBufferList * _Nonnull outputData) {
        Float32 theta_increment = M_PI_SQR * frequency / sampleRate;
        Float32 * buffer = (Float32 *)outputData->mBuffers[0].mData;
        
        for (AVAudioFrameCount frame = 0; frame < frameCount; frame++)
            *(buffer + frame) = sin(^ (Float32 * theta_increment_t) {
                static Float32 theta;
                !((theta += *theta_increment_t) > M_PI_SQR) ?: (theta -= M_PI_SQR);
                return theta;
            }(&theta_increment)) * amplitude;
        
        return (OSStatus)noErr;
    };
};

static AVAudioSourceNode * (^audio_source)(AVAudioSourceNodeRenderBlock) = ^ AVAudioSourceNode * (AVAudioSourceNodeRenderBlock audio_renderer) {
    AVAudioSourceNode * source_node = [[AVAudioSourceNode alloc] initWithRenderBlock:audio_renderer];
    return source_node;
};

typedef typeof(void(^(^)(NSNotification *, void(^)(NSNotification *)))(void)) notification_observer;
typedef typeof(void(^)(void)) notification;
void(^(^(^notification_observation)(NSNotificationCenter *, NSOperationQueue *))(NSNotification *, void(^)(NSNotification *)))(void)  = ^ (NSNotificationCenter * notification_center, NSOperationQueue * operation_queue) {
    return ^ (NSNotification * observed_notification, typeof(void(^)(NSNotification *))notification_handler) {
        return ^{
            [notification_center addObserverForName:observed_notification.name object:observed_notification.object queue:operation_queue usingBlock:notification_handler];
        };
    };
};
static void (^observe_notifications)(NSArray<notification> *) = ^ (NSArray<notification> * notifications) {
    for (notification observed_notification in notifications) observed_notification();
};

static AVAudioSession * (^audio_session)(void) = ^ AVAudioSession * {
    AVAudioSession * session = [AVAudioSession sharedInstance];
    
    @try {
        __autoreleasing NSError *error = nil;
        [session setCategory:AVAudioSessionCategoryPlayAndRecord mode:AVAudioSessionModeDefault options:AVAudioSessionCategoryOptionDefaultToSpeaker error:&error];
        [session setSupportsMultichannelContent:TRUE  error:&error];
        [session setPreferredInputNumberOfChannels:2  error:&error];
        [session setPreferredOutputNumberOfChannels:2 error:&error];
        [session setPrefersNoInterruptionsFromSystemAlerts:TRUE error:&error]; // TO-DO: Make this a user-specified preference
        
        !(error) ?: ^ (NSError ** error_t) {
            printf("Error configuring audio session:\n\t%s\n", [[*error_t debugDescription] UTF8String]);
            NSException* exception = [NSException
                                      exceptionWithName:(*error_t).domain
                                      reason:(*error_t).localizedDescription
                                      userInfo:@{@"Error Code" : @((*error_t).code)}];
            @throw exception;
        }(&error);
    } @catch (NSException *exception) {
        printf("Exception configuring audio session:\n\t%s\n\t%s\n\t%lu",
               [exception.name UTF8String],
               [exception.reason UTF8String],
               ((NSNumber *)[exception.userInfo valueForKey:@"Error Code"]).unsignedIntegerValue);
    } @finally {
        // Setup notifications
        
        notification_observer audio_session_notification_observer = notification_observation([NSNotificationCenter defaultCenter], [NSOperationQueue mainQueue]);
        
        notification observe_audio_session_interruption_notification = audio_session_notification_observer([NSNotification notificationWithName:(NSNotificationName)AVAudioSessionInterruptionNotification object:session], ^(NSNotification * notification) {
            UInt8 theInterruptionType = [[notification.userInfo valueForKey:AVAudioSessionInterruptionTypeKey] intValue];
            NSLog(@"Session interrupted > --- %s ---\n", theInterruptionType == AVAudioSessionInterruptionTypeBegan ? "Begin Interruption" : "End Interruption");
            static BOOL _isSessionInterrupted;
            switch (theInterruptionType) {
                case AVAudioSessionInterruptionTypeBegan: {
                    //                _isSessionInterrupted = [_engine isRunning];
                    //                !(_isSessionInterrupted) ?: [self toggleAudioEngineRunningStatus:((ViewController *)[[[[UIApplication sharedApplication] delegate] window] rootViewController]).playPauseButton];
                    break;
                }
                    
                case AVAudioSessionInterruptionTypeEnded: {
                    //                !(_isSessionInterrupted) ?: [self toggleAudioEngineRunningStatus:((ViewController *)[[[[UIApplication sharedApplication] delegate] window] rootViewController]).playPauseButton];
                    //                _isSessionInterrupted = FALSE;
                    break;
                }
                    
                default:
                    break;
            }
        });
        
        notification observe_audio_route_change_notification = audio_session_notification_observer([NSNotification notificationWithName:(NSNotificationName)AVAudioSessionRouteChangeNotification object:session], ^(NSNotification * notification) {
            UInt8 reasonValue = [[notification.userInfo valueForKey:AVAudioSessionRouteChangeReasonKey] intValue];
            AVAudioSessionRouteDescription *routeDescription = [notification.userInfo valueForKey:AVAudioSessionRouteChangePreviousRouteKey];
            
            NSLog(@"Route change:");
            switch (reasonValue) {
                case AVAudioSessionRouteChangeReasonNewDeviceAvailable:
                    NSLog(@"     NewDeviceAvailable");
                    break;
                case AVAudioSessionRouteChangeReasonOldDeviceUnavailable:
                    NSLog(@"     OldDeviceUnavailable");
                    break;
                case AVAudioSessionRouteChangeReasonCategoryChange:
                    NSLog(@"     CategoryChange");
                    NSLog(@"     New Category: %@", [[AVAudioSession sharedInstance] category]);
                    break;
                case AVAudioSessionRouteChangeReasonOverride:
                    NSLog(@"     Override");
                    break;
                case AVAudioSessionRouteChangeReasonWakeFromSleep:
                    NSLog(@"     WakeFromSleep");
                    break;
                case AVAudioSessionRouteChangeReasonNoSuitableRouteForCategory:
                    NSLog(@"     NoSuitableRouteForCategory");
                    break;
                default:
                    NSLog(@"     ReasonUnknown");
            }
            
            NSLog(@"Previous route:\n");
            NSLog(@"%@", routeDescription);
        });

        observe_notifications(@[observe_audio_session_interruption_notification, observe_audio_route_change_notification]);
    }
    
    return session;
};

static AVAudioEngine * (^audio_engine)(AVAudioSourceNode *) = ^(AVAudioSourceNode * audio_source) {
    AVAudioEngine * audio_engine = [[AVAudioEngine alloc] init];
    [audio_engine attachNode:audio_source];
    [audio_engine connect:audio_source to:audio_engine.mainMixerNode format:nil];
    [audio_engine prepare];
    
    return audio_engine;
};

typedef typeof(bool(^)(void)) audio_state;
typedef typeof(audio_state(^)(AVAudioEngine *, AVAudioSession *)) audio_state_ref;
audio_state_ref audio_state_ref_init = ^ (AVAudioEngine * audio_engine, AVAudioSession * audio_session) {
    __block NSError * error = nil;
    return ^ bool {
        return ([audio_session setActive:(((![audio_engine isRunning]) && ^ BOOL { [audio_engine startAndReturnError:&error]; return (!error) ? ([audio_engine isRunning]) : FALSE; }()) || ^ BOOL { [audio_engine stop]; return ([audio_engine isRunning]); }()) error:&error]) & [audio_engine isRunning];
    };
};

typedef typeof(const bool (^ __strong)(const bool)) audio_state_notification_handler;
typedef const typeof(audio_state_notification_handler * restrict) audio_state_notification_handler_t;
audio_state_notification_handler (^audio_state_notification_handlers)(audio_state_notification_handler, audio_state_notification_handler) = ^ (audio_state_notification_handler object_blk_a, audio_state_notification_handler object_blk_b) {
    return ^ bool (const bool b) {
        return object_blk_b(object_blk_a(b));
    };
};

typedef typeof(const bool (^ __strong)(const bool)) audio_state_notification_handler;
typedef typeof(const bool(^ __strong)(void)) toggle_audio_control_state;
typedef typeof(toggle_audio_control_state(^)(audio_state_notification_handler, ...)) audio_control;
static bool (^(^(^audio_controller)(const _Nonnull audio_state))(audio_state_notification_handler))(void) = ^ (const audio_state _Nonnull state) {
    static audio_state_notification_handler object_composition = ^ bool (bool cond) { return cond; };
    static audio_state_notification_handler_t object_composition_t = &object_composition;
    return ^ (audio_state_notification_handler object) {
        *object_composition_t = audio_state_notification_handlers(object, *object_composition_t);
        return ^ bool {
            return (*object_composition_t)(state());
        };
    };
};
static bool (^ _Nonnull (^ _Nonnull (^ _Nonnull const (* _Nonnull restrict audio_controller_t))(const _Nonnull audio_state))(audio_state_notification_handler))(void) = &audio_controller;


@interface ViewController ()



@end

NS_ASSUME_NONNULL_END
