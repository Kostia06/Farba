#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <string>
#include <assert.h>

#include "input.hpp"

using namespace std;

class Farba{
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

        int window_width = 800;
        int window_height = 600;
        
        int mouse[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int mouse_wait[3] = {0, 0, 0};

        bool* keys;
        bool* keys_wait;

        Uint32 last_time = SDL_GetPerformanceCounter();
        Uint32 current_time = 0;
        double frequency = static_cast<double>(SDL_GetPerformanceFrequency());
        double delta_time = 0;

        void handle_event();
    public:
        string title;
        bool running = true;

        Farba(string title = "Farba", size_t window_width = 800, size_t window_height = 600);
        ~Farba();

        bool update(int fixed_fps = 60);
        
        bool key_pressed(int key);
        bool mouse_pressed(int button);

        double get_delta_time();
};
