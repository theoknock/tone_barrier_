//
//  ViewController+AudioGenerator.h
//  tone_barrier
//
//  Created by Xcode Developer on 8/28/22.
//

#import "ViewController.h"
#include <math.h>

NS_ASSUME_NONNULL_BEGIN

static const float  high_frequency = 3000.0;
static const float  low_frequency  = 440.0;
static const float  min_duration   = 0.25;
static const float  max_duration   = 2.0;
static const double PI_SQUARED     = 2.0 * M_PI;

static double (^normalize_range)(double *, double *) = ^ (double * a, double * b) {
    return (double)((*a) / (*b));
};

static double (^normalize_value)(double, double, double) = ^double(double min, double max, double value) {
    double result = (value - min) / (max - min);
    return result;
};

static double (^linearize)(double, double, double) = ^ double (double range_min, double range_max, double value) {
    double result = (value * (range_max - range_min)) + range_min;
    
    return result;
};

static double (^scale)(double, double, double, double, double) = ^ double (double val_old, double min_new, double max_new, double min_old, double max_old) {
    double val_new = min_new + ((((val_old - min_old) * (max_new - min_new))) / (max_old - min_old));
    
    return val_new;
};


typedef typeof(double(^)(void)) random_n;
typedef typeof(double(^(* restrict))(void)) random_n_t;
static double (^(^(^(^random_generator)(double(^)(double)))(double(^)(double)))(double(^)(double)))(void) = ^ (double(^(randomizer))(double)) {
    return ^ (double(^(distributor))(double)) {
        srand48((unsigned int)time(0));
        return ^ (double(^(feature_scaler))(double)) {
            static double random;
            return ^ (double * random_t) {
                return ^ double {
                    return (feature_scaler)((distributor)((*random_t = (drand48()))));
                };
            }(&random);
        };
    };
};
//static random_n (^(^random_)(random_n))(void) = ^ (random_n r) {
//    return ^ (random_n * r_ptr) {
//        return ^ random_n {
//            Block_release(r_ptr);
//            return (random_n)(*r_ptr);
//        };
//    }(&r);
//};


static AVAudioFramePosition (^minimum)(AVAudioFramePosition, AVAudioFramePosition) = ^ AVAudioFramePosition (AVAudioFramePosition x, AVAudioFramePosition y) {
    return y ^ ((x ^ y) & -(x < y));
};

static double (^note)(double) = ^ double (double coefficient) {
    int c = round(coefficient);
    double random_note = pow(2.f, c/12.f) * 440.f;
    printf("random_note == %f\n", random_note);
    return random_note;
};

static double (^(^(^signal_frequency)(double))(double *))(void) = ^ (double frequency) {
    static double sample_frequency;
    static double * sample_frequency_t;
    sample_frequency_t = &sample_frequency;
    sample_frequency = (2.0 * M_PI * frequency);
    return ^ (double * time) {
        return ^ double {
            return sin(*sample_frequency_t * (*time));
        };
    };
};

static double (^(^(^signal_amplitude)(double))(double *))(void) = ^ (double amplitude) {
    double sample_amplitude = (M_PI * amplitude);
    return ^ (double * time) {
        return ^ double {
            return sin(sample_amplitude * (*time));
        };
    };
};

static double (^(^(^sample_signal)(double(^)(void)))(double(^(^)(double *))(void), double(^(^)(double *))(void)))(void) = ^ (double(^normalized_time)(void)) {
    static double sample_time;
    static double * sample_time_t;
    sample_time_t = &sample_time;
    static double(^sample_frequency)(void);
    static double(^sample_amplitude)(void);
    return ^ (double(^(^frequency_time)(double *))(void), double(^(^amplitude_time)(double *))(void)) {
        sample_frequency = frequency_time(sample_time_t);
        sample_amplitude = amplitude_time(sample_time_t);
        return ^ double {
            *sample_time_t = normalized_time();
            return sample_frequency() * sample_amplitude();
        };
    };
};



#define max_frequency      1500.0
#define min_frequency       100.0
#define max_trill_interval    4.0
#define min_trill_interval    2.0
#define duration_interval     5.0
#define duration_maximum      2.0

typedef NS_ENUM(NSUInteger, TonalHarmony) {
    TonalHarmonyConsonance,
    TonalHarmonyDissonance,
    TonalHarmonyRandom
};

typedef NS_ENUM(NSUInteger, TonalInterval) {
    TonalIntervalUnison,
    TonalIntervalOctave,
    TonalIntervalMajorSixth,
    TonalIntervalPerfectFifth,
    TonalIntervalPerfectFourth,
    TonalIntervalMajorThird,
    TonalIntervalMinorThird,
    TonalIntervalOffkey,
    TonalIntervalDefault
};

