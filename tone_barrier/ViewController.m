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

typedef const bool (^ __strong object_block)(const bool);
object_block object_blk = ^ bool (const bool b) {
    printf("object_blk state == %s\n", (b) ? "TRUE" : "FALSE");
    return b;
};
object_block object_blk_2 = ^ bool (const bool b) {
    printf("object_blk_2 state == %s\n", (b) ? "TRUE" : "FALSE");
    return b;
};
object_block object_blk_3 = ^ bool (const bool b) {
    printf("object_blk_3 state == %s\n", (b) ? "TRUE" : "FALSE");
    return b;
};

object_block (^object_blk_composition)(object_block, object_block) = ^ (object_block object_blk_a, object_block object_blk_b) {
    return ^ bool (const bool b) {
        return object_blk_b(object_blk_a(b));
    };
};

typedef const typeof(object_block * restrict) retained_object;
//
//object_block (^ const __strong object_block_store)(object_block) = ^ (object_block blk_ref) {
//    return ^ (retained_object retained_blk) {
//        Block_release(retained_blk);
//        return (object_block)*retained_blk;
//    }(Block_copy(&blk_ref));
//};

static bool (^(^(^audio_controller_)(const _Nonnull audio_state))(object_block))(void) = ^ (const audio_state _Nonnull state) {
    static object_block object_composition = ^ bool (bool cond) { return cond; };
    static retained_object object_composition_t = &object_composition;
    return ^ (object_block object) {
        *object_composition_t = object_blk_composition(object, *object_composition_t);
        return ^ bool {
            return (*object_composition_t)(state());
        };
    };
};
static bool (^ _Nonnull (^ _Nonnull (^ _Nonnull const (* _Nonnull restrict audio_controller_t))(const _Nonnull audio_state))(object_block))(void) = &audio_controller_;

//static void (^(^(^objects_iterator)(const _Nonnull audio_state))(const unsigned long))(object_block) = ^ (const audio_state _Nonnull state) {
//    return ^ (const unsigned long count) {
//        typeof(const id *) retained_objects[count];
//        return ^ (const id * _Nonnull retained_objects_t) {
//            __block unsigned long object_index = 0;
//            __block bool audio_state_cond;
//            static object_block object_composition = ^ bool (bool cond) { return cond; };
//            static retained_object object_composition_t = &object_composition;
//            return ^ (object_block object) {
//                ^ (unsigned long * object_index_t) {
//                    ^ (const id * _Nonnull retained_objects_t) {
//                        (*object_composition_t)(audio_state_cond);
//                        (*object_index_t = *object_index_t + 1);
//                    }(^ const id * _Nonnull (const id * _Nonnull retained_objects_t) {
//                        *((id * const)retained_objects_t + *object_index_t) = (id)(object_block_store(object));
//                        *object_composition_t = object_blk_composition(((object_block)(*((id * const)retained_objects_t + *object_index_t))), *object_composition_t);
//                        printf("write pointer == %p\n", (*((id * const)retained_objects_t + *object_index_t)));
//                        return (const id *)(retained_objects_t);
//                    }((const id *)(retained_objects_t)));
//                }(&object_index);
//            };
//        }((const id *)&retained_objects);
//    };
//};

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    stride = generator(0.f, 1.f, 10);
    printf("stride = %f\n", stride());
    printf("stride = %f\n", stride());
    
//    void (^object_array)(object_block) = (objects_iterator(audio_state_ref_init(audio_engine(audio_source(audio_renderer())), audio_session())))(3);
//    object_array(object_blk);
//    object_array(object_blk_2);
//    object_array(object_blk_3);
    
    bool (^(^audio_controller_ref)(object_block))(void) = (*audio_controller_t)(audio_state_ref_init(audio_engine(audio_source(audio_renderer())), audio_session()));
    void (^toggle_play_pause_button)(void) = ^ (bool(^toggle_play_pause)(void)) {
        [self.playPauseButton setImage:[UIImage systemImageNamed:@"pause.circle"] forState:UIControlStateSelected];
        [self.playPauseButton setImage:[UIImage systemImageNamed:@"play.circle"]  forState:UIControlStateNormal];
        [self.playPauseButton setImage:[UIImage systemImageNamed:@"play.slash"]   forState:UIControlStateDisabled];
        return ^{
            dispatch_async(dispatch_get_main_queue(), ^{
                [self.playPauseButton setSelected:toggle_play_pause()];
            });
        };
    }(audio_controller_ref(object_blk));
    
    objc_setAssociatedObject(self.playPauseButton, @selector(invoke), toggle_play_pause_button, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    [self.playPauseButton addTarget:toggle_play_pause_button action:@selector(invoke) forControlEvents:UIControlEventTouchDown];
    
    [self.routePickerView setDelegate:(id<AVRoutePickerViewDelegate> _Nullable)self];
}

@end
