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
#include <stdint.h>

#import "ViewController+AudioGenerator.h"

NS_ASSUME_NONNULL_BEGIN



static AVAudioSourceNodeRenderBlock (^audio_renderer)(void) = ^ AVAudioSourceNodeRenderBlock {
    static int note_c;
    static int * note_ct = &note_c;
    random_n r = random_generator(^ double (double n) { return n; })(^ double (double n) { return n; })(^ double (double n) { return scale(n, 0, 12, 0.0, 1.0); });
    double(^t)(double) = tonal();
    const Float32 sampleRate = 48000.f;
    const Float32 amplitude = 0.25f;
    const Float32 M_PI_SQR = 2.f * M_PI;
    Float32 (^phase_channel_l)(AVAudioFrameCount) = ^(AVAudioFrameCount split_frame) {
        __block Float32 theta_increment_t = (M_PI_SQR * note((*note_ct)) / sampleRate);
        return ^ (AVAudioFrameCount frame) {
            if ((frame = frame % 50) == split_frame) (theta_increment_t = (M_PI_SQR * note((*note_ct)) / sampleRate));
            static Float32 theta;
            !((theta += theta_increment_t) > M_PI_SQR) ?: (theta -= M_PI_SQR);
            return sinf(theta);
        };
    }(50);
    
    Float32 (^phase_channel_r)(AVAudioFrameCount) = ^(AVAudioFrameCount split_frame) {
        __block Float32 theta_increment_t = (M_PI_SQR * note((*note_ct)) / sampleRate); // tonal(note()) is wrong; use root_frequency generated by note() on the other channel
        return ^ (AVAudioFrameCount frame) {
            if ((frame = frame % 50) == split_frame) (theta_increment_t = (M_PI_SQR * note((*note_ct)++) / sampleRate));
            static Float32 theta;
            !((theta += theta_increment_t) > M_PI_SQR) ?: (theta -= M_PI_SQR);
            return sinf(theta);
        };
    }(50);
    
    return ^OSStatus(BOOL * _Nonnull isSilence, const AudioTimeStamp * _Nonnull timestamp, AVAudioFrameCount frameCount, AudioBufferList * _Nonnull outputData) {
        AVAudioFrameCount frame = 0;
        AVAudioFrameCount * frame_t = &frame;
        static int counter = 0;
//        for (; *frame_t < frameCount; (*frame_t = -~(*frame_t))) {
        for (frame = 0; frame < frameCount; frame++) {
            *((Float32 *)((Float32 *)((outputData->mBuffers + 0))->mData) + frame) = (Float32)phase_channel_l(counter) * amplitude;
            *((Float32 *)((Float32 *)((outputData->mBuffers + 1))->mData) + frame) = (Float32)phase_channel_r(counter) * amplitude;
        }
        counter++;
        printf("note_c == %d\n", (*note_ct));
        return (OSStatus)noErr;
    };
};

static AVAudioFormat const * audio_format_ref = NULL;
static AVAudioFormat const * (^audio_format)(void) = ^ AVAudioFormat const * {
    static dispatch_once_t onceSecurePredicate;
    dispatch_once(&onceSecurePredicate, ^{
        audio_format_ref = [[AVAudioFormat alloc] initStandardFormatWithSampleRate:48000.f channels:(AVAudioChannelCount)2];
    });
    return audio_format_ref;
};

static AVAudioSourceNode const * audio_source_ref = NULL;
static AVAudioSourceNode const * (^audio_source)(AVAudioFormat *, AVAudioSourceNodeRenderBlock) = ^ AVAudioSourceNode const * (AVAudioFormat * audio_format, AVAudioSourceNodeRenderBlock audio_renderer) {
    static dispatch_once_t onceSecurePredicate;
    dispatch_once(&onceSecurePredicate, ^{
        audio_source_ref = [[AVAudioSourceNode alloc] initWithFormat:audio_format renderBlock:audio_renderer];
    });
    return audio_source_ref;
};

