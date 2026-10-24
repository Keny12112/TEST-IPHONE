/* SDL3 en iPhone: video + tactil + audio.
   Mientras tocas la pantalla suena un tono; mas arriba = mas agudo. */
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define SAMPLE_RATE 44100

static SDL_Window      *window   = NULL;
static SDL_Renderer    *renderer = NULL;
static SDL_AudioStream *audio    = NULL;

static float  touch_x = 0.5f, touch_y = 0.5f; /* normalizado 0..1 */
static bool   touching = false;
static double phase = 0.0;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    (void)appstate; (void)argc; (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL_Init fallo: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!SDL_CreateWindowAndRenderer("Hola iPhone", 0, 0,
            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_HIGH_PIXEL_DENSITY,
            &window, &renderer)) {
        SDL_Log("No se pudo crear la ventana: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_AudioSpec spec;
    SDL_zero(spec);
    spec.format   = SDL_AUDIO_F32;
    spec.channels = 1;
    spec.freq     = SAMPLE_RATE;
    audio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                                      &spec, NULL, NULL);
    if (!audio) {
        SDL_Log("Audio fallo: %s", SDL_GetError());
    } else {
        SDL_ResumeAudioStreamDevice(audio);
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    (void)appstate;
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_FINGER_DOWN:
    case SDL_EVENT_FINGER_MOTION:
        touch_x = event->tfinger.x;
        touch_y = event->tfinger.y;
        touching = true;
        break;
    case SDL_EVENT_FINGER_UP:
        touching = false;
        if (audio) SDL_ClearAudioStream(audio); /* corta el sonido ya */
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

static void feed_audio(void)
{
    if (!audio || !touching) return;

    const double freq = 220.0 + (1.0 - touch_y) * 660.0; /* 220..880 Hz */
    const int target_bytes = SAMPLE_RATE * (int)sizeof(float) / 10; /* ~0.1 s */
    float buf[1024];

    while (SDL_GetAudioStreamQueued(audio) < target_bytes) {
        for (int i = 0; i < 1024; i++) {
            buf[i] = (float)(0.25 * SDL_sin(phase));
            phase += 2.0 * SDL_PI_D * freq / SAMPLE_RATE;
            if (phase > 2.0 * SDL_PI_D) phase -= 2.0 * SDL_PI_D;
        }
        SDL_PutAudioStreamData(audio, buf, sizeof(buf));
    }
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    (void)appstate;
    int w = 0, h = 0;
    SDL_GetCurrentRenderOutputSize(renderer, &w, &h);

    const double t = SDL_GetTicks() / 1000.0;
    const float r = (float)(0.5 + 0.5 * SDL_sin(t));
    const float g = (float)(0.5 + 0.5 * SDL_sin(t + 2.0));
    const float b = (float)(0.5 + 0.5 * SDL_sin(t + 4.0));
    SDL_SetRenderDrawColorFloat(renderer, r * 0.4f, g * 0.4f, b * 0.4f, 1.0f);
    SDL_RenderClear(renderer);

    const float size = (float)(w < h ? w : h) * 0.15f;
    SDL_FRect box = { touch_x * w - size / 2, touch_y * h - size / 2, size, size };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &box);
    SDL_RenderPresent(renderer);

    feed_audio();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    (void)appstate; (void)result;
}
