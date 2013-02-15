/*
 * Really basic sample for the NDS.
 *
 * Fills a rectangle increasingly smaller of random color every time a
 * button (a, b, x, y) is pressed.
 *
 * The behaviour whether SDL is compiled with HW support or not (see
 * USE_HW_RENDERER in Makefile.ds).
 *
 * In framebuffer mode, the old rectangles stay because the screen has
 * not been cleared.
 *
 * In accelerated mode, old the last rectangle is visible.
 *
 * No text is displayed.
 */

#include <SDL/SDL.h>
#if defined(NDS) || defined(__NDS__) || defined (__NDS)
#include <nds.h>
#include <fat.h>
#else
#define consoleDemoInit()
#define fatInitDefault()
#define RGB15(r,g,b) SDL_MapRGB(screen->format,((r)<<3),((g)<<3),((b)<<3))
#endif

int main(void)
{
	SDL_Window *window;
	SDL_Renderer *renderer;
    SDL_Joystick *stick;
    SDL_Event event;
    SDL_Rect rect = { 0, 0, 256, 192 };
    int i;

    consoleDemoInit();
    puts("Hello world!  Initializing FAT...");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        puts("# error initializing SDL");
        puts(SDL_GetError());
        return 1;
    }
    puts("* initialized SDL");

    if (SDL_CreateWindowAndRenderer(256, 192, SDL_RENDERER_ACCELERATED, &window, &renderer) < 0 &&
		SDL_CreateWindowAndRenderer(256, 192, SDL_RENDERER_SOFTWARE, &window, &renderer) < 0) {
        exit(1);
    }

    stick = SDL_JoystickOpen(0);
    if (stick == NULL) {
        puts("# error opening joystick");
        puts(SDL_GetError());
    }
    puts("* opened joystick");

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);

    while (1)
        while (SDL_PollEvent(&event))
            switch (event.type) {
            case SDL_JOYBUTTONDOWN:
				
				SDL_SetRenderDrawColor(renderer, rand(), rand(), rand(), SDL_ALPHA_OPAQUE);
				SDL_RenderFillRect(renderer, &rect);
				SDL_RenderPresent(renderer);
                if (rect.w > 8) {
                    rect.x += 4;
                    rect.y += 3;
                    rect.w -= 8;
                    rect.h -= 6;
                }
				/*
				  printf("button %d pressed at %d ticks\n",
				  event.jbutton.button, SDL_GetTicks()); 
				*/
                break;
            case SDL_QUIT:
                SDL_Quit();
                return 0;
            default:
                break;
            }
    return 0;
}
