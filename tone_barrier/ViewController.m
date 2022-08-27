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

//static void (^(^(^notification_observer)(NSNotificationCenter *))(NSNotification *, typeof(void(^)(NSNotification *)))(void)) = ^ (NSNotificationCenter * notification_center) {
//    return ^ (NSNotification * observed_notification, typeof(void(^)(NSNotification *))notification_handler) {
//        return ^{
//            [notification_center addObserverForName:observed_notification.name object:observed_notification.object queue:[NSOperationQueue mainQueue] usingBlock:notification_handler];
//        };
//    };
//};

//typedef typeof(unsigned long (^)(unsigned long)) iterator;
//typedef typeof(void(^__strong)(bool)) mapper;
//typedef typeof(void(^__strong)(bool)) applier;
//static void (^(^(^iterate_)(const unsigned long))(typeof(mapper)))(void) = ^ (const unsigned long iterations) {
//    CFTypeRef obj_collection[iterations];
//    return ^ (CFTypeRef obj_collection_t) {
//        __block iterator integrand;
//        return ^ (mapper map) {
//            (integrand = ^ unsigned long (unsigned long index) {
//                --index;
//                *((CFTypeRef *)obj_collection_t + index) = CFBridgingRetain((id _Nullable)(map));
//                ((__bridge void(^__strong)(bool))(*((CFTypeRef *)obj_collection_t + index)))(TRUE);
//                return (unsigned long)(index ^ 0UL) && (unsigned long)(integrand)(index);
//            })(iterations);
//            return ^{
//                (integrand = ^ unsigned long (unsigned long index) {
//                    --index;
//                    ((__bridge void(^__strong)(bool))(*((CFTypeRef *)obj_collection_t + index)))(TRUE);
//                    return (unsigned long)(index ^ 0UL) && (unsigned long)(integrand)(index);
//                })(iterations);
//            };
//        };
//    }(&obj_collection);
//};


//typedef typeof(unsigned long (^)(unsigned long)) iterator;
//typedef typeof(void(^__strong)(bool)) mapper;
//typedef typeof(void(^__strong)(bool)) applier;
//static void (^(^(^iterate_)(const unsigned long))(typeof(void(^__strong)(bool))))(void) = ^ (const unsigned long iterations) {
//    const id * obj_collection[iterations];
//    return ^ (const id ** obj_collection_t) {
//        __block iterator integrand;
//        return ^ (void(^map)(bool)) {
//            (integrand = ^ unsigned long (unsigned long index) {
//                --index;
//                *(obj_collection_t + index) = (const id *)&map;
//                ((void(^)(bool))(*(*(obj_collection_t + index))))(TRUE);
//                //                ((void(^)(bool))(*((id *)obj_collection_t + index)))(TRUE);
//                return (unsigned long)(index ^ 0UL) && (unsigned long)(integrand)(index);
//            })(iterations);
//            return ^{
//                ((void(^)(bool))(*(*(obj_collection_t + 0UL))))(TRUE);
//            };
//        };
//    }(&obj_collection[0]);
//};

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    stride = generator(0.f, 1.f, 10);
    printf("stride = %f\n", stride());
    printf("stride = %f\n", stride());
    
    //    iterate_(1)(^ (bool state) {
    //        printf("iterate_+state == %s", (state) ? "TRUE" : "FALSE");
    //    })();
    
    void (^state)(bool) = ^ (bool state) {
        printf("state == %s\n", (state) ? "TRUE" : "FALSE");
    };
    void (^ const * state_t)(bool) = &state;
    void (^toggle_play_pause_button)(void) = audio_controller(audio_state_ref_init(audio_engine(audio_source(audio_renderer())), audio_session()))(1)(state);
    
    objc_setAssociatedObject(self.playPauseButton, @selector(invoke), toggle_play_pause_button, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    [self.playPauseButton addTarget:toggle_play_pause_button action:@selector(invoke) forControlEvents:UIControlEventTouchDown];
}

@end
