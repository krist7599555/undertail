#pragma once
#include <SDL2/SDL.h>
#include "./memory.cpp"
#include "./colors.cpp"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGH 480

namespace graphic {
  
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Event event;

  void init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGH, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
  }

  void clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
  }

  void set_pixel(int x, int y, int color_code) {
    auto color = colors[color_code];
    SDL_SetRenderDrawColor(renderer, hex2red(color), hex2green(color), hex2blue(color), 255);
    SDL_RenderDrawPoint(renderer, x, y);
  }
  void draw_rect(int x, int y, int szx, int szy, int color_code) {
    for (int i = 0; i != szx; ++i) {
    for (int j = 0; j != szy; ++j) {
      set_pixel(x + i, y + j, color_code);
    }}
  }

  void render() {
    SDL_RenderPresent(renderer);
    // SDL_RenderSetScale(renderer, 2, 2);
  }

  void quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

  SDL_Event* poll_event() {
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        graphic::quit();
        exit(0);
      } else {
        const Uint8* keys = SDL_GetKeyboardState(NULL); // NOT "keys = NULL;"
        const uint16_t prev = *memory::mem(JOYSTICK);
        memory::set(JOYSTICK, BTN_L, keys[SDL_SCANCODE_LEFT]);
        memory::set(JOYSTICK, BTN_R, keys[SDL_SCANCODE_RIGHT]);
        memory::set(JOYSTICK, BTN_U, keys[SDL_SCANCODE_UP]);
        memory::set(JOYSTICK, BTN_D, keys[SDL_SCANCODE_DOWN]);
        memory::set(JOYSTICK, BTN_X, keys[SDL_SCANCODE_SPACE]);
        const uint16_t curr = *memory::mem(JOYSTICK);
        *memory::mem(JOYSTICK_ONESHOT) = curr & ~prev;
      }
      return &event;
    } else {
      return nullptr;
    }
  }

  const int FPS = 60;
  const int frame_delay = 1000 / FPS;
  Uint32 _frame_clock;
  void frame_rate_start() {
    _frame_clock = SDL_GetTicks();
  }
  void frame_rate_end() {
    auto use_time = SDL_GetTicks() - _frame_clock;
    if (use_time < frame_delay) {
      SDL_Delay(frame_delay - use_time);
    }
  }
};

