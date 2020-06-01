#pragma once
#include <algorithm>
#include "./graphic.cpp"
#include "./memory.cpp"
#include "./font.cpp"
#include "./bitmap/heart.cpp"


class Renderable {
  virtual void render() = 0;
};

struct Vector {
  int x, y;
  Vector(int _x = 0, int _y = 0): x(_x), y(_y) {}
  Vector(const Vector& v): x(v.x), y(v.y) {}
};

struct Text : public Renderable, Vector {
  const char* str;
  const uint8_t color_code;
  Text(const char* s, int x, int y, int c): Vector(x, y), str(s), color_code(c) {}
  void render() {
    for (int c = 0; str[c]; ++c) {
      auto& cur = font8x8[(int)str[c]];
      for (int i = 0; i != 8; ++i) {
      for (int j = 0; j != 8; ++j) {
        if (cur[j] & 1 << i) {
          graphic::set_pixel(c * 8 + x + i, y + j, color_code);
        }
    }}}
  }
};

struct Sprite : public Renderable, Vector {
  const int* bitmap;
  uint8_t color_code;
  Vector size;
  Sprite(int x, int y, const int* bm, int szx, int szy, uint8_t clr = CODE_WHITE): Vector(x, y), bitmap(bm), size(szx, szy), color_code(clr) {}
  Sprite& color(uint8_t c) {
    color_code = c;
    return *this;
  }
  void render() {
    for (int i = 0; i != size.x; ++i) {
    for (int j = 0; j != size.y; ++j) {
      if (bitmap[j] & 1 << i) {
        graphic::set_pixel(x + i, y + j, color_code);
      }
    }}
  }
};

struct Rectangle : public Renderable, Vector {
  Vector size;
  uint8_t color_code;
  Rectangle(int x, int y, int szx, int szy, uint8_t clr = CODE_WHITE): Vector(x, y), size(szx, szy), color_code(clr) {}
  void render() {
    for (int i = 0; i != size.x; ++i) {
    for (int j = 0; j != size.y; ++j) {
      graphic::set_pixel(x + i, y + j, color_code);
    }}
  }
};

const int SCENE_INTRO = 0;
const int SCENE_BAR   = 1;
const int SCENE_MAZE  = 2;
const int SCENE_GAME_OVER = 3;
const int SCENE_WIN  = 4;



namespace game {
  int my_life_point = 40;
  int enermy_life_point = 100;
  int scene = SCENE_INTRO;

  void reset() {
    my_life_point = 40;
    enermy_life_point = 100;
  }

  int render_scene_intro() {
    reset();
    Text a("6031301721 Krist",   100, 100, CODE_WHITE);
    Text b("6031301721 Icekang", 100, 110, CODE_WHITE);
    Text c("6031301721 Piew",    100, 120, CODE_WHITE);
    while (1) {
      graphic::frame_rate_start();
      graphic::poll_event();
      graphic::clear();
      if (memory::is_set(JOYSTICK_ONESHOT, BTN_X)) {
        return SCENE_BAR;
      }
      a.render();
      b.render();
      c.render();
      graphic::render();
      graphic::frame_rate_end();
    }
  }

  template <typename T1, typename T2>
  bool is_overlap(const T1& lhs, const T2& rhs) {
    if (lhs.x > rhs.x + rhs.size.x || rhs.x > lhs.x + lhs.size.x) return false;   
    if (lhs.y > rhs.y + rhs.size.y || rhs.y > lhs.y + lhs.size.y) return false;   
    return true; 
  }

  void render_life() {
    static char my_life[10] = "";
    static char en_life[10] = "";
    static Text my(my_life, 50, 50, CODE_WHITE);
    static Text en(en_life, 50, 60, CODE_WHITE);
    sprintf(my_life, "HP %d", my_life_point);
    sprintf(en_life, "EN %d", enermy_life_point);
    my.render();
    en.render();
  }

