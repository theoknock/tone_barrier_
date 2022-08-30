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
+ (typeof(bool (^)(void)) _Nonnull (^)(const bool (^)(const bool))) audio_control;

@end

NS_ASSUME_NONNULL_END
