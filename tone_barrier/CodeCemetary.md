#  Code Cemetary

typedef CFTypeRef _Nonnull (^stored_object)(void);
stored_object (^ _Nonnull store_object)(CFTypeRef) = ^ (CFTypeRef storable_object) {
    return ^ CFTypeRef (void) {
        return (__bridge CFTypeRef)CFBridgingRelease(storable_object);
    };
};

typedef typeof(CFTypeRef(^)(void)) update_control_block_ref;
typedef typeof(CFTypeRef(^ _Nonnull const (* _Nonnull restrict))(void)) update_control_block_ref_t;
typedef typeof(void(^ _Nonnull const)(bool)) update_control_block;
typedef typeof(void(^ _Nonnull const (* _Nonnull restrict))(bool)) update_control_block_t;

static void (^(^(^(^audio_controller)(audio_state))(unsigned long))(CFTypeRef(^)(void)))(void) = ^ (audio_state state) {
    return ^ (unsigned long object_count) {
        typedef CFTypeRef objects[object_count];
        typeof(objects) objects_ptr[object_count];
        return ^ (CFTypeRef * objects_t) {
            return ^ (CFTypeRef(^write_object)(void)) {
                for (unsigned int index = 0; index < object_count; index++) {
                    *((CFTypeRef *)objects_t + index) = write_object();
                }
                return ^{
                    for (unsigned int index = 0; index < object_count; index++) {
                        ((__bridge typeof(void(^)(bool)))(store_object(*((CFTypeRef *)objects_t + index))()))(state());
                    }
                };
            };
        }(objects_ptr);
    };
};

// ------------------------------------------------------------------

id (^retainable_object)(id(^)(void)) = ^ id (id(^object)(void)) {
    return ^{
        return object();
    };
};

id (^(^retain_object)(id(^)(void)))(void) = ^ (id(^retainable_object)(void)) {
    id retained_object = retainable_object();
    return ^ id {
        return retained_object;
    };
};

typedef typeof(unsigned long (^)(unsigned long)) recursive_iterator;
static void (^(^_iterator_)(const unsigned long))(id(^)(void)) = ^ (const unsigned long object_count) {
    typeof(id(^)(void)) retained_objects_ref;
    return ^ (id * retained_objects_t) {
        return ^ (id(^object)(void)) {
            //            return ^ (void (^(^retain_objects)(unsigned long))(void(^)(void))) {
            //                return ^ (unsigned long index) {
            //                    return retain_objects(index)(^{
            for (int index = 0 ; index < object_count; index++) {
                printf("retained_object: %p\n", (*((id * const)retained_objects_t + index) = retain_object(retainable_object(object))));
                ((typeof(update_control_block))(*((id * const)retained_objects_t + index)))(FALSE);
            }
            //        });
            //                }(object_count);
            //            };
        };
    }((id *)&retained_objects_ref);
};

/*
 id (^player_node_object)(void) = ^ id {
 AVAudioPlayerNode * player = [[AVAudioPlayerNode alloc] init];
 [player setRenderingAlgorithm:AVAudio3DMixingRenderingAlgorithmAuto];
 [player setSourceMode:AVAudio3DMixingSourceModeAmbienceBed];
 [player setPosition:AVAudioMake3DPoint(0.0, 0.0, 0.0)];
 
 return player;
}; iterator(2)(player_node_object);
 
 */


// ------------------------------------------------------------------

