#include <stdlib.h>
#include <SDL2/SDL.h>
#include "./graphic.cpp"
#include "./game.cpp"
#include "./memory.cpp"

void handle_event(SDL_Event e) {
  using memory::mem;
  const Uint8* keys = SDL_GetKeyboardState(NULL); // NOT "keys = NULL;"
  memory::set(JOYSTICK, BTN_L, keys[SDL_SCANCODE_LEFT]);
  memory::set(JOYSTICK, BTN_R, keys[SDL_SCANCODE_RIGHT]);
  memory::set(JOYSTICK, BTN_U, keys[SDL_SCANCODE_UP]);
  memory::set(JOYSTICK, BTN_D, keys[SDL_SCANCODE_DOWN]);
  memory::set(JOYSTICK, BTN_X, keys[SDL_SCANCODE_SPACE]);
  printf("%hu", *mem(JOYSTICK));
}

int main(int argc, char *argv[]) {
  using namespace graphic;
  graphic::init();
  memory::init();
  while (1) {
    game::render();
  }
  graphic::quit();
}