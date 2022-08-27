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

typedef const void (^ const __strong object_block)(const bool);
object_block object_blk = ^ (const bool b) {
    printf("object_blk state == %s\n", (b) ? "TRUE" : "FALSE");
};
object_block object_blk_2 = ^ (const bool b) {
    printf("object_blk_2 state == %s\n", (b) ? "TRUE" : "FALSE");
};
object_block object_blk_3 = ^ (const bool b) {
    printf("object_blk_3 state == %s\n", (b) ? "TRUE" : "FALSE");
};

typedef const typeof(object_block *) retained_object;

object_block (^ const __strong object_block_store)(object_block) = ^ (object_block blk_ref) {
    return ^ (retained_object retained_blk) {
        Block_release(retained_blk);
        return (object_block)*retained_blk;
    }(Block_copy(&blk_ref));
};

static void (^(^(^objects_iterator)(const _Nonnull audio_state))(const unsigned long))(object_block) = ^ (const audio_state _Nonnull state) {
    return ^ (const unsigned long count) {
        typeof(const id *) retained_objects[count];
        return ^ (const id * _Nonnull retained_objects_t) {
            static unsigned long object_index = 0;
            __block bool audio_state_cond;
            return ^ (object_block object) {
                ^ (unsigned long * object_index_t) {
                    ^ (const id * _Nonnull retained_objects_t) {
                        for (int i = 0; i < (*object_index_t + 1); i++)
                            ((object_block)(*((id * const)retained_objects_t + *object_index_t)))(audio_state_cond);
                        (*object_index_t = *object_index_t + 1);
                    }(^ const id * _Nonnull (const id * _Nonnull retained_objects_t) {
                        *((id * const)retained_objects_t + *object_index_t) = (id)(object_block_store(object));
                        printf("write pointer == %p\n", (*((id * const)retained_objects_t + *object_index_t)));
                        return (const id *)(retained_objects_t);
                    }((const id *)(retained_objects_t)));
                }(&object_index);
            };
        }((const id *)&retained_objects);
    };
};

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    stride = generator(0.f, 1.f, 10);
    printf("stride = %f\n", stride());
    printf("stride = %f\n", stride());
    
    void (^object_array)(object_block) = (objects_iterator(audio_state_ref_init(audio_engine(audio_source(audio_renderer())), audio_session())))(3);
    object_array(object_blk);
    object_array(object_blk_2);
    object_array(object_blk_3);

//    void (^toggle_play_pause_button)(void) = audio_controller(audio_state_ref_init(audio_engine(audio_source(audio_renderer())), audio_session()))(1)(object);
    
//    objc_setAssociatedObject(self.playPauseButton, @selector(invoke), toggle_play_pause_button, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
//    [self.playPauseButton addTarget:toggle_play_pause_button action:@selector(invoke) forControlEvents:UIControlEventTouchDown];
}

@end
