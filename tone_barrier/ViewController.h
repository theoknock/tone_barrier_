//
//  ViewController.h
//  tone_barrier
//
//  Created by Xcode Developer on 8/2/22.
//

@import UIKit;
@import Foundation;
@import AVFoundation;
@import AVFAudio;
@import AVRouting;
@import AVKit;
@import MediaPlayer;

NS_ASSUME_NONNULL_BEGIN

@interface ViewController : UIViewController <AVRoutePickerViewDelegate>

@property (weak, nonatomic) IBOutlet UIButton *playPauseButton;
@property (weak, nonatomic) IBOutlet AVRoutePickerView *routePickerView;

@property (strong, nonatomic) MPNowPlayingInfoCenter * nowPlayingInfoCenter;
@property (strong, nonatomic) MPRemoteCommandCenter  * remoteCommandCenter;

typedef typeof(bool(^)(void)) toggle_audio;
typedef typeof(const bool (^ __strong)(const bool)) audio_state_notification_handler;
typedef const typeof(audio_state_notification_handler *) audio_state_notification_handler_t;
+ (typeof(toggle_audio) _Nonnull (^)(audio_state_notification_handler)) audio_control;

@end

NS_ASSUME_NONNULL_END
