#include <stdio.h>
#include <string.h>

#include "sndfile.h"
#include "portaudio.h"
#include <string>

#define FRAMES_PER_BUFFER   (512)

typedef struct
{
    SNDFILE* file;
    SF_INFO      info;
} callback_data_s;


class SoundManager{

private:

SNDFILE* file;
PaStream* stream;
PaError error;
callback_data_s data;


static int callback
(const void* input
    , void* output
    , unsigned long                   frameCount
    , const PaStreamCallbackTimeInfo* timeInfo
    , PaStreamCallbackFlags           statusFlags
    , void* userData
)
{
    float* out;
    callback_data_s* p_data = (callback_data_s*)userData;
    sf_count_t       num_read;

    out = (float*)output;
    p_data = (callback_data_s*)userData;

    /* clear output buffer */
    memset(out, 0, sizeof(float) * frameCount * p_data->info.channels);

    /* read directly into output buffer */
    num_read = sf_read_float(p_data->file, out, frameCount * p_data->info.channels);

    /*  If we couldn't read a full frameCount of samples we've reached EOF */
    if (num_read < frameCount)
    {
        return paComplete;
    }

    return paContinue;
}

public:

    int init() {
        /* init portaudio */
        error = Pa_Initialize();
        if (error != paNoError)
        {
            fprintf(stderr, "Problem initializing\n");
            return 1;
        }
}

int loadSound(const std::string& file) {




    /* Open the soundfile */
    data.file = sf_open(file.c_str(), SFM_READ, &data.info);
    if (sf_error(data.file) != SF_ERR_NO_ERROR)
    {
        fprintf(stderr, "%s\n", sf_strerror(data.file));
        fprintf(stderr, "File: %s\n", "piano2.wav");
        return 1;
    }



    /* Open PaStream with values read from the file */
    error = Pa_OpenDefaultStream(&stream
        , 0                     /* no input */
        , data.info.channels         /* stereo out */
        , paFloat32             /* floating point */
        , data.info.samplerate
        , FRAMES_PER_BUFFER
        , callback
        , &data);        /* our sndfile data struct */
    if (error != paNoError)
    {
        fprintf(stderr, "Problem opening Default Stream\n");
        return 1;
    }



    return 0;

}

int playSound() {

    /* Start the stream */
    error = Pa_StartStream(stream);
    if (error != paNoError)
    {
        fprintf(stderr, "Problem opening starting Stream\n");
        return 1;
    }
    return 0;
}

int stopSound() {
    /* Close the soundfile */
    sf_close(data.file);

    /*  Shut down portaudio */
    error = Pa_CloseStream(stream);
    if (error != paNoError)
    {
        fprintf(stderr, "Problem closing stream\n");
        return 1;
    }

    error = Pa_Terminate();
    if (error != paNoError)
    {
        fprintf(stderr, "Problem terminating\n");
        return 1;
    }
    return 0;
}




};