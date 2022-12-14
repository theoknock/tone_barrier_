//
//  ViewController.m
//  tone_barrier
//
//  Created by Xcode Developer on 8/2/22.
//

#import "ViewController.h"
#import "ViewController+AudioController.h"
#import "ViewController+AudioGenerator.h"

#import <objc/runtime.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


@interface ViewController ()

@end

@implementation ViewController

static typeof(toggle_audio) _Nonnull (^audio_control)(audio_state_notification_handler) = NULL;
+ (typeof(toggle_audio) _Nonnull (^)(audio_state_notification_handler)) audio_control
{
    static dispatch_once_t onceSecurePredicate;
    dispatch_once(&onceSecurePredicate, ^{
            audio_control = ^ (AVAudioEngine const * audio_engine, AVAudioSession const * audio_session) {
                static audio_state_notification_handler object_composition = ^ bool (bool cond) { (cond) ? ^{ [player_node_ref pause]; [audio_engine_ref pause]; }() : ^{ [audio_engine_ref startAndReturnError:nil]; [player_node_ref play]; }(); return cond; };
                static volatile audio_state_notification_handler_t object_composition_t = &object_composition;
                return ^ (audio_state_notification_handler object) {
                    *object_composition_t = audio_state_notification_handlers(object, *object_composition_t);
                    return ^ bool {
                        __autoreleasing NSError * error = nil;
                        return (*object_composition_t)((audio_engine_ref.isRunning));//((![audio_engine isRunning])  && [audio_session setActive:[audio_engine startAndReturnError:&error] error:&error] && !error) || (^ bool { [audio_engine stop]; __autoreleasing NSError * error = nil; return ![audio_session setActive:[audio_engine isRunning] error:&error]; }()));
                    };
                };
            }(audio_engine(player_node()), audio_session());
    });
    
    return audio_control;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    [self.routePickerView setDelegate:(id<AVRoutePickerViewDelegate> _Nullable)self];
    
    ^ (id obj, ...) {
        va_list argp;
        va_start(argp, obj);
        
//        AVAudioPlayerNode * player_node = va_arg(argp, AVAudioPlayerNode *);

        ViewController.audio_control(^ bool (const bool b) {
            //            ([audio_engine_ref isRunning] && ![player_node() isPlaying]) ? ^{ [player_node() play]; }() : ^{ [player_node() pause]; }();
            static void (^play_audio_buffer)(void);
            (play_audio_buffer = ^{
                [player_node_ref scheduleBuffer:audio_buffer(audio_format())() completionCallbackType:AVAudioPlayerNodeCompletionDataPlayedBack completionHandler:^(AVAudioPlayerNodeCompletionCallbackType callbackType) {
                   printf("play_audio_buffer()\n");
//                    play_audio_buffer();
                    !(audio_engine_ref.isRunning) ?: play_audio_buffer();
                }];
            })();
            play_audio_buffer();
            printf("\t\tplayer_node playing state is %s (%s)(%s)\n", (player_node_ref.isPlaying) ? "TRUE" : "FALSE", (audio_engine_ref.isRunning) ? "TRUE" : "FALSE", (b) ? "TRUE" : "FALSE");
            
            return (player_node_ref.isPlaying);
        });
        
    }((id)player_node_ref);
    
    ^ (bool(^toggle_play_pause)(void), ...) {
        va_list argp;
        va_start(argp, toggle_play_pause);
        
        UIButton * play_pause_button = va_arg(argp, UIButton *);
        
        [play_pause_button setImage:[UIImage systemImageNamed:@"pause.circle"] forState:UIControlStateSelected];
        [play_pause_button setImage:[UIImage systemImageNamed:@"play.circle"]  forState:UIControlStateNormal];
    [play_pause_button setImage:[UIImage systemImageNamed:@"play.slash"]   forState:UIControlStateDisabled];
    
    objc_setAssociatedObject(play_pause_button, @selector(invoke), toggle_play_pause, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    [play_pause_button addTarget:objc_getAssociatedObject(play_pause_button, @selector(invoke)) action:@selector(invoke) forControlEvents:UIControlEventTouchDown];
    }(ViewController.audio_control(^ (UIButton * play_pause_button) {
        return ^ bool (const bool b) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [play_pause_button setSelected:audio_engine_ref.isRunning];
            });
            printf("toggle_play_pause_button selected state set to %s (%s)\n", (play_pause_button.isSelected) ? "TRUE" : "FALSE", ([audio_engine_ref isRunning]) ? "TRUE" : "FALSE");
            return audio_engine_ref.isRunning;
        };
    }(_playPauseButton)), _playPauseButton);
    
    ^ (bool(^toggle_play_pause)(void), ...) {
        va_list argp;
        va_start(argp, toggle_play_pause);
        
//        UIButton * play_pause_button = va_arg(argp, UIButton *);
        
        MPNowPlayingInfoCenter * nowPlayingInfoCenter = va_arg(argp, MPNowPlayingInfoCenter *);
        MPRemoteCommandCenter  * remoteCommandCenter  = va_arg(argp, MPRemoteCommandCenter  *);
        
        NSMutableDictionary<NSString *, id> * nowPlayingInfo = [[NSMutableDictionary alloc] initWithCapacity:4];
        [nowPlayingInfo setObject:@"ToneBarrier" forKey:MPMediaItemPropertyTitle];
        [nowPlayingInfo setObject:(NSString *)@"James Alan Bush" forKey:MPMediaItemPropertyArtist];
        [nowPlayingInfo setObject:(NSString *)@"The Life of a Demoniac" forKey:MPMediaItemPropertyAlbumTitle];
        
        static UIImage * image;
        image = [UIImage imageNamed:@"WaveIcon"];
        static MPMediaItemArtwork *artwork;
        artwork = [[MPMediaItemArtwork alloc] initWithBoundsSize:image.size requestHandler:^UIImage * _Nonnull(CGSize size) {
            return image;
        }];
        [nowPlayingInfo setObject:(MPMediaItemArtwork *)artwork forKey:MPMediaItemPropertyArtwork];

        [(nowPlayingInfoCenter = [MPNowPlayingInfoCenter defaultCenter]) setNowPlayingInfo:(NSDictionary<NSString *,id> * _Nullable)nowPlayingInfo];

        MPRemoteCommandHandlerStatus (^remote_command_handler)(MPRemoteCommandEvent * _Nonnull) = ^ MPRemoteCommandHandlerStatus (MPRemoteCommandEvent * _Nonnull event) {
            __block NSError * error = nil;
            [nowPlayingInfoCenter setPlaybackState:(audio_engine_ref.isRunning) ? MPNowPlayingPlaybackStatePlaying : MPNowPlayingPlaybackStateStopped];
//            [nowPlayingInfoCenter setPlaybackState:((toggle_audio)(objc_getAssociatedObject(play_pause_button, @selector(invoke))))()];
            return (!error) ? MPRemoteCommandHandlerStatusSuccess : MPRemoteCommandHandlerStatusCommandFailed;
        };

        [[(remoteCommandCenter = [MPRemoteCommandCenter sharedCommandCenter]) playCommand] addTargetWithHandler:remote_command_handler];
        [[remoteCommandCenter stopCommand] addTargetWithHandler:remote_command_handler];
        [[remoteCommandCenter pauseCommand] addTargetWithHandler:remote_command_handler];
        [[remoteCommandCenter togglePlayPauseCommand] addTargetWithHandler:remote_command_handler];
    }(ViewController.audio_control(^ bool (const bool b) {
        printf("audio engine/session selected state set to %s\n", (b) ? "TRUE" : "FALSE");
        printf("MPNowPlayingInfoCenter playback state set to %lu\n", (unsigned long)[[MPNowPlayingInfoCenter defaultCenter] playbackState]);
        (b) ? [[UIApplication sharedApplication] beginReceivingRemoteControlEvents] : [[UIApplication sharedApplication] endReceivingRemoteControlEvents];
        return b;
    }), _nowPlayingInfoCenter, _remoteCommandCenter);
}

@end
