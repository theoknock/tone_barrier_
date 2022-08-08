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

NS_ASSUME_NONNULL_BEGIN

static AVAudioSourceNodeRenderBlock (^audio_renderer)(void) = ^ AVAudioSourceNodeRenderBlock {
    __block Float32 theta = 0.f;
    const Float32 frequency = 440.f;
    const Float32 sampleRate = 48000.f;
    const Float32 amplitude = 0.25f;
    const Float32 M_PI_SQR = 2.f * M_PI;
    
    return ^OSStatus(BOOL * _Nonnull isSilence, const AudioTimeStamp * _Nonnull timestamp, AVAudioFrameCount frameCount, AudioBufferList * _Nonnull outputData) {
                Float32 theta_increment = M_PI_SQR * frequency / sampleRate;
                Float32 * buffer = (Float32 *)outputData->mBuffers[0].mData;
                for (AVAudioFrameCount frame = 0; frame < frameCount; frame++)
                {
                    buffer[frame] = sin(theta) * amplitude;
                    theta += theta_increment;
                    !(theta > M_PI_SQR) ?: (theta -= M_PI_SQR);
                }
                return (OSStatus)noErr;
            };
};

static AVAudioSourceNode * (^AudioSourceRef)(AVAudioSourceNodeRenderBlock) = ^ AVAudioSourceNode * (AVAudioSourceNodeRenderBlock audio_renderer) {
    AVAudioSourceNode * source_node = [[AVAudioSourceNode alloc] initWithRenderBlock:audio_renderer];

    return source_node;
};

static AVAudioSession * (^AudioSessionRef)(void) = ^ AVAudioSession * {
    AVAudioSession * session = [AVAudioSession sharedInstance];
    
    @try {
        __autoreleasing NSError *error = nil;
//        [self.session setCategory:AVAudioSessionCategoryPlayback error:&error];
//        [self.session setMode:AVAudioSessionModeDefault error:&error];
        [session setCategory:AVAudioSessionCategoryPlayAndRecord mode:AVAudioSessionModeDefault options:AVAudioSessionCategoryOptionDefaultToSpeaker error:&error];
        [session setSupportsMultichannelContent:TRUE error:&error];
        [session setPreferredInputNumberOfChannels:2 error:&error];
        [session setPreferredOutputNumberOfChannels:2 error:&error];
        [session setPrefersNoInterruptionsFromSystemAlerts:TRUE error:&error];
        
        !(!error) ?: ^ (NSError ** error_t) {
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
    }
    
    return session;
};

static AVAudioEngine * (^AudioEngineRef)(AVAudioSourceNode *) = ^(AVAudioSourceNode * audio_source) {
    AVAudioEngine * audio_engine = [[AVAudioEngine alloc] init];
    [audio_engine attachNode:audio_source];
    [audio_engine connect:audio_source to:audio_engine.mainMixerNode format:nil];
    [audio_engine prepare];
    
    return audio_engine;
};

static bool (^toggle_audio_playback)(AVAudioEngine *, AVAudioSession *) = ^ bool (AVAudioEngine * audio_engine, AVAudioSession * audio_session) {
    __block NSError * error = nil;
    return ([audio_session setActive:(((![audio_engine isRunning]) && ^ BOOL { [audio_engine startAndReturnError:&error]; return (!error) ? ([audio_engine isRunning]) : FALSE; }()) || ^ BOOL { [audio_engine stop]; return ([audio_engine isRunning]); }()) error:&error]) & [audio_engine isRunning];
};

@interface ViewController ()



@end

NS_ASSUME_NONNULL_END