  int render_scene_maze() {
    Rectangle outer_box(100, 100, 150, 150, CODE_WHITE);
    Rectangle inner_box(105, 105, 140, 140, CODE_BLACK);
    Sprite heart(180, 150, HEART, HEART_SIZE_X, HEART_SIZE_Y, CODE_RED);
    Rectangle enemy_1(115, 135, 20, 20, CODE_YELLOW);
    Vector enermy_1_velocity(3, 2);
    char clk[10];
    Text clock(clk, 150, 70, CODE_WHITE);
    int count_down = 5 * graphic::FPS;
    while (count_down) {
      graphic::frame_rate_start();
      graphic::poll_event();
      graphic::clear();
      if (memory::is_set(JOYSTICK, BTN_U)) heart.y -= 1;
      if (memory::is_set(JOYSTICK, BTN_D)) heart.y += 1;
      if (memory::is_set(JOYSTICK, BTN_L)) heart.x -= 1;
      if (memory::is_set(JOYSTICK, BTN_R)) heart.x += 1;
      heart.x = std::clamp(heart.x, inner_box.x, inner_box.x + inner_box.size.x - heart.size.x);
      heart.y = std::clamp(heart.y, inner_box.y, inner_box.y + inner_box.size.y - heart.size.y);
      enemy_1.x += enermy_1_velocity.x;
      enemy_1.y += enermy_1_velocity.y;
      if (enemy_1.x < inner_box.x || enemy_1.x > inner_box.x + inner_box.size.x - enemy_1.size.x) enermy_1_velocity.x *= -1;
      if (enemy_1.y < inner_box.y || enemy_1.y > inner_box.y + inner_box.size.y - enemy_1.size.y) enermy_1_velocity.y *= -1;
      outer_box.render();
      inner_box.render();
      heart.render();
      enemy_1.render();
      if (is_overlap(heart, enemy_1)) {
        my_life_point -= 1;
      }
      if (my_life_point <= 0) return SCENE_GAME_OVER;
      render_life();
      count_down -= 1;
      sprintf(clk, "TIME %.2f", double(count_down) / graphic::FPS);
      clock.render();
      graphic::render();
      graphic::frame_rate_end();
    }
    return SCENE_BAR;
  }

  int render_scene_bar() {
    Rectangle fair_area(300, 200, 50, 10, CODE_YELLOW);
    Rectangle good_area(315, 195, 20, 25, CODE_GREEN);
    Rectangle bar(50, 105, 3, 140, CODE_WHITE);
    bool pause = false;
    while (1) {
      graphic::frame_rate_start();
      graphic::poll_event();
      graphic::clear();
      if (memory::is_set(JOYSTICK_ONESHOT, BTN_X)) {
        if (not pause) {
          pause = true;
          if (is_overlap(bar, good_area)) enermy_life_point -= 20;
          if (is_overlap(bar, fair_area)) enermy_life_point -= 10;
          if (enermy_life_point <= 0) return SCENE_WIN;
        } else {
          return SCENE_MAZE;
        }
      }
      if (not pause) {
        bar.x += 5;
      }
      if (bar.x > WINDOW_WIDTH) {
        return SCENE_MAZE;
      }
      fair_area.render();
      good_area.render();
      bar.render();
      render_life();
      graphic::render();
      graphic::frame_rate_end();
    }
  }

  int render_scene_game_over() {
    Text text1("Game Over", 150, 70, CODE_WHITE);
    Text text2("press [space] to restart", 150, 90, CODE_WHITE);
    while (1) {
      graphic::frame_rate_start();
      graphic::poll_event();
      graphic::clear();
      text1.render();
      text2.render();
      if (memory::is_set(JOYSTICK_ONESHOT, BTN_X)) {
        return SCENE_INTRO;
      }
      graphic::render();
      graphic::frame_rate_end();
    }
  }
  int render_scene_win() {
    Text text1("WIN", 150, 70, CODE_WHITE);
    Text text2("press [space] to restart", 150, 90, CODE_WHITE);
    while (1) {
      graphic::frame_rate_start();
      graphic::poll_event();
      graphic::clear();
      text1.render();
      text2.render();
      if (memory::is_set(JOYSTICK_ONESHOT, BTN_X)) {
        return SCENE_INTRO;
      }
      graphic::render();
      graphic::frame_rate_end();
    }
  }

  void render() {
    while (1) {
      if (scene == SCENE_INTRO) {
        scene = render_scene_intro();
      }
      if (scene == SCENE_BAR) {
        scene = render_scene_bar();
      }
      if (scene == SCENE_MAZE) {
        scene = render_scene_maze();
      }
      if (scene == SCENE_WIN) {
        scene = render_scene_win();
      }
      if (scene == SCENE_GAME_OVER) {
        scene = render_scene_game_over();
      }
    }
  }
};