static AVAudioEngine const * audio_engine_ref = NULL;
static AVAudioEngine const * (^audio_engine)(AVAudioSourceNode *) = ^(AVAudioSourceNode * audio_source) {
    static dispatch_once_t onceSecurePredicate;
    dispatch_once(&onceSecurePredicate, ^{
        audio_engine_ref = [[AVAudioEngine alloc] init];
        [audio_engine_ref attachNode:audio_source];
        [audio_engine_ref connect:audio_source to:audio_engine_ref.mainMixerNode format:[audio_engine_ref.mainMixerNode outputFormatForBus:(AVAudioNodeBus)0]];
    });
    return audio_engine_ref;
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

static AVAudioSession const * audio_session_ref = NULL;
static AVAudioSession const * (^audio_session)(void) = ^ AVAudioSession const * {
    static dispatch_once_t onceSecurePredicate;
    dispatch_once(&onceSecurePredicate, ^{
        audio_session_ref = [AVAudioSession sharedInstance];
        
        @try {
            __autoreleasing NSError *error = nil;
            [audio_session_ref setCategory:AVAudioSessionCategoryPlayAndRecord mode:AVAudioSessionModeDefault options:AVAudioSessionCategoryOptionDefaultToSpeaker error:&error];
            [audio_session_ref setSupportsMultichannelContent:TRUE  error:&error];
            [audio_session_ref setPreferredInputNumberOfChannels:2  error:&error];
            [audio_session_ref setPreferredOutputNumberOfChannels:2 error:&error];
            [audio_session_ref setPrefersNoInterruptionsFromSystemAlerts:TRUE error:&error]; // TO-DO: Make this a user-specified preference
            
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
            
            notification observe_audio_session_interruption_notification = audio_session_notification_observer([NSNotification notificationWithName:(NSNotificationName)AVAudioSessionInterruptionNotification object:audio_session_ref], ^(NSNotification * notification) {
                UInt8 theInterruptionType = [[notification.userInfo valueForKey:AVAudioSessionInterruptionTypeKey] intValue];
                NSLog(@"Session interrupted > --- %s ---\n", theInterruptionType == AVAudioSessionInterruptionTypeBegan ? "Begin Interruption" : "End Interruption");
                static BOOL _isSessionInterrupted;
                toggle_audio toggle_play_pause = ViewController.audio_control(^ bool (bool cond) { return cond; });
                switch (theInterruptionType) {
                    case AVAudioSessionInterruptionTypeBegan: {
                        _isSessionInterrupted = [audio_engine_ref isRunning];
                        toggle_play_pause();
                        break;
                    }
                        
                    case AVAudioSessionInterruptionTypeEnded: {
                        !(_isSessionInterrupted) ?: toggle_play_pause();
                        _isSessionInterrupted = FALSE;
                        break;
                    }
                        
                    default:
                        break;
                }
            });
            
            notification observe_audio_route_change_notification = audio_session_notification_observer([NSNotification notificationWithName:(NSNotificationName)AVAudioSessionRouteChangeNotification object:audio_session_ref], ^(NSNotification * notification) {
                AVAudioSessionRouteDescription *routeDescription = [notification.userInfo valueForKey:AVAudioSessionRouteChangePreviousRouteKey];
                
                UInt8 reasonValue = [[notification.userInfo valueForKey:AVAudioSessionRouteChangeReasonKey] intValue];
                AVAudioSessionPortDescription * output = [audio_session_ref.currentRoute.outputs firstObject];
                switch (reasonValue) {
                    case AVAudioSessionRouteChangeReasonNewDeviceAvailable || AVAudioSessionRouteChangeReasonOldDeviceUnavailable: {
                        
                        if (output.portType == AVAudioSessionPortHeadphones) {
                            printf("\nheadphones\n");
                        }
                        break;
                    }
                    case AVAudioSessionRouteChangeReasonOldDeviceUnavailable:
                        //                    NSLog(@"     OldDeviceUnavailable");
                        //                    break;
                    case AVAudioSessionRouteChangeReasonCategoryChange:
                        //                    NSLog(@"     CategoryChange");
                        //                    NSLog(@"     New Category: %@", [[AVAudioSession sharedInstance] category]);
                        break;
                    case AVAudioSessionRouteChangeReasonOverride:
                        //                    NSLog(@"     Override");
                        break;
                    case AVAudioSessionRouteChangeReasonWakeFromSleep:
                        //                    NSLog(@"     WakeFromSleep");
                        break;
                    case AVAudioSessionRouteChangeReasonNoSuitableRouteForCategory:
                        //                    NSLog(@"     NoSuitableRouteForCategory");
                        break;
                    default: ;
                        //                    NSLog(@"     ReasonUnknown");
                }
                
                
            });
            
            observe_notifications(@[observe_audio_session_interruption_notification, observe_audio_route_change_notification]);
        }
    });
    
    return audio_session_ref;
};

typedef typeof(const bool (^ __strong)(const bool)) audio_state_notification_handler;
typedef const typeof(audio_state_notification_handler *) audio_state_notification_handler_t;
typedef typeof(audio_state_notification_handler(^)(void)) audio_state_notification_handler_container;
audio_state_notification_handler (^audio_state_notification_handlers)(audio_state_notification_handler, audio_state_notification_handler) = ^ (audio_state_notification_handler object_blk_a, audio_state_notification_handler object_blk_b) {
    return ^ bool (const bool b) {
        return object_blk_b(object_blk_a(b));
    };
};


audio_state state = ^ bool {
    return [audio_engine_ref isRunning];
};

@interface ViewController ()

@end

NS_ASSUME_NONNULL_END
