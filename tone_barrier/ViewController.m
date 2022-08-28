//
//  ViewController.m
//  tone_barrier
//
//  Created by Xcode Developer on 8/2/22.
//

#import "ViewController.h"
#import "ViewController+AudioController.h"

#import <objc/runtime.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


@interface ViewController ()

@end

@implementation ViewController

static const float (^stride)(void);
static const float (^(^generator)(const float, const float, const int))(void) = ^ (const float start, const float stride, const int length) {
    static float result;
    result = start;
    static float end;
    end = start + (stride * length);
    return ^ const float {
        return (result = ((result += stride) < end) ? result : start);
    };
};

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    stride = generator(0.f, 1.f, 10);
    printf("stride = %f\n", stride());
    printf("stride = %f\n", stride());
    

    bool (^(^audio_controller_ref)(audio_state_notification_handler))(void) = (*audio_controller_t)(audio_state_ref_init(audio_engine(audio_source(audio_renderer())), audio_session()));
    void (^toggle_play_pause_button)(void) = ^ (bool(^toggle_play_pause)(void), ...) {
        va_list argp;
        va_start(argp, toggle_play_pause);
        
        UIButton * play_pause_button = va_arg(argp, UIButton *);
        
        [play_pause_button setImage:[UIImage systemImageNamed:@"pause.circle"] forState:UIControlStateSelected];
        [play_pause_button setImage:[UIImage systemImageNamed:@"play.circle"]  forState:UIControlStateNormal];
        [play_pause_button setImage:[UIImage systemImageNamed:@"play.slash"]   forState:UIControlStateDisabled];
        
        return ^{
            [play_pause_button setSelected:toggle_play_pause()];
        };
    }(audio_controller_ref(^ bool (const bool b) {
        printf("toggle_play_pause_button selected state set to %s\n", (b) ? "TRUE" : "FALSE");
        return b;
    }), _playPauseButton);
    objc_setAssociatedObject(_playPauseButton, @selector(invoke), toggle_play_pause_button, OBJC_ASSOCIATION_COPY_NONATOMIC);
    [_playPauseButton addTarget:toggle_play_pause_button action:@selector(invoke) forControlEvents:UIControlEventTouchDown];
    
    void (^toggle_media_player_play_pause_button)(void) = ^ (bool(^toggle_media_player_play_pause)(void), ...) {
        va_list argp;
        va_start(argp, toggle_media_player_play_pause);

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
            [nowPlayingInfoCenter setPlaybackState:toggle_media_player_play_pause()];
            return (!error) ? MPRemoteCommandHandlerStatusSuccess : MPRemoteCommandHandlerStatusCommandFailed;
        };

        [[(remoteCommandCenter = [MPRemoteCommandCenter sharedCommandCenter]) playCommand] addTargetWithHandler:remote_command_handler];
        [[remoteCommandCenter stopCommand] addTargetWithHandler:remote_command_handler];
        [[remoteCommandCenter pauseCommand] addTargetWithHandler:remote_command_handler];
        [[remoteCommandCenter togglePlayPauseCommand] addTargetWithHandler:remote_command_handler];

        [[UIApplication sharedApplication] beginReceivingRemoteControlEvents];
        return ^{
            // Nothing goes here
        };
    }(audio_controller_ref(^ bool (const bool b) {
        printf("toggle_media_player_play_pause_button playback state set to %s\n", (b) ? "TRUE" : "FALSE");
        return b;
    }), _nowPlayingInfoCenter, _remoteCommandCenter);
    
    [self.routePickerView setDelegate:(id<AVRoutePickerViewDelegate> _Nullable)self];
}

@end