typedef NS_ENUM(NSUInteger, TonalEnvelope) {
    TonalEnvelopeAverageSustain,
    TonalEnvelopeLongSustain,
    TonalEnvelopeShortSustain
};

static unsigned int counter = 0;
static double (^tonal_interval)(TonalInterval) = ^ double (TonalInterval interval) {
    double consonant_harmonic_interval_ratios [8] = {1.0, 2.0, 5.0/3.0, 4.0/3.0, 5.0/4.0, 6.0/5.0, (1.1 + drand48()), 5.0/4.0};
    return consonant_harmonic_interval_ratios[interval % TonalIntervalDefault];
};

static double (^(^tonal)(void))(double) = ^{
    double(^random_tonal_interval)(void) = random_generator(^ double (double n) { return n; })(^ double (double n) { return n; })(^ double (double n) { return scale(n, TonalIntervalUnison, TonalIntervalDefault, 0.0, 1.0); });
    double(^random_tonal_harmony)(void)  = random_generator(^ double (double n) { return (round(pow(n, 0.75))); })(^ double (double n) { return n; })(^ double (double n) { return scale(n, TonalHarmonyConsonance, TonalHarmonyRandom, 0.0, 1.0); });
    return ^ (double root_frequency) {
        TonalInterval interval = (random_tonal_harmony() == TonalHarmonyDissonance) ? TonalIntervalOffkey : random_tonal_interval();
        double harmonic_frequency = root_frequency * interval;
        return harmonic_frequency;
    };
};

static AVAudioPCMBuffer * (^audio_buffer)(void) = ^ AVAudioPCMBuffer * {
    static int note_c;
    static int * note_ct = &note_c;
    random_n r = random_generator(^ double (double n) { return n; })(^ double (double n) { return n; })(^ double (double n) { return scale(n, 0, 12, 0.0, 1.0); });
    double(^t)(double) = tonal();
    const Float32 sampleRate = 48000.f;
    const Float32 amplitude = 0.25f;
    const Float32 M_PI_SQR = 2.f * M_PI;
    Float32 (^phase_channel_l)(AVAudioFrameCount) = ^(AVAudioFrameCount split_frame) {
        __block Float32 theta_increment_t = (M_PI_SQR * note((*note_ct)) / sampleRate);
        return ^ (AVAudioFrameCount frame) {
            if ((frame = frame % 50) == split_frame) (theta_increment_t = (M_PI_SQR * note((*note_ct)) / sampleRate));
            static Float32 theta;
            !((theta += theta_increment_t) > M_PI_SQR) ?: (theta -= M_PI_SQR);
            return sinf(theta);
        };
    }(50);
    
    Float32 (^phase_channel_r)(AVAudioFrameCount) = ^(AVAudioFrameCount split_frame) {
        __block Float32 theta_increment_t = (M_PI_SQR * note((*note_ct)) / sampleRate); // tonal(note()) is wrong; use root_frequency generated by note() on the other channel
        return ^ (AVAudioFrameCount frame) {
            if ((frame = frame % 50) == split_frame) (theta_increment_t = (M_PI_SQR * note((*note_ct)++) / sampleRate));
            static Float32 theta;
            !((theta += theta_increment_t) > M_PI_SQR) ?: (theta -= M_PI_SQR);
            return sinf(theta);
        };
    }(50);
    
   AVAudioFrameCount frame = 0;
        AVAudioFrameCount * frame_t = &frame;
        static int counter = 0;
//        for (; *frame_t < frameCount; (*frame_t = -~(*frame_t))) {
    AVAudioFrameCount frameCount = audio_format_ref.sampleRate * 2.0;
    AVAudioPCMBuffer *pcmBuffer = [[AVAudioPCMBuffer alloc] initWithPCMFormat:audio_format_ref frameCapacity:frameCount];
    pcmBuffer.frameLength = frameCount;
    float *left_channel  = pcmBuffer.floatChannelData[0];
    float *right_channel = (audio_format_ref.channelCount == 2) ? pcmBuffer.floatChannelData[1] : nil;
    
        for (frame = 0; frame < frameCount; frame++) {
            /*((Float32 *)((pcmBuffer.floatChannelData + 0)) + frame)*/*left_channel = (Float32)phase_channel_l(counter) * amplitude;
            /**((Float32 *)((pcmBuffer.floatChannelData + 1)) + frame)*/ *right_channel = (Float32)phase_channel_r(counter) * amplitude;
        }
 
    return pcmBuffer;
};

@interface ViewController ()

@end

NS_ASSUME_NONNULL_END