static void (^(^(^(^audio_controller)(audio_state))(unsigned int))(update_control_block_ref))(void) = ^ (audio_state state) {
    return ^ (unsigned int control_count) {
        typedef CFTypeRef update_control_blocks[control_count];
        typeof(update_control_blocks) update_control_blocks_ptr[control_count];
        return ^ (CFTypeRef * update_control_blocks_t) {
            __block unsigned long (^recursive_block)(unsigned long);
            return ^ (update_control_block_ref update_control_block) {
                return ^ (unsigned long offset) {
//                    *((CFTypeRef *)update_control_blocks_t) = CFBridgingRetain((__bridge id _Nullable)(update_control_block()));
//                    //                    *(update_control_blocks_t + offset) = &update_control_block;
//                    (recursive_block = ^ unsigned long (unsigned long index) {
//                        //                            update_control_block_ref update_control = *(*(update_control_blocks_t + 0));
//                        update_control_block update_control = (*((CFTypeRef *)update_control_blocks_t + index));
//                        (update_control)(state()); // WRONG: capture the return value of state and then pass that value in
//                        return (unsigned long)(0UL ^ (unsigned long)(recursive_block)(~-index));
//                    })(~-offset);
                };
            }(control_count);
        };
    }(&update_control_blocks_ptr[0]);
};
static void (^ _Nonnull (^ _Nonnull const (* _Nonnull restrict audio_controller_t))(void))(void(^)(bool)) = &audio_controller;

/*
 
 */


static int c = 0;

static void (^(^(^(^(^array_pointer_test)(unsigned int))(CFTypeRef(^)(void)))(void(^)(CFTypeRef)))(CFTypeRef(^)(CFTypeRef)))(bool(^)(CFTypeRef)) = ^ (unsigned int object_count) {
    typedef CFTypeRef objects[object_count];
    typeof(objects) objects_ptr[object_count];
    return ^ (CFTypeRef * objects_t) {
        
        __block void (^recurse)(void(^__strong)(CFTypeRef)) = ^{
            __block unsigned long (^recursive_block)(unsigned long);
            return ^ (void(^block)(CFTypeRef)) {
                (recursive_block = ^ unsigned long (unsigned long index) {
                    printf("index == %lu\t", index);
                    block((objects_t + (index * sizeof(CFTypeRef))));
                    return (unsigned long)(index ^ 0UL) && (unsigned long)recursive_block(~-index);
                })(~-object_count);
            };
        }();
        
        return ^ (CFTypeRef(^write_object)(void)) {
            for (unsigned int index = 0; index < object_count; index++) {
                *((CFTypeRef *)objects_t + index) = CFBridgingRetain((__bridge id _Nullable)(write_object()));
                printf("Stored number_write = %lu\n", [(NSNumber *)((__bridge id)(*((CFTypeRef *)objects_t + index))) unsignedLongValue]);
            }
            return ^ (void(^read_object)(CFTypeRef)) {
                for (unsigned int index = 0; index < object_count; index++) {
                    read_object((*((CFTypeRef *)objects_t + index)));
                    printf("Stored number_read = %lu\n", [(NSNumber *)((__bridge id)(*((CFTypeRef *)objects_t + index))) unsignedLongValue]);
                }
                return ^ (CFTypeRef(^modify_object)(CFTypeRef)) {
                    for (unsigned int index = 0; index < object_count; index++) {
                        *((CFTypeRef *)objects_t + index) = CFBridgingRetain((__bridge id _Nullable)(modify_object((*((CFTypeRef *)objects_t + index)))));
                        printf("modified_number = %lu\n", [(NSNumber *)((__bridge id)(*((CFTypeRef *)objects_t + index))) unsignedLongValue]);
                    }
                    return ^ (bool(^filter_object)(CFTypeRef)) {
                        unsigned long unfiltered_object_count = object_count;
                        for (unsigned int index = 0; index < unfiltered_object_count; index++) {
                            CFTypeRef subject_object = (*((CFTypeRef *)objects_t + index));
                            (filter_object(subject_object)) ? ^{ printf("number %lu is odd\n", [(__bridge NSNumber *)subject_object unsignedLongValue]); (*((CFTypeRef *)objects_t + index) = CFBridgingRetain((__bridge id _Nullable)(subject_object))); }()
                            : ^ (unsigned int * object_count_t){ *object_count_t -= 1; }(&object_count);
                        }
                        array_pointer_test(object_count)(^ CFTypeRef {
                            __block NSNumber * number = [[NSNumber alloc] initWithUnsignedLong:c++];
                            printf("Sent number_write == %lu\n", [number unsignedLongValue]);
                            return (__bridge CFTypeRef)(number);
                        });
                        printf("modified object_count == %lu\n", object_count);
                    };
                };
            };
        };
    }(objects_ptr);
};

