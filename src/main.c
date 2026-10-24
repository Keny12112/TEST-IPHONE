/* Programa minimo con SDL3: pinta la pantalla y un cuadro que sigue tu dedo.
   Sirve para validar toda la cadena: compilar -> .ipa -> instalar en iPhone. */
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window   *window   = NULL;
static SDL_Renderer *renderer = NULL;
static float touch_x = 0.5f, touch_y = 0.5f; /* normalizado 0..1 */

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    (void)appstate; (void)argc; (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init fallo: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!SDL_CreateWindowAndRenderer("Hola iPhone", 0, 0,
            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_HIGH_PIXEL_DENSITY,
            &window, &renderer)) {
        SDL_Log("No se pudo crear la ventana: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_Log("Render driver: %s", SDL_GetRendererName(renderer));
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
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    (void)appstate;
    int w = 0, h = 0;
    SDL_GetCurrentRenderOutputSize(renderer, &w, &h);

    /* fondo que cambia de color con el tiempo, para ver que hay animacion */
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
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    (void)appstate; (void)result;
}
