#include <QApplication>
#include "main_controller.h"
#include <SDL.h>

int test_sound() {
    SDL_Init(SDL_INIT_AUDIO);

    // the representation of our audio device in SDL:
    SDL_AudioDeviceID audio_device;

    // opening an audio device:
    SDL_AudioSpec audio_spec;
    SDL_zero(audio_spec);
    audio_spec.freq = 44100;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = 1;
    audio_spec.samples = 1024;
    audio_spec.callback = NULL;

    audio_device = SDL_OpenAudioDevice(
        NULL, 0, &audio_spec, NULL, 0);
    // pushing 3 seconds of samples to the audio buffer:
    float x = 0;
    for (int i = 0; i < audio_spec.freq * 3; i++) {
        x += .010f;

        // SDL_QueueAudio expects a signed 16-bit value
        // note: "5000" here is just gain so that we will hear something
        int16_t sample = sin(x * 4) * 5000;

        const int sample_size = sizeof(int16_t) * 1;
        SDL_QueueAudio(audio_device, &sample, sample_size);
    }



    // unpausing the audio device (starts playing):
    SDL_PauseAudioDevice(audio_device, 0);

    SDL_Delay(3000);

    SDL_CloseAudioDevice(audio_device);
    SDL_Quit();
}

int main(int argc, char **argv) {
    QApplication application(argc, argv);
    MainController controller;
    return QApplication::exec();
}