// Sample usage

//    array_pointer_test(10)(^ CFTypeRef {
//        __block NSNumber * number = [[NSNumber alloc] initWithUnsignedLong:c++];
//        printf("Sent number_write == %lu\n", [number unsignedLongValue]);
//        return (__bridge CFTypeRef)(number);
//    })(^ (CFTypeRef number) {
//        printf("Returned number_read == %lu\n", [(__bridge NSNumber *)number unsignedLongValue]);
//    })(^ CFTypeRef (CFTypeRef number) {
//        number = (__bridge CFTypeRef)([[NSNumber alloc] initWithUnsignedLong:c++]);
//        return (CFTypeRef)(number);
//    })(^ bool (CFTypeRef number) {
//        return ([(__bridge NSNumber *)number unsignedLongValue] % 2);
//    });

/*
 
 */

static void (^(^(^notification_observer)(NSNotificationCenter *))(NSNotification *, typeof(void(^)(NSNotification *)))(void)) = ^ (NSNotificationCenter * notification_center) {
    return ^ (NSNotification * observed_notification, typeof(void(^)(NSNotification *))notification_handler) {
        return ^{
            [notification_center addObserverForName:observed_notification.name object:observed_notification.object queue:[NSOperationQueue mainQueue] usingBlock:notification_handler];
        };
    };
};



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
            __block unsigned long object_index = 0;
            __block bool audio_state_cond;
            static object_block object_composition = ^ bool (bool cond) { return cond; };
            static retained_object object_composition_t = &object_composition;
            return ^ (object_block object) {
                ^ (unsigned long * object_index_t) {
                    ^ (const id * _Nonnull retained_objects_t) {
                        (*object_composition_t)(audio_state_cond);
                        (*object_index_t = *object_index_t + 1);
                    }(^ const id * _Nonnull (const id * _Nonnull retained_objects_t) {
                        *((id * const)retained_objects_t + *object_index_t) = (id)(object_block_store(object));
                        *object_composition_t = object_blk_composition(((object_block)(*((id * const)retained_objects_t + *object_index_t))), *object_composition_t);
                        printf("write pointer == %p\n", (*((id * const)retained_objects_t + *object_index_t)));
                        return (const id *)(retained_objects_t);
                    }((const id *)(retained_objects_t)));
                }(&object_index);
            };
        }((const id *)&retained_objects);
    };
};

    void (^object_array)(object_block) = (objects_iterator(audio_state_ref_init(audio_engine(audio_source(audio_renderer())), audio_session())))(3);
    object_array(object_blk);
    object_array(object_blk_2);
    object_array(object_blk_3);
    
// ---------------

typedef typeof(const int(^)(void)) generator;
typedef typeof(unsigned long(^)(unsigned long)) recursive_block_wrapper;
typedef typeof(void(^)(const int)) recursive_block_auxillary;
static void (^(^iterator)(const int, const int, const int))(void(^)(const int)) = ^ (const int start, const int stride, const int length) {
    static int result;
    result = start;
    static int end;
    end = start + (stride * length);
    const int iterations = (const int)(round(length / stride));
    
    __block recursive_block_wrapper wrapper_block;
    return ^ (recursive_block_auxillary auxillary_block) {
        (wrapper_block = ^ unsigned long (unsigned long index) {
            auxillary_block((result = ((result += stride) < end) ? result : start));
            return (unsigned long)(index ^ 0UL) && (unsigned long)wrapper_block(~-index);
        })(iterations);
    };
};

        /*
        iterator(0, 1, frameCount)(^ (const int frame) {
            *(buffer + frame) = sin(theta) * amplitude;
            theta += theta_increment;
            !(theta > M_PI_SQR) ?: (theta -= M_PI_SQR);
        });
       */
             
