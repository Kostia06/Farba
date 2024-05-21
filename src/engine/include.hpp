#pragma once

#include <iostream>
#include <string>
#include <map>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define MOUSE_LEFT 0
#define MOUSE_MIDDLE 1
#define MOUSE_RIGHT 2
#define MOUSE_SCROLL_X 3
#define MOUSE_SCROLL_Y 4
#define MOUSE_X 5
#define MOUSE_Y 6
#define MOUSE_LEFT_PRESSED 7
#define MOUSE_MIDDLE_PRESSED 8
#define MOUSE_RIGHT_PRESSED 9

#define KEY_NUM 2100
#define LEFT_CTRL SDLK_LCTRL % 10000
#define RIGHT_CTRL SDLK_RCTRL % 10000
#define LEFT_SHIFT SDLK_LSHIFT % 10000
#define RIGHT_SHIFT SDLK_RSHIFT % 10000
#define LEFT_ALT SDLK_LALT % 10000
#define RIGHT_ALT SDLK_RALT % 10000
#define LEFT_CMD SDLK_LGUI % 10000
#define RIGHT_CMD SDLK_RGUI % 10000
#define SPACE SDLK_SPACE % 10000
#define ENTER SDLK_RETURN % 10000
#define DELETE SDLK_DELETE % 10000
#define BACKSPACE SDLK_BACKSPACE % 10000
#define TAB SDLK_TAB % 10000
#define ESCAPE SDLK_ESCAPE % 10000
#define UP SDLK_UP % 10000
#define DOWN SDLK_DOWN % 10000
#define LEFT SDLK_LEFT % 10000
#define RIGHT SDLK_RIGHT % 10000

int InitWindow(int width, int height, std::string title);
int InitImage();
int InitFont();
void QuitWindow();

bool UpdateWindow(size_t r = 0, size_t g = 0, size_t b = 0, size_t a = 255);
void PresentWindow();
int* GetWindowSize();
void SetFPS(int fps);
float GetDeltaTime();

int MouseGet(int button);
bool MouseHovering(int x, int y, int w, int h);
bool* KeysGet();
int KeyGet();
void Keywait(int key);


void SetColor(int r, int g, int b, int a = 255);
void SetColor(int hex); 
bool CreateFont(std::string name, std::string path, int size);
bool SetFont(std::string name);
void SetFontSize(int size);
int* FontGetSize(std::string text);


void DrawRect(int x, int y, size_t w, size_t h);
void DrawCircle(int center_x, int center_y, size_t d, size_t fill = 100);
void DrawText(std::string text, int x, int y, int text_size = 1);
void DrawLine(int x1, int y1, int x2, int y2, int line_size = 1); 

int RandomNumber(int min, int max);
#define InRange(x1, y1, x2, y2, w, h) (x1 >= x2 && x1 <= x2+w && y1 >= y2 && y1 <= y2+h) 
