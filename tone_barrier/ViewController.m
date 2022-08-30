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

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    audio_control control_audio = audio_controller(audio_engine(audio_source(audio_format(), audio_renderer())), audio_session());
    
    [self.routePickerView setDelegate:(id<AVRoutePickerViewDelegate> _Nullable)self];
    
    [_playPauseButton setImage:[UIImage systemImageNamed:@"pause.circle"] forState:UIControlStateSelected];
    [_playPauseButton setImage:[UIImage systemImageNamed:@"play.circle"]  forState:UIControlStateNormal];
    [_playPauseButton setImage:[UIImage systemImageNamed:@"play.slash"]   forState:UIControlStateDisabled];
    
    objc_setAssociatedObject(_playPauseButton, @selector(invoke), control_audio(^ bool (const bool b) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [_playPauseButton setSelected:b];
        });
        printf("audio engine/session selected state set to %s\n", (b) ? "TRUE" : "FALSE");
        printf("toggle_play_pause_button selected state set to %s\n", (b) ? "TRUE" : "FALSE");
        return b;
    }), OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    [_playPauseButton addTarget:objc_getAssociatedObject(_playPauseButton, @selector(invoke)) action:@selector(invoke) forControlEvents:UIControlEventTouchDown];
    
    ^ (bool(^toggle_play_pause)(void), ...) {
        va_list argp;
        va_start(argp, toggle_play_pause);

        MPNowPlayingInfoCenter * nowPlayingInfoCenter = va_arg(argp, MPNowPlayingInfoCenter *);
        MPRemoteCommandCenter  * remoteCommandCenter  = va_arg(argp, MPRemoteCommandCenter  *);

        NSMutableDictionary<NSString *, id> * nowPlayingInfo = [[NSMutableDictionary alloc] initWithCapacity:4];
        [nowPlayingInfo setObject:@"ToneBarrier" forKey:MPMediaItemPropertyTitle];
        [nowPlayingInfo setObject:(NSString *)@"James Alan Bush" forKey:MPMediaItemPropertyArtist];
        [nowPlayingInfo setObject:(NSString *)@"The Life of a Demoniac" forKey:MPMediaItemPropertyAlbumTitle];

        static UIImage * image;
        MPMediaItemArtwork *artwork = [[MPMediaItemArtwork alloc] initWithImage:(image = [UIImage imageNamed:@"WaveIcon"])];
        [nowPlayingInfo setObject:(MPMediaItemArtwork *)artwork forKey:MPMediaItemPropertyArtwork];

        [(nowPlayingInfoCenter = [MPNowPlayingInfoCenter defaultCenter]) setNowPlayingInfo:(NSDictionary<NSString *,id> * _Nullable)nowPlayingInfo];

        MPRemoteCommandHandlerStatus (^remote_command_handler)(MPRemoteCommandEvent * _Nonnull) = ^ MPRemoteCommandHandlerStatus (MPRemoteCommandEvent * _Nonnull event) {
            __block NSError * error = nil;
            [nowPlayingInfoCenter setPlaybackState:toggle_play_pause()];
            return (!error) ? MPRemoteCommandHandlerStatusSuccess : MPRemoteCommandHandlerStatusCommandFailed;
        };

        [[(remoteCommandCenter = [MPRemoteCommandCenter sharedCommandCenter]) playCommand] addTargetWithHandler:remote_command_handler];
        [[remoteCommandCenter stopCommand] addTargetWithHandler:remote_command_handler];
        [[remoteCommandCenter pauseCommand] addTargetWithHandler:remote_command_handler];
        [[remoteCommandCenter togglePlayPauseCommand] addTargetWithHandler:remote_command_handler];
    }(control_audio(^ bool (const bool b) {
        (b) ? [[UIApplication sharedApplication] beginReceivingRemoteControlEvents] : [[UIApplication sharedApplication] endReceivingRemoteControlEvents];
        return b;
    }), _nowPlayingInfoCenter, _remoteCommandCenter);
}

@end
