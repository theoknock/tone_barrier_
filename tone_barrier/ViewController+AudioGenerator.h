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

static double (^(^generate_normalized_random)(void))(void) = ^{
    srand48((unsigned int)time(0));
    static double random;
    return ^ (double * random_t) {
        return ^ double {
            return (*random_t = (drand48()));
        };
    }(&random);
};

typedef typeof(double(^)(void)) random_generator;
typedef typeof(double(^(* restrict))(void)) random_n_t;
static double (^(^(^(^generate_random)(double(^)(void)))(double(^)(double)))(double(^)(double)))(void) = ^ (double(^randomize)(void)) {
    return ^ (double(^distribute)(double)) {
        return ^ (double(^scale)(double)) {
                return ^ double {
                    return scale(distribute(randomize()));
                };
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

typedef NS_ENUM(unsigned int, MusicalNote) {
    MusicalNoteA,
    MusicalNoteBFlat,
    MusicalNoteB,
    MusicalNoteC,
    MusicalNoteCSharp,
    MusicalNoteD,
    MusicalNoteDSharp,
    MusicalNoteE,
    MusicalNoteF,
    MusicalNoteFSharp,
    MusicalNoteG,
    MusicalNoteAFlat
};

typedef NS_ENUM(unsigned int, MusicalNoteFrequency) {
    MusicalNoteFrequencyA      = 440,
    MusicalNoteFrequencyBFlat  = 466,
    MusicalNoteFrequencyB      = 494,
    MusicalNoteFrequencyC      = 523,
    MusicalNoteFrequencyCSharp = 554,
    MusicalNoteFrequencyD      = 587,
    MusicalNoteFrequencyDSharp = 622,
    MusicalNoteFrequencyE      = 659,
    MusicalNoteFrequencyF      = 698,
    MusicalNoteFrequencyFSharp = 740,
    MusicalNoteFrequencyG      = 784,
    MusicalNoteFrequencyAFlat  = 831
};

typedef struct __attribute__((objc_boxable)) MusicalNotes MusicalNotes;
struct __attribute__((objc_boxable)) MusicalNotes
{
    MusicalNoteFrequency note_frequency[12];
}  musical_note = { .note_frequency = { MusicalNoteFrequencyA, MusicalNoteFrequencyBFlat, MusicalNoteFrequencyB, MusicalNoteFrequencyC, MusicalNoteFrequencyCSharp, MusicalNoteFrequencyD, MusicalNoteFrequencyDSharp, MusicalNoteFrequencyE, MusicalNoteFrequencyF, MusicalNoteFrequencyFSharp, MusicalNoteFrequencyG, MusicalNoteFrequencyAFlat } };

static unsigned int counter = 0;
static double (^tonal_interval)(TonalInterval) = ^ double (TonalInterval interval) {
    double consonant_harmonic_interval_ratios [8] = {1.0, 2.0, 5.0/3.0, 4.0/3.0, 5.0/4.0, 6.0/5.0, (1.1 + drand48()), 5.0/4.0};
    return consonant_harmonic_interval_ratios[interval % TonalIntervalDefault];
};

//static double (^(^tonal)(void))(double) = ^{
//    double(^random_tonal_interval)(void) = random_generator(^ double (double n) { return n; })(^ double (double n) { return n; })(^ double (double n) { return scale(n, TonalIntervalUnison, TonalIntervalDefault, 0.0, 1.0); });
//    double(^random_tonal_harmony)(void)  = random_generator(^ double (double n) { return (round(pow(n, 0.75))); })(^ double (double n) { return n; })(^ double (double n) { return scale(n, TonalHarmonyConsonance, TonalHarmonyRandom, 0.0, 1.0); });
//    return ^ (double root_frequency) {
//        TonalInterval interval = (random_tonal_harmony() == TonalHarmonyDissonance) ? TonalIntervalOffkey : random_tonal_interval();
//        double harmonic_frequency = root_frequency * interval;
//        return harmonic_frequency;
//    };
//};

typedef typeof(AVAudioPCMBuffer * (^)(void)) audio_buffer_ref;
audio_buffer_ref (^audio_buffer)(AVAudioFormat *) = ^ (AVAudioFormat * buffer_format) {
    return ^ AVAudioPCMBuffer * {
        static AVAudioPCMBuffer * audio_buffer_ref;
        static int note_c;
        static int * note_ct = &note_c;
        random_generator random_musical_note_generator = generate_random(generate_normalized_random())(^ double (double n) { return n; })(^ double (double n) { return pow(2.f, round(scale(n, MusicalNoteA, MusicalNoteAFlat, 0.0, 1.0))/12.f) * 440.f; });
        printf("frequency == %f", random_musical_note_generator());
        //    double(^t)(double) = tonal();
        const Float32 sampleRate = buffer_format.sampleRate;
        const Float32 amplitude = 0.25f;
        const Float32 M_PI_SQR = 2.f * M_PI;
        Float32 (^phase_channel_l)(AVAudioFrameCount) = ^(Float32 theta_increment_t) {
            static Float32 theta;
            return ^ (AVAudioFrameCount frame) {
                !((theta += theta_increment_t) > M_PI_SQR) ?: (theta -= M_PI_SQR);
                return sinf(theta);
            };
        }((M_PI_SQR * random_musical_note_generator() / sampleRate));
        
        Float32 (^phase_channel_r)(AVAudioFrameCount) = ^(Float32 theta_increment_t) {
            static Float32 theta;
            return ^ (AVAudioFrameCount frame) {
                !((theta += theta_increment_t) > M_PI_SQR) ?: (theta -= M_PI_SQR);
                return sinf(theta);
            };
        }((M_PI_SQR * random_musical_note_generator() / sampleRate));
        
        
        AVAudioFrameCount frame = 0;
        AVAudioFrameCount * frame_t = &frame;
        static int counter = 0;
        AVAudioFrameCount frameCount = buffer_format.sampleRate * 2.0;
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            audio_buffer_ref = [[AVAudioPCMBuffer alloc] initWithPCMFormat:buffer_format frameCapacity:frameCount];
        });
        audio_buffer_ref.frameLength = frameCount;
        float *left_channel  = audio_buffer_ref.floatChannelData[0];
        float *right_channel = (buffer_format.channelCount == 2) ? audio_buffer_ref.floatChannelData[1] : nil;
        
        for (; *frame_t < frameCount; (*frame_t = -~(*frame_t))) {
            /*((Float32 *)((pcmBuffer.floatChannelData + 0)) + frame)*/left_channel[*frame_t] = (Float32)phase_channel_l(*frame_t) * 1.0;
            /**((Float32 *)((pcmBuffer.floatChannelData + 1)) + frame)*/ right_channel[*frame_t] = (Float32)phase_channel_r(*frame_t) * 1.0;
        }
        
        return audio_buffer_ref;
    };
};

@interface ViewController ()

@end

NS_ASSUME_NONNULL_END
